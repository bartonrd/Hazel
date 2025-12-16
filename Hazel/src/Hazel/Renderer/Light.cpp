#include "Light.h"
#include <glm/gtc/type_ptr.hpp>

namespace Hazel {

	Light::Light(LightType type)
		: m_Type(type)
	{
	}

	/////////////////////////////////////////////////////////////////////////////
	// DirectionalLight /////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////

	DirectionalLight::DirectionalLight()
		: Light(LightType::Directional)
	{
	}

	void DirectionalLight::UploadToShader(const std::shared_ptr<Shader>& shader, int index)
	{
		std::string prefix = "u_DirectionalLights[" + std::to_string(index) + "]";
		shader->SetFloat3(prefix + ".direction", m_Direction);
		shader->SetFloat3(prefix + ".color", m_Color);
		shader->SetFloat(prefix + ".intensity", m_Intensity);
	}

	/////////////////////////////////////////////////////////////////////////////
	// PointLight ///////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////

	PointLight::PointLight()
		: Light(LightType::Point)
	{
	}

	void PointLight::UploadToShader(const std::shared_ptr<Shader>& shader, int index)
	{
		std::string prefix = "u_PointLights[" + std::to_string(index) + "]";
		shader->SetFloat3(prefix + ".position", m_Position);
		shader->SetFloat3(prefix + ".color", m_Color);
		shader->SetFloat(prefix + ".intensity", m_Intensity);
		shader->SetFloat(prefix + ".constant", m_Constant);
		shader->SetFloat(prefix + ".linear", m_Linear);
		shader->SetFloat(prefix + ".quadratic", m_Quadratic);
	}

	/////////////////////////////////////////////////////////////////////////////
	// SpotLight ////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////

	SpotLight::SpotLight()
		: Light(LightType::Spot)
	{
	}

	void SpotLight::UploadToShader(const std::shared_ptr<Shader>& shader, int index)
	{
		std::string prefix = "u_SpotLights[" + std::to_string(index) + "]";
		shader->SetFloat3(prefix + ".position", m_Position);
		shader->SetFloat3(prefix + ".direction", m_Direction);
		shader->SetFloat3(prefix + ".color", m_Color);
		shader->SetFloat(prefix + ".intensity", m_Intensity);
		shader->SetFloat(prefix + ".innerCutOff", glm::cos(glm::radians(m_InnerCutOff)));
		shader->SetFloat(prefix + ".outerCutOff", glm::cos(glm::radians(m_OuterCutOff)));
	}

}
