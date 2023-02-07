#pragma once

#include "Component.h"

#include <string>
#include <vector>
#include <array>
#include <bitset>

class Component;

namespace DuckEngine
{
	class Entity
	{
	public:
		char* GetName() { return m_Name.data(); }
		unsigned int GetId() { return m_Id; }

		void SetName(std::string name) { m_Name = name; }
		void SetId(unsigned int id) { m_Id = id; }

		void Draw();

		template<typename T>
		bool HasComponent() const {
			return m_ComponentsBitset[GetComponentTypeID<T>()];
		}

		template<typename T, typename... TArgs>
		T& AddComponent(TArgs&&... mArgs) {
			T* component(new T(std::forward <TArgs>(mArgs)...));

			//std::shared_ptr<Component> uPtr { component };

			//m_Components.emplace_back(std::move(uPtr));
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
		unsigned int m_Id;

		//std::vector<std::shared_ptr<Component>> m_Components;
		std::bitset<MAX_COMPONENTS> m_ComponentsBitset;
		std::array<Component*, MAX_COMPONENTS> m_ComponentsArray;
	};
}