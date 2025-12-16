#pragma once

#include "../Core.h"
#include "Shader.h"
#include <glm/glm.hpp>
#include <memory>

namespace Hazel {

#ifdef _MSC_VER
	#pragma warning(push)
	#pragma warning(disable: 4251) // Disable warning about DLL interface for private members
#endif

	class HAZEL_API Material
	{
	public:
		Material(const std::shared_ptr<Shader>& shader);

		void Bind();
		
		// Set material properties
		void SetColor(const glm::vec4& color) { m_Color = color; }
		void SetShininess(float shininess) { m_Shininess = shininess; }
		void SetMetallic(float metallic) { m_Metallic = metallic; }
		void SetRoughness(float roughness) { m_Roughness = roughness; }

		// Get material properties
		const glm::vec4& GetColor() const { return m_Color; }
		float GetShininess() const { return m_Shininess; }
		float GetMetallic() const { return m_Metallic; }
		float GetRoughness() const { return m_Roughness; }

		std::shared_ptr<Shader> GetShader() const { return m_Shader; }

	private:
		std::shared_ptr<Shader> m_Shader;
		
		// Material properties
		glm::vec4 m_Color = { 1.0f, 1.0f, 1.0f, 1.0f };
		float m_Shininess = 32.0f;
		float m_Metallic = 0.0f;
		float m_Roughness = 0.5f;
	};

#ifdef _MSC_VER
	#pragma warning(pop)
#endif

}
