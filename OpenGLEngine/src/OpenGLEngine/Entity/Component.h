#pragma once

#include <OpenGLEngine/Resources/Model.h>
#include <OpenGLEngine/Resources/Materials/Material.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

#define MAX_COMPONENTS 12

class Entity;

inline size_t GetComponentTypeID() {
	static size_t lastID = 0;
	return lastID++;
}

template<typename T>
inline size_t GetComponentTypeID() noexcept {
	static size_t typeID = GetComponentTypeID();
	return typeID;
}

class Component
{
public:
	Entity* entity;

	Component() : entity(nullptr) {}
	virtual ~Component() {}
};