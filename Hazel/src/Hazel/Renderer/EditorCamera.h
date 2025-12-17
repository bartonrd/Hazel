#pragma once

#include "Camera.h"
#include <glm/glm.hpp>

namespace Hazel {

#ifdef _MSC_VER
	#pragma warning(push)
	#pragma warning(disable: 4251) // Disable warning about DLL interface for private members
#endif

	class HAZEL_API EditorCamera : public Camera
	{
	public:
		EditorCamera();

		void OnUpdate(float deltaTime);

		void ProcessMouseMovement(float xOffset, float yOffset, bool constrainPitch = true);
		void ProcessMouseScroll(float yOffset);

		void SetViewportSize(float width, float height);

		// Movement flags (set by keyboard input)
		void SetMoveForward(bool moving) { m_MoveForward = moving; }
		void SetMoveBackward(bool moving) { m_MoveBackward = moving; }
		void SetMoveLeft(bool moving) { m_MoveLeft = moving; }
		void SetMoveRight(bool moving) { m_MoveRight = moving; }
		void SetMoveUp(bool moving) { m_MoveUp = moving; }
		void SetMoveDown(bool moving) { m_MoveDown = moving; }

		float GetYaw() const { return m_Yaw; }
		float GetPitch() const { return m_Pitch; }

		void SetYaw(float yaw) { m_Yaw = yaw; UpdateCameraVectors(); }
		void SetPitch(float pitch) { m_Pitch = pitch; UpdateCameraVectors(); }

	private:
		void UpdateCameraVectors();

	private:
		// Camera vectors
		glm::vec3 m_Front = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 m_Up = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 m_Right = glm::vec3(1.0f, 0.0f, 0.0f);
		glm::vec3 m_WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);

		// Euler angles
		float m_Yaw = -90.0f;
		float m_Pitch = 0.0f;

		// Camera options
		float m_MovementSpeed = 5.0f;
		float m_MouseSensitivity = 0.1f;
		float m_ZoomLevel = 45.0f;

		// Movement flags
		bool m_MoveForward = false;
		bool m_MoveBackward = false;
		bool m_MoveLeft = false;
		bool m_MoveRight = false;
		bool m_MoveUp = false;
		bool m_MoveDown = false;
	};

#ifdef _MSC_VER
	#pragma warning(pop)
#endif

}
