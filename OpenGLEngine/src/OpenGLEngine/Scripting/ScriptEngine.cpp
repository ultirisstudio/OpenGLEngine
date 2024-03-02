#include "depch.h"
#include "ScriptEngine.h"

#include "ScriptGlue.h"

#include <fstream>

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/object.h>

#include <glm/glm.hpp>

#include <FileWatch.h>

#include <OpenGLEngine/Core/Application.h>
#include <OpenGLEngine/Scene/Scene.h>
#include <OpenGLEngine/Entity/Entity.h>

#include <OpenGLEngine/Entity/Components/ScriptComponent.h>

#include <OpenGLEngine/Tools/UUID.h>

namespace OpenGLEngine
{
	namespace Utils
	{
		static char* ReadBytes(const std::filesystem::path& filepath, uint32_t* outSize)
		{
			std::ifstream stream(filepath, std::ios::binary | std::ios::ate);

			if (!stream)
			{
				// Failed to open the file
				return nullptr;
			}

			std::streampos end = stream.tellg();
			stream.seekg(0, std::ios::beg);
			uint64_t size = end - stream.tellg();

			if (size == 0)
			{
				// File is empty
				return nullptr;
			}

			char* buffer = new char[size];
			stream.read((char*)buffer, size);
			stream.close();

			*outSize = (uint32_t)size;
			return buffer;
		}

		static MonoAssembly* LoadMonoAssembly(const std::filesystem::path& assemblyPath)
		{
			uint32_t fileSize = 0;
			char* fileData = ReadBytes(assemblyPath, &fileSize);

			// NOTE: We can't use this image for anything other than loading the assembly because this image doesn't have a reference to the assembly
			MonoImageOpenStatus status;
			MonoImage* image = mono_image_open_from_data_full(fileData, fileSize, 1, &status, 0);

			if (status != MONO_IMAGE_OK)
			{
				const char* errorMessage = mono_image_strerror(status);
				// Log some error message using the errorMessage data
				return nullptr;
			}

			std::string string = assemblyPath.string();
			MonoAssembly* assembly = mono_assembly_load_from_full(image, string.c_str(), &status, 0);
			mono_image_close(image);

			// Don't forget to free the file data
			delete[] fileData;

			return assembly;
		}

