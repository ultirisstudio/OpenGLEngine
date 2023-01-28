#include "depch.h"
#include <DuckEngine/Shader/ShaderGenerator.h>
#include <DuckEngine/tools/Log.h>
#include <sstream>

DuckEngine::ShaderGenerator::ShaderGenerator() :
	m_shaders()
{

}

DuckEngine::ShaderGenerator::~ShaderGenerator()
{
	std::vector<std::string> ids;
	ids.reserve(m_shaders.size());

	for (std::map<std::string, Shader*>::iterator it = m_shaders.begin(); it != m_shaders.end(); it++)
		ids.push_back(it->first);

	for (std::string id : ids)
		freeShader(id);
}

DuckEngine::Shader* DuckEngine::ShaderGenerator::generateShader(Material& material)
{
	std::ostringstream vsStream;
	std::ostringstream fsStream;
	std::ostringstream gsStream;

	vsStream
		<< "#version 330 core\n"
		<< "\n"
		<< "layout(location = 0) in vec3 vPosition;\n"
		<< "layout(location = 1) in vec3 vNormal;\n"
		<< "\n"
		<< "out VS_OUT\n"
		<< "{\n"
		<< "\tvec3 fPosition;\n"
		<< "\tvec3 fNormal;\n"
		<< "} vs_out;\n"
		<< "\n"
		<< "uniform mat4 uModel;\n"
		<< "uniform mat4 uView;\n"
		<< "uniform mat4 uProjection;\n"
		<< "\n"
		<< ""
		<< "void main()\n"
		<< "{\n"
		<< "\tvec4 worldPosition = uModel * vec4(vPosition, 1.0f);\n"
		<< "\n"
		<< "\tvs_out.fPosition = vec3(worldPosition);\n"
		<< "\tvs_out.fNormal = mat3(transpose(inverse(uModel))) * vNormal;\n"
		<< "\n"
		<< "\tgl_Position = uProjection * uView * worldPosition;\n"
		<< "}\n";

	fsStream
		<< "#version 330 core\n"
		<< "\n"
		<< "struct Material\n"
		<< "{\n"
		<< "\tvec3 ambient;\n"
		<< "\tvec3 diffuse;\n"
		<< "\tvec3 specular;\n"
		<< "\tfloat shininess;\n"
		<< "};\n"
		<< "\n"
		<< "in VS_OUT\n"
		<< "{\n"
		<< "\tvec3 fPosition;\n"
		<< "\tvec3 fNormal;\n"
		<< "} fs_in;\n"
		<< "\n"
		<< "out vec4 color;\n"
		<< "\n"
		<< "uniform Material uMaterial;\n"
		<< "\n"
		<< "void main()\n"
		<< "{\n"
		<< "\tcolor = vec4(1.0f);\n"
		<< "}\n";

	std::string vs = vsStream.str();
	std::string fs = fsStream.str();
	std::string gs = gsStream.str();

	Shader* shader = new Shader(vs, fs, gs);
	//m_shaders[id] = shader;

	//Log::debug("Shader '" + id + "' generated");

	return shader;
}

void DuckEngine::ShaderGenerator::generateShader(const std::string& id, Material& material)
{
	if (m_shaders.find(id) != m_shaders.end())
	{
		Log::error("Shader '" + id + "' already exists");
		return;
	}

	std::ostringstream vsStream;
	std::ostringstream fsStream;
	std::ostringstream gsStream;

	vsStream
		<< "#version 330 core\n"
		<< "\n"
		<< "layout(location = 0) in vec3 vPosition;\n"
		<< "layout(location = 1) in vec3 vNormal;\n"
		<< "\n"
		<< "out VS_OUT\n"
		<< "{\n"
		<< "\tvec3 fPosition;\n"
		<< "\tvec3 fNormal;\n"
		<< "} vs_out;\n"
		<< "\n"
		<< "uniform mat4 uModel;\n"
		<< "uniform mat4 uView;\n"
		<< "uniform mat4 uProjection;\n"
		<< "\n"
		<< ""
		<< "void main()\n"
		<< "{\n"
		<< "\tvec4 worldPosition = uModel * vec4(vPosition, 1.0f);\n"
		<< "\n"
		<< "\tvs_out.fPosition = vec3(worldPosition);\n"
		<< "\tvs_out.fNormal = mat3(transpose(inverse(uModel))) * vNormal;\n"
		<< "\n"
		<< "\tgl_Position = uProjection * uView * worldPosition;\n"
		<< "}\n";

	fsStream
		<< "#version 330 core\n"
		<< "\n"
		<< "struct Material\n"
		<< "{\n"
		<< "\tvec3 ambient;\n"
		<< "\tvec3 diffuse;\n"
		<< "\tvec3 specular;\n"
		<< "\tfloat shininess;\n"
		<< "};\n"
		<< "\n"
		<< "in VS_OUT\n"
		<< "{\n"
		<< "\tvec3 fPosition;\n"
		<< "\tvec3 fNormal;\n"
		<< "} fs_in;\n"
		<< "\n"
		<< "out vec4 color;\n"
		<< "\n"
		<< "uniform Material uMaterial;\n"
		<< "\n"
		<< "void main()\n"
		<< "{\n"
		<< "\tcolor = vec4(1.0f);\n"
		<< "}\n";

	std::string vs = vsStream.str();
	std::string fs = fsStream.str();
	std::string gs = gsStream.str();

	Shader* shader = new Shader(vs, fs, gs);
	m_shaders[id] = shader;

	Log::debug("Shader '" + id + "' generated");
}

void DuckEngine::ShaderGenerator::freeShader(const std::string& id)
{
	if (m_shaders.find(id) == m_shaders.end())
	{
		Log::error("Shader '" + id + "' doesn't exist");
		return;
	}

	if (m_shaders.at(id))
		delete m_shaders.at(id);

	m_shaders.erase(id);

	Log::debug("Shader '" + id + "' freed");
}

DuckEngine::Shader& DuckEngine::ShaderGenerator::getShader(const std::string& id)
{
	return *m_shaders.at(id);
}
