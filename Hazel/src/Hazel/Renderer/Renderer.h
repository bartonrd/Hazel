#pragma once

#include "../Core.h"
#include "Camera.h"
#include "Shader.h"
#include "Buffer.h"
#include "Material.h"
#include "Light.h"
#include <glm/glm.hpp>
#include <memory>

namespace Hazel {

	class HAZEL_API Renderer
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const Camera& camera);
		static void EndScene();

		static void Submit(const std::shared_ptr<VertexArray>& vertexArray, 
		                   const std::shared_ptr<Material>& material,
		                   const glm::mat4& transform = glm::mat4(1.0f));

		static void AddLight(const std::shared_ptr<Light>& light);
		static void ClearLights();

		static void SetClearColor(const glm::vec4& color);
		static void Clear();

	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
			std::vector<std::shared_ptr<Light>> Lights;
		};

		static SceneData* s_SceneData;
	};

}
