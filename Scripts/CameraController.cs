using System;

namespace Hazel
{
    /// <summary>
    /// Example camera controller script
    /// </summary>
    public class CameraController
    {
        // Camera settings
        public float MouseSensitivity = 2.0f;
        public float MoveSpeed = 10.0f;
        public float SprintMultiplier = 2.0f;

        private float pitch = 0.0f;
        private float yaw = 0.0f;

        public void OnCreate()
        {
            Console.WriteLine("CameraController: Initialized");
        }

        public void OnUpdate(float deltaTime)
        {
            // In a real implementation, this would:
            // 1. Get mouse input for camera rotation
            // 2. Get keyboard input for camera movement
            // 3. Update the camera transform
        }

        public void OnDestroy()
        {
            Console.WriteLine("CameraController: Destroyed");
        }

        /// <summary>
        /// Rotates the camera based on mouse input
        /// </summary>
        public void Rotate(float deltaX, float deltaY)
        {
            yaw += deltaX * MouseSensitivity;
            pitch -= deltaY * MouseSensitivity;

            // Clamp pitch to avoid camera flipping
            if (pitch > 89.0f) pitch = 89.0f;
            if (pitch < -89.0f) pitch = -89.0f;

            Console.WriteLine($"Camera rotated - Yaw: {yaw}, Pitch: {pitch}");
        }
    }
}
