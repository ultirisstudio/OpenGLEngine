#pragma once

#include "Component.h"

#include <string>
#include <vector>
#include <array>
#include <bitset>
#include <memory>
#include <unordered_map>

#include <OpenGLEngine/Core/UUID.h>

class Entity
{
public:
	Entity() = default;
	Entity(std::string name, OpenGLEngine::UUID uuid);

	char* GetName() { return m_Name.data(); }

	OpenGLEngine::UUID GetUUID() { return m_UUID; }

	void SetName(std::string name) { m_Name = name; }

	template<typename T>
	bool HasComponent() const {
		return m_ComponentsBitset[GetComponentTypeID<T>()];
	}

	template<typename T, typename... TArgs>
	T& AddComponent(TArgs&&... mArgs) {
		T* component(new T(std::forward <TArgs>(mArgs)...));
		component->entity = this;

		m_ComponentsArray[GetComponentTypeID<T>()] = component;
		m_ComponentsBitset[GetComponentTypeID<T>()] = true;

		return *component;
	}

	template<typename T>
	void RemoveComponent() {
		delete m_ComponentsArray[GetComponentTypeID<T>()];
		m_ComponentsArray[GetComponentTypeID<T>()] = nullptr;
		m_ComponentsBitset[GetComponentTypeID<T>()] = false;
	}

	template<typename T>
	T& GetComponent() const {
		auto pointer(m_ComponentsArray[GetComponentTypeID<T>()]);
		return *static_cast<T*>(pointer);
	}

	void AddChild(const OpenGLEngine::UUID& id);
public:
	std::string m_Name;
	OpenGLEngine::UUID m_UUID;

	std::vector<OpenGLEngine::UUID> m_Children;
	OpenGLEngine::UUID m_Parent = OpenGLEngine::UUID::Null();

private:
	std::bitset<MAX_COMPONENTS> m_ComponentsBitset;
	std::array<Component*, MAX_COMPONENTS> m_ComponentsArray;
};

struct EntityMapping
{
	size_t operator()(const Entity& k)const
	{
		return std::hash<int>()(k.m_UUID);
	}

	bool operator()(const Entity& a, const Entity& b)const
	{
		return a.m_UUID == b.m_UUID;
	}
};