#pragma once

#include <glm/glm.hpp>

namespace DuckEngine
{
	class Transformable {
	protected:
		glm::vec3 m_position;
		glm::vec3 m_rotation;
		glm::vec3 m_scale;

		glm::mat4 m_ModelMatrix;
		const glm::mat4 m_identityMatrix;

		void updateModelMatrix();
	public:
		Transformable();

		const glm::vec3& getPosition() const;
		const glm::vec3& getRotation() const;
		const glm::vec3& getScale() const;

		const glm::mat4& getModelMatrix() const;

		void setPosition(const glm::vec3& position);
		void setRotation(const glm::vec3& rotation);
		void setScale(const glm::vec3& scale);

		void translate(const glm::vec3& translation);
		void rotate(const glm::vec3& rotation);
		void scale(const glm::vec3& scale);
	};
}