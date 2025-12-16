#include "Material.h"

namespace Hazel {

	Material::Material(const std::shared_ptr<Shader>& shader)
		: m_Shader(shader)
	{
	}

	void Material::Bind()
	{
		m_Shader->Bind();
		m_Shader->SetFloat4("u_Material.color", m_Color);
		m_Shader->SetFloat("u_Material.shininess", m_Shininess);
		m_Shader->SetFloat("u_Material.metallic", m_Metallic);
		m_Shader->SetFloat("u_Material.roughness", m_Roughness);
	}

}
