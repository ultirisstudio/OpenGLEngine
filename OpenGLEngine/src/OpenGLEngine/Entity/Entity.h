#pragma once

#include "Component.h"

#include <string>
#include <vector>
#include <array>
#include <bitset>
#include <unordered_map>

class Component;

class Entity
{
public:
	Entity() = default;
	Entity(std::string name, std::string uuid);

	char* GetName() { return m_Name.data(); }
	char* GetId() { return m_Id.data(); }

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
private:
	std::string m_Name;
	std::string m_Id;

	std::bitset<MAX_COMPONENTS> m_ComponentsBitset;
	std::array<Component*, MAX_COMPONENTS> m_ComponentsArray;
};