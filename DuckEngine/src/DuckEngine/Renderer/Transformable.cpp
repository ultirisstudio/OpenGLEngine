#include "depch.h"
#include "DuckEngine/Renderer/Transformable.h"
#include <glm/gtc/matrix_transform.hpp>

namespace DuckEngine
{
	void Transformable::updateModelMatrix()
	{
		m_ModelMatrix = glm::translate(m_identityMatrix, m_position);

		m_ModelMatrix = glm::rotate(m_ModelMatrix, glm::radians(m_rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		m_ModelMatrix = glm::rotate(m_ModelMatrix, glm::radians(m_rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		m_ModelMatrix = glm::rotate(m_ModelMatrix, glm::radians(m_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

		m_ModelMatrix = glm::scale(m_ModelMatrix, m_scale);
	}

	Transformable::Transformable() : m_position(0.0f), m_rotation(0.0f), m_scale(1.0f), m_ModelMatrix(1.0f), m_identityMatrix(1.0f)
	{

	}

	const glm::vec3& Transformable::getPosition() const {
		return m_position;
	}

	const glm::vec3& Transformable::getRotation() const {
		return m_rotation;
	}

	const glm::vec3& Transformable::getScale() const {
		return m_scale;
	}

	const glm::mat4& Transformable::getModelMatrix() const {
		return m_ModelMatrix;
	}

	void Transformable::setPosition(const glm::vec3& position)
	{
		m_position = position;
		updateModelMatrix();
	}

	void Transformable::setRotation(const glm::vec3& rotation)
	{
		m_rotation = rotation;
		updateModelMatrix();
	}

	void Transformable::setScale(const glm::vec3& scale)
	{
		m_scale = scale;
		updateModelMatrix();
	}

	void Transformable::translate(const glm::vec3& translation)
	{
		m_position += translation;
		updateModelMatrix();
	}

	void Transformable::rotate(const glm::vec3& rotation)
	{
		m_rotation += rotation;
		updateModelMatrix();
	}

	void Transformable::scale(const glm::vec3& scale)
	{
		m_scale += scale;
		updateModelMatrix();
	}
}