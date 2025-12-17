#include "EditorCamera.h"
#include <glm/gtc/matrix_transform.hpp>

namespace Hazel {

	EditorCamera::EditorCamera()
		: Camera(ProjectionType::Perspective)
	{
		SetPerspective(45.0f, 16.0f / 9.0f, 0.1f, 1000.0f);
		SetPosition(glm::vec3(0.0f, 5.0f, 10.0f));
		UpdateCameraVectors();
	}

	void EditorCamera::OnUpdate(float deltaTime)
	{
		// Calculate movement
		glm::vec3 position = GetPosition();
		float velocity = m_MovementSpeed * deltaTime;

		if (m_MoveForward)
			position += m_Front * velocity;
		if (m_MoveBackward)
			position -= m_Front * velocity;
		if (m_MoveLeft)
			position -= m_Right * velocity;
		if (m_MoveRight)
			position += m_Right * velocity;
		if (m_MoveUp)
			position += m_WorldUp * velocity;
		if (m_MoveDown)
			position -= m_WorldUp * velocity;

		SetPosition(position);
	}

	void EditorCamera::ProcessMouseMovement(float xOffset, float yOffset, bool constrainPitch)
	{
		xOffset *= m_MouseSensitivity;
		yOffset *= m_MouseSensitivity;

		m_Yaw += xOffset;
		m_Pitch += yOffset;

		// Constrain pitch to prevent screen flip
		if (constrainPitch)
		{
			m_Pitch = glm::clamp(m_Pitch, -89.0f, 89.0f);
		}

		// Update camera vectors using the updated Euler angles
		UpdateCameraVectors();
	}

	void EditorCamera::ProcessMouseScroll(float yOffset)
	{
		m_ZoomLevel -= yOffset;
		m_ZoomLevel = glm::clamp(m_ZoomLevel, 1.0f, 90.0f);
		
		// Update perspective with new FOV
		SetPerspective(m_ZoomLevel, 16.0f / 9.0f, 0.1f, 1000.0f);
	}

	void EditorCamera::SetViewportSize(float width, float height)
	{
		float aspectRatio = width / height;
		SetPerspective(m_ZoomLevel, aspectRatio, 0.1f, 1000.0f);
	}

	void EditorCamera::UpdateCameraVectors()
	{
		// Calculate the new Front vector
		glm::vec3 front;
		front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
		front.y = sin(glm::radians(m_Pitch));
		front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
		m_Front = glm::normalize(front);

		// Recalculate the Right and Up vector
		m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));
		m_Up = glm::normalize(glm::cross(m_Right, m_Front));

		// Recalculate the view matrix with updated vectors
		RecalculateViewMatrix();
	}

	void EditorCamera::RecalculateViewMatrix()
	{
		// Use lookAt for FPS-style camera
		glm::vec3 position = GetPosition();
		SetViewMatrix(glm::lookAt(position, position + m_Front, m_Up));
	}

}
