using System;

namespace Hazel
{
    /// <summary>
    /// Example player controller script demonstrating C# scripting in Hazel
    /// </summary>
    public class PlayerController
    {
        // Public fields are exposed in the Inspector
        public float Speed = 5.0f;
        public float JumpForce = 10.0f;
        public float Health = 100.0f;

        private float velocity = 0.0f;

        /// <summary>
        /// Called when the script is first created
        /// </summary>
        public void OnCreate()
        {
            Console.WriteLine("PlayerController: OnCreate called");
            Console.WriteLine($"Player initialized with Speed={Speed}, JumpForce={JumpForce}");
        }

        /// <summary>
        /// Called every frame with the time since last frame
        /// </summary>
        /// <param name="deltaTime">Time in seconds since last frame</param>
        public void OnUpdate(float deltaTime)
        {
            // Example: Simple movement logic
            // In a real implementation, this would access Input system and Transform component
            
            // Simulate gravity
            velocity -= 9.81f * deltaTime;
            
            // Check if on ground (simplified)
            if (IsOnGround())
            {
                velocity = 0.0f;
            }
        }

        /// <summary>
        /// Called when the script is destroyed
        /// </summary>
        public void OnDestroy()
        {
            Console.WriteLine("PlayerController: OnDestroy called");
            Console.WriteLine("Cleaning up player resources...");
        }

        /// <summary>
        /// Example method that could be called from C++ or other scripts
        /// </summary>
        public void TakeDamage(float damage)
        {
            Health -= damage;
            Console.WriteLine($"Player took {damage} damage! Health: {Health}");
            
            if (Health <= 0)
            {
                Die();
            }
        }

        /// <summary>
        /// Example method for jumping
        /// </summary>
        public void Jump()
        {
            if (IsOnGround())
            {
                velocity = JumpForce;
                Console.WriteLine($"Player jumped with force {JumpForce}");
            }
        }

        private bool IsOnGround()
        {
            // In real implementation, this would do a raycast or collision check
            // For now, just a placeholder
            return velocity <= 0.0f;
        }

        private void Die()
        {
            Console.WriteLine("Player died!");
            // In real implementation, trigger death animation, respawn, etc.
        }
    }
}
