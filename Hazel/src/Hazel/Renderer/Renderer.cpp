#include "Renderer.h"
#include "../Log.h"
#include <imgui_impl_opengl3_loader.h>

namespace Hazel {

	Renderer::SceneData* Renderer::s_SceneData = nullptr;
	static bool s_RendererInitialized = false;

	void Renderer::Init()
	{
		if (s_RendererInitialized)
			return;

		// Initialize OpenGL loader
		if (imgl3wInit() != 0)
		{
			HZ_ERROR("Failed to initialize OpenGL loader");
			return;
		}

		s_SceneData = new SceneData();
		s_RendererInitialized = true;
		
		HZ_INFO("Renderer initialized");
	}

	void Renderer::Shutdown()
	{
		delete s_SceneData;
		s_SceneData = nullptr;
		s_RendererInitialized = false;
	}

	void Renderer::BeginScene(const Camera& camera)
	{
		// Lazy initialization
		if (!s_RendererInitialized)
			Init();

		s_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
		
		// Set up OpenGL state for 3D rendering (done each frame)
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::Submit(const std::shared_ptr<VertexArray>& vertexArray,
	                      const std::shared_ptr<Material>& material,
	                      const glm::mat4& transform)
	{
		material->Bind();

		auto shader = material->GetShader();
		shader->SetMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
		shader->SetMat4("u_Transform", transform);

		// Upload lights to shader
		int dirLightCount = 0;
		int pointLightCount = 0;
		int spotLightCount = 0;

		for (const auto& light : s_SceneData->Lights)
		{
			switch (light->GetType())
			{
				case LightType::Directional:
					if (dirLightCount < 4) // Max 4 directional lights
					{
						light->UploadToShader(shader, dirLightCount);
						dirLightCount++;
					}
					break;
				case LightType::Point:
					if (pointLightCount < 4) // Max 4 point lights
					{
						light->UploadToShader(shader, pointLightCount);
						pointLightCount++;
					}
					break;
				case LightType::Spot:
					if (spotLightCount < 4) // Max 4 spot lights
					{
						light->UploadToShader(shader, spotLightCount);
						spotLightCount++;
					}
					break;
			}
		}

		shader->SetInt("u_DirectionalLightCount", dirLightCount);
		shader->SetInt("u_PointLightCount", pointLightCount);
		shader->SetInt("u_SpotLightCount", spotLightCount);

		vertexArray->Bind();
		glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
	}

	void Renderer::AddLight(const std::shared_ptr<Light>& light)
	{
		s_SceneData->Lights.push_back(light);
	}

	void Renderer::ClearLights()
	{
		s_SceneData->Lights.clear();
	}

	void Renderer::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void Renderer::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

}
