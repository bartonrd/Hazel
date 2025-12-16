#pragma once

#include "../Core.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Hazel {

	class HAZEL_API Camera
	{
	public:
		enum class ProjectionType { Perspective = 0, Orthographic = 1 };

		Camera(ProjectionType type = ProjectionType::Perspective);

		void SetPerspective(float fov, float aspectRatio, float nearClip, float farClip);
		void SetOrthographic(float size, float aspectRatio, float nearClip, float farClip);

		void SetPosition(const glm::vec3& position);
		void SetRotation(const glm::vec3& rotation);

		const glm::vec3& GetPosition() const { return m_Position; }
		const glm::vec3& GetRotation() const { return m_Rotation; }

		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		glm::mat4 GetViewProjectionMatrix() const { return m_ProjectionMatrix * m_ViewMatrix; }

		ProjectionType GetProjectionType() const { return m_ProjectionType; }

	private:
		void RecalculateViewMatrix();

	private:
		ProjectionType m_ProjectionType;
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;

		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_Rotation = { 0.0f, 0.0f, 0.0f };
	};

}
