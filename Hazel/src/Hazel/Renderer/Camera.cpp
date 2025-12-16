#include "Camera.h"
#include "../Log.h"
#include <glm/gtc/quaternion.hpp>

namespace Hazel {

	Camera::Camera(ProjectionType type)
		: m_ProjectionType(type)
	{
		m_ProjectionMatrix = glm::mat4(1.0f);
		m_ViewMatrix = glm::mat4(1.0f);
	}

	void Camera::SetPerspective(float fov, float aspectRatio, float nearClip, float farClip)
	{
		m_ProjectionType = ProjectionType::Perspective;
		m_ProjectionMatrix = glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip);
	}

	void Camera::SetOrthographic(float size, float aspectRatio, float nearClip, float farClip)
	{
		m_ProjectionType = ProjectionType::Orthographic;
		m_ProjectionMatrix = glm::ortho(-size * aspectRatio, size * aspectRatio, -size, size, nearClip, farClip);
	}

	void Camera::SetPosition(const glm::vec3& position)
	{
		m_Position = position;
		RecalculateViewMatrix();
	}

	void Camera::SetRotation(const glm::vec3& rotation)
	{
		m_Rotation = rotation;
		RecalculateViewMatrix();
	}

	void Camera::RecalculateViewMatrix()
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position);
		transform = glm::rotate(transform, glm::radians(m_Rotation.x), glm::vec3(1, 0, 0));
		transform = glm::rotate(transform, glm::radians(m_Rotation.y), glm::vec3(0, 1, 0));
		transform = glm::rotate(transform, glm::radians(m_Rotation.z), glm::vec3(0, 0, 1));

		m_ViewMatrix = glm::inverse(transform);
	}

}