		void PrintAssemblyTypes(MonoAssembly* assembly)
		{
			MonoImage* image = mono_assembly_get_image(assembly);
			const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
			int32_t numTypes = mono_table_info_get_rows(typeDefinitionsTable);

			for (int32_t i = 0; i < numTypes; i++)
			{
				uint32_t cols[MONO_TYPEDEF_SIZE];
				mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);

				const char* nameSpace = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]);
				const char* name = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);

				std::cout << nameSpace << "." << name << std::endl;
			}
		}
	}

	struct ScriptEngineData
	{
		MonoDomain* RootDomain = nullptr;
		MonoDomain* AppDomain = nullptr;

		MonoAssembly* CoreAssembly = nullptr;
		MonoImage* CoreAssemblyImage = nullptr;

		MonoAssembly* AppAssembly = nullptr;
		MonoImage* AppAssemblyImage = nullptr;

		std::filesystem::path CoreAssemblyFilepath = "Scripts/OpenGLEngine-ScriptCore.dll";
		std::filesystem::path AppAssemblyFilepath = "";

		ScriptClass EntityClass;

		std::unordered_map<std::string, std::shared_ptr<ScriptClass>> EntityClasses;
		std::unordered_map<UUID, std::shared_ptr<ScriptInstance>> EntityInstances;

		Scene* SceneContext = nullptr;
	};

	static ScriptEngineData* s_Data = nullptr;

	void ScriptEngine::Init()
	{
		s_Data = new ScriptEngineData();

		InitMono();
		ScriptGlue::RegisterFunctions();

		bool status = LoadAssembly(s_Data->CoreAssemblyFilepath);
		if (!status)
		{
			std::cout << "Failed to load the core assembly" << std::endl;
			return;
		}

		status = LoadAppAssembly(s_Data->AppAssemblyFilepath);
		if (!status)
		{
			std::cout << "Failed to load the app assembly" << std::endl;
			return;
		}

		LoadAssemblyClasses();

		ScriptGlue::RegisterComponents();

		s_Data->EntityClass = ScriptClass("OpenGLEngine", "Entity", true);
	}

	void ScriptEngine::Shutdown()
	{
		ShutdownMono();

		delete s_Data;
	}

	bool ScriptEngine::LoadAssembly(const std::filesystem::path& filepath)
	{
		s_Data->AppDomain = mono_domain_create_appdomain("OpenGLEngineScriptRuntime", nullptr);
		mono_domain_set(s_Data->AppDomain, true);

		s_Data->CoreAssembly = Utils::LoadMonoAssembly(filepath);
		if (s_Data->CoreAssembly == nullptr)
		{
			std::cout << "Failed to load the core assembly" << std::endl;
			return false;
		}

		s_Data->CoreAssemblyImage = mono_assembly_get_image(s_Data->CoreAssembly);

		return true;
	}

	bool ScriptEngine::LoadAppAssembly(const std::filesystem::path& filepath)
	{
		s_Data->AppAssembly = Utils::LoadMonoAssembly(filepath);
		if (s_Data->AppAssembly == nullptr)
		{
			std::cout << "Failed to load the app assembly" << std::endl;
			return false;
		}

		s_Data->AppAssemblyImage = mono_assembly_get_image(s_Data->AppAssembly);

		return true;
	}

	void ScriptEngine::ReloadAssembly()
	{
		mono_domain_set(mono_get_root_domain(), false);

		mono_domain_unload(s_Data->AppDomain);

		LoadAssembly(s_Data->CoreAssemblyFilepath);
		LoadAppAssembly(s_Data->AppAssemblyFilepath);
		LoadAssemblyClasses();

		ScriptGlue::RegisterComponents();

		s_Data->EntityClass = ScriptClass("OpenGLEngine", "Entity", true);
	}

	void ScriptEngine::OnRuntimeStart(Scene* scene)
	{
		s_Data->SceneContext = scene;
	}

	void ScriptEngine::OnRuntimeStop()
	{
		s_Data->SceneContext = nullptr;

		s_Data->EntityInstances.clear();
	}

	bool ScriptEngine::EntityClassExist(const std::string& fullClassName)
	{
		return s_Data->EntityClasses.find(fullClassName) != s_Data->EntityClasses.end();
	}

	void ScriptEngine::OnCreateEntity(Entity entity)
	{
		const auto& sc = entity.GetComponent<ScriptComponent>();
		if (EntityClassExist(sc.m_Name))
		{
			std::shared_ptr<ScriptInstance> instance = std::make_shared<ScriptInstance>(s_Data->EntityClasses[sc.m_Name], entity);
			s_Data->EntityInstances[entity.GetUUID()] = instance;

			instance->InvokeOnCreate();
		}
	}

	void ScriptEngine::OnUpdateEntity(Entity entity, float ts)
	{
		UUID uuid = entity.GetUUID();

		if (s_Data->EntityInstances.find(uuid) == s_Data->EntityInstances.end())
			return;

		std::shared_ptr<ScriptInstance> instance = s_Data->EntityInstances[uuid];
		instance->InvokeOnUpdate(ts);
	}

	Scene* ScriptEngine::GetSceneContext()
	{
		return s_Data->SceneContext;
	}

	std::unordered_map<std::string, std::shared_ptr<ScriptClass>> ScriptEngine::GetEntityClasses()
	{
		return s_Data->EntityClasses;
	}

	void ScriptEngine::LoadAssemblyClasses()
	{
		s_Data->EntityClasses.clear();

		const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(s_Data->AppAssemblyImage, MONO_TABLE_TYPEDEF);
		int32_t numTypes = mono_table_info_get_rows(typeDefinitionsTable);

		MonoClass* entityClass = mono_class_from_name(s_Data->CoreAssemblyImage, "OpenGLEngine", "Entity");

		for (int32_t i = 0; i < numTypes; i++)
		{
			uint32_t cols[MONO_TYPEDEF_SIZE];
			mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);

			const char* nameSpace = mono_metadata_string_heap(s_Data->AppAssemblyImage, cols[MONO_TYPEDEF_NAMESPACE]);
			const char* name = mono_metadata_string_heap(s_Data->AppAssemblyImage, cols[MONO_TYPEDEF_NAME]);

			std::string fullName;
			if (strlen(nameSpace) != 0)
				fullName = nameSpace + std::string(".") + name;
			else
				fullName = name;

			MonoClass* monoClass = mono_class_from_name(s_Data->AppAssemblyImage, nameSpace, name);

			if (monoClass == entityClass)
				continue;

			bool isEntity = mono_class_is_subclass_of(monoClass, entityClass, false);
			if (isEntity)
			{
				s_Data->EntityClasses[fullName] = std::make_shared<ScriptClass>(nameSpace, name, false);
				std::cout << "Found entity class: " << fullName << std::endl;
			}
		}
	}

	MonoImage* ScriptEngine::GetCoreAssemblyImage()
	{
		return s_Data->CoreAssemblyImage;
	}

	MonoDomain* ScriptEngine::GetCoreDomain()
	{
		return s_Data->RootDomain;
	}

	void ScriptEngine::SetAppAssemblyPath(const std::filesystem::path& filepath)
	{
		s_Data->AppAssemblyFilepath = filepath;
	}

	void ScriptEngine::InitMono()
	{
		mono_set_assemblies_path("mono/lib");

		MonoDomain* rootDomain = mono_jit_init("OpenGLEngineJITRuntime");
		if (rootDomain == nullptr) return;

		s_Data->RootDomain = rootDomain;
	}

	void ScriptEngine::ShutdownMono()
	{
		mono_domain_set(mono_get_root_domain(), false);

		mono_domain_unload(s_Data->AppDomain);
		s_Data->AppDomain = nullptr;

		mono_jit_cleanup(s_Data->RootDomain);
		s_Data->RootDomain = nullptr;
	}

	MonoObject* ScriptEngine::InstantiateClass(MonoClass* monoClass)
	{
		MonoObject* instance = mono_object_new(s_Data->AppDomain, monoClass);
		mono_runtime_object_init(instance);

		return instance;
	}

	ScriptClass::ScriptClass(const std::string& classNamespace, const std::string& className, bool isCore) : m_ClassNamespace(classNamespace), m_ClassName(className)
	{
		m_MonoClass = mono_class_from_name(isCore ? s_Data->CoreAssemblyImage : s_Data->AppAssemblyImage, classNamespace.c_str(), className.c_str());
	}

	MonoObject* ScriptClass::Instantiate()
	{
		return ScriptEngine::InstantiateClass(m_MonoClass);
	}

	MonoMethod* ScriptClass::GetMethod(const std::string& name, int parameterCount)
	{
		return mono_class_get_method_from_name(m_MonoClass, name.c_str(), parameterCount);
	}

	MonoObject* ScriptClass::InvokeMethod(MonoMethod* method, MonoObject* instance, void** params)
	{
		return mono_runtime_invoke(method, instance, params, nullptr);
	}

	ScriptInstance::ScriptInstance(std::shared_ptr<ScriptClass> scriptClass, Entity entity) : m_ScriptClass(scriptClass)
	{
		m_Instance = scriptClass->Instantiate();

		m_Constructor = s_Data->EntityClass.GetMethod(".ctor", 1);
		m_OnCreateMethod = scriptClass->GetMethod("OnCreate", 0);
		m_OnUpdateMethod = scriptClass->GetMethod("OnUpdate", 1);

		uint64_t entityID = entity.GetUUID();
		void* param = &entityID;
		scriptClass->InvokeMethod(m_Constructor, m_Instance, &param);
	}

	void ScriptInstance::InvokeOnCreate()
	{
		if (m_OnCreateMethod)
			m_ScriptClass->InvokeMethod(m_OnCreateMethod, m_Instance);
	}

	void ScriptInstance::InvokeOnUpdate(float ts)
	{
		if (m_OnUpdateMethod)
		{
			void* param = &ts;
			m_ScriptClass->InvokeMethod(m_OnUpdateMethod, m_Instance, &param);
		}
	}
}