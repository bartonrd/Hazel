#pragma once

#include "../Core.h"
#include <glm/glm.hpp>
#include <vector>

namespace Hazel {

	struct MeshData
	{
		std::vector<float> Vertices;
		std::vector<unsigned int> Indices;
	};

	class HAZEL_API MeshGenerator
	{
	public:
		// Generate cube mesh with positions and normals
		// Returns vertices in format: [position.x, position.y, position.z, normal.x, normal.y, normal.z, ...]
		static MeshData CreateCube(float size = 1.0f);

		// Generate sphere mesh with positions and normals
		// segments: number of horizontal and vertical segments (higher = smoother)
		static MeshData CreateSphere(float radius = 0.5f, int segments = 32);

		// Generate capsule mesh with positions and normals
		// height: height of the cylindrical portion
		// radius: radius of the cylinder and hemispheres
		static MeshData CreateCapsule(float height = 1.0f, float radius = 0.5f, int segments = 32);
	};

}
