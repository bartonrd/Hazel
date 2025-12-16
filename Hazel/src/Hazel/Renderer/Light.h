#pragma once

#include "../Core.h"
#include "Shader.h"
#include <glm/glm.hpp>
#include <memory>

namespace Hazel {

	enum class LightType
	{
		Directional = 0,
		Point = 1,
		Spot = 2
	};

	class HAZEL_API Light
	{
	public:
		Light(LightType type);
		virtual ~Light() = default;

		void SetColor(const glm::vec3& color) { m_Color = color; }
		void SetIntensity(float intensity) { m_Intensity = intensity; }

		const glm::vec3& GetColor() const { return m_Color; }
		float GetIntensity() const { return m_Intensity; }
		LightType GetType() const { return m_Type; }

		virtual void UploadToShader(const std::shared_ptr<Shader>& shader, int index) = 0;

	protected:
		LightType m_Type;
		glm::vec3 m_Color = { 1.0f, 1.0f, 1.0f };
		float m_Intensity = 1.0f;
	};

	class HAZEL_API DirectionalLight : public Light
	{
	public:
		DirectionalLight();

		void SetDirection(const glm::vec3& direction) { m_Direction = glm::normalize(direction); }
		const glm::vec3& GetDirection() const { return m_Direction; }

		virtual void UploadToShader(const std::shared_ptr<Shader>& shader, int index) override;

	private:
		glm::vec3 m_Direction = { 0.0f, -1.0f, 0.0f };
	};

	class HAZEL_API PointLight : public Light
	{
	public:
		PointLight();

		void SetPosition(const glm::vec3& position) { m_Position = position; }
		void SetAttenuation(float constant, float linear, float quadratic)
		{
			m_Constant = constant;
			m_Linear = linear;
			m_Quadratic = quadratic;
		}

		const glm::vec3& GetPosition() const { return m_Position; }

		virtual void UploadToShader(const std::shared_ptr<Shader>& shader, int index) override;

	private:
		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
		float m_Constant = 1.0f;
		float m_Linear = 0.09f;
		float m_Quadratic = 0.032f;
	};

	class HAZEL_API SpotLight : public Light
	{
	public:
		SpotLight();

		void SetPosition(const glm::vec3& position) { m_Position = position; }
		void SetDirection(const glm::vec3& direction) { m_Direction = glm::normalize(direction); }
		void SetCutOff(float innerCutOff, float outerCutOff)
		{
			m_InnerCutOff = innerCutOff;
			m_OuterCutOff = outerCutOff;
		}

		const glm::vec3& GetPosition() const { return m_Position; }
		const glm::vec3& GetDirection() const { return m_Direction; }

		virtual void UploadToShader(const std::shared_ptr<Shader>& shader, int index) override;

	private:
		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_Direction = { 0.0f, -1.0f, 0.0f };
		float m_InnerCutOff = 12.5f;
		float m_OuterCutOff = 17.5f;
	};

}
