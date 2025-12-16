#include "MeshGenerator.h"
#include <cmath>

namespace Hazel {

	// Use a portable definition of PI
	constexpr double PI = 3.14159265358979323846;

	MeshData MeshGenerator::CreateCube(float size)
	{
		MeshData meshData;
		float halfSize = size * 0.5f;

		// Cube vertices with positions and normals
		float vertices[] = {
			// Front face (Z+)
			-halfSize, -halfSize,  halfSize,  0.0f,  0.0f,  1.0f,
			 halfSize, -halfSize,  halfSize,  0.0f,  0.0f,  1.0f,
			 halfSize,  halfSize,  halfSize,  0.0f,  0.0f,  1.0f,
			-halfSize,  halfSize,  halfSize,  0.0f,  0.0f,  1.0f,

			// Back face (Z-)
			-halfSize, -halfSize, -halfSize,  0.0f,  0.0f, -1.0f,
			 halfSize, -halfSize, -halfSize,  0.0f,  0.0f, -1.0f,
			 halfSize,  halfSize, -halfSize,  0.0f,  0.0f, -1.0f,
			-halfSize,  halfSize, -halfSize,  0.0f,  0.0f, -1.0f,

			// Left face (X-)
			-halfSize,  halfSize,  halfSize, -1.0f,  0.0f,  0.0f,
			-halfSize,  halfSize, -halfSize, -1.0f,  0.0f,  0.0f,
			-halfSize, -halfSize, -halfSize, -1.0f,  0.0f,  0.0f,
			-halfSize, -halfSize,  halfSize, -1.0f,  0.0f,  0.0f,

			// Right face (X+)
			 halfSize,  halfSize,  halfSize,  1.0f,  0.0f,  0.0f,
			 halfSize,  halfSize, -halfSize,  1.0f,  0.0f,  0.0f,
			 halfSize, -halfSize, -halfSize,  1.0f,  0.0f,  0.0f,
			 halfSize, -halfSize,  halfSize,  1.0f,  0.0f,  0.0f,

			// Bottom face (Y-)
			-halfSize, -halfSize, -halfSize,  0.0f, -1.0f,  0.0f,
			 halfSize, -halfSize, -halfSize,  0.0f, -1.0f,  0.0f,
			 halfSize, -halfSize,  halfSize,  0.0f, -1.0f,  0.0f,
			-halfSize, -halfSize,  halfSize,  0.0f, -1.0f,  0.0f,

			// Top face (Y+)
			-halfSize,  halfSize, -halfSize,  0.0f,  1.0f,  0.0f,
			 halfSize,  halfSize, -halfSize,  0.0f,  1.0f,  0.0f,
			 halfSize,  halfSize,  halfSize,  0.0f,  1.0f,  0.0f,
			-halfSize,  halfSize,  halfSize,  0.0f,  1.0f,  0.0f,
		};

		unsigned int indices[] = {
			0, 1, 2, 2, 3, 0,       // front
			4, 5, 6, 6, 7, 4,       // back
			8, 9, 10, 10, 11, 8,    // left
			12, 13, 14, 14, 15, 12, // right
			16, 17, 18, 18, 19, 16, // bottom
			20, 21, 22, 22, 23, 20  // top
		};

		meshData.Vertices.assign(vertices, vertices + sizeof(vertices) / sizeof(float));
		meshData.Indices.assign(indices, indices + sizeof(indices) / sizeof(unsigned int));

		return meshData;
	}

	MeshData MeshGenerator::CreateSphere(float radius, int segments)
	{
		MeshData meshData;

		// Generate sphere using UV sphere method
		for (int lat = 0; lat <= segments; lat++)
		{
			float theta = lat * PI / segments;
			float sinTheta = sin(theta);
			float cosTheta = cos(theta);

			for (int lon = 0; lon <= segments; lon++)
			{
				float phi = lon * 2.0f * PI / segments;
				float sinPhi = sin(phi);
				float cosPhi = cos(phi);

				float x = cosPhi * sinTheta;
				float y = cosTheta;
				float z = sinPhi * sinTheta;

				// Position
				meshData.Vertices.push_back(x * radius);
				meshData.Vertices.push_back(y * radius);
				meshData.Vertices.push_back(z * radius);

				// Normal (same as position for unit sphere)
				meshData.Vertices.push_back(x);
				meshData.Vertices.push_back(y);
				meshData.Vertices.push_back(z);
			}
		}

		// Generate indices
		for (int lat = 0; lat < segments; lat++)
		{
			for (int lon = 0; lon < segments; lon++)
			{
				int first = (lat * (segments + 1)) + lon;
				int second = first + segments + 1;

				meshData.Indices.push_back(first);
				meshData.Indices.push_back(second);
				meshData.Indices.push_back(first + 1);

				meshData.Indices.push_back(second);
				meshData.Indices.push_back(second + 1);
				meshData.Indices.push_back(first + 1);
			}
		}

		return meshData;
	}

	MeshData MeshGenerator::CreateCapsule(float height, float radius, int segments)
	{
		MeshData meshData;

		float halfHeight = height * 0.5f;

		// Generate cylinder portion
		for (int i = 0; i <= segments; i++)
		{
			float angle = i * 2.0f * PI / segments;
			float x = cos(angle);
			float z = sin(angle);

			// Top circle vertex
			meshData.Vertices.push_back(x * radius);
			meshData.Vertices.push_back(halfHeight);
			meshData.Vertices.push_back(z * radius);
			meshData.Vertices.push_back(x);
			meshData.Vertices.push_back(0.0f);
			meshData.Vertices.push_back(z);

			// Bottom circle vertex
			meshData.Vertices.push_back(x * radius);
			meshData.Vertices.push_back(-halfHeight);
			meshData.Vertices.push_back(z * radius);
			meshData.Vertices.push_back(x);
			meshData.Vertices.push_back(0.0f);
			meshData.Vertices.push_back(z);
		}

		// Generate cylinder indices
		for (int i = 0; i < segments; i++)
		{
			int topLeft = i * 2;
			int bottomLeft = i * 2 + 1;
			int topRight = ((i + 1) % (segments + 1)) * 2;
			int bottomRight = ((i + 1) % (segments + 1)) * 2 + 1;

			meshData.Indices.push_back(topLeft);
			meshData.Indices.push_back(bottomLeft);
			meshData.Indices.push_back(topRight);

			meshData.Indices.push_back(topRight);
			meshData.Indices.push_back(bottomLeft);
			meshData.Indices.push_back(bottomRight);
		}

		int cylinderVertexCount = (segments + 1) * 2;

		// Generate top hemisphere
		int hemisphereSegments = segments / 2;
		for (int lat = 0; lat <= hemisphereSegments; lat++)
		{
			float theta = lat * PI * 0.5f / hemisphereSegments;
			float sinTheta = sin(theta);
			float cosTheta = cos(theta);

			for (int lon = 0; lon <= segments; lon++)
			{
				float phi = lon * 2.0f * PI / segments;
				float sinPhi = sin(phi);
				float cosPhi = cos(phi);

				float x = cosPhi * sinTheta;
				float y = cosTheta;
				float z = sinPhi * sinTheta;

				meshData.Vertices.push_back(x * radius);
				meshData.Vertices.push_back(y * radius + halfHeight);
				meshData.Vertices.push_back(z * radius);
				meshData.Vertices.push_back(x);
				meshData.Vertices.push_back(y);
				meshData.Vertices.push_back(z);
			}
		}

		// Generate top hemisphere indices
		for (int lat = 0; lat < hemisphereSegments; lat++)
		{
			for (int lon = 0; lon < segments; lon++)
			{
				int first = cylinderVertexCount + (lat * (segments + 1)) + lon;
				int second = first + segments + 1;

				meshData.Indices.push_back(first);
				meshData.Indices.push_back(second);
				meshData.Indices.push_back(first + 1);

				meshData.Indices.push_back(second);
				meshData.Indices.push_back(second + 1);
				meshData.Indices.push_back(first + 1);
			}
		}

		int topHemisphereVertexCount = (hemisphereSegments + 1) * (segments + 1);

		// Generate bottom hemisphere
		for (int lat = 0; lat <= hemisphereSegments; lat++)
		{
			float theta = lat * PI * 0.5f / hemisphereSegments;
			float sinTheta = sin(theta);
			float cosTheta = cos(theta);

			for (int lon = 0; lon <= segments; lon++)
			{
				float phi = lon * 2.0f * PI / segments;
				float sinPhi = sin(phi);
				float cosPhi = cos(phi);

				float x = cosPhi * sinTheta;
				float y = -cosTheta;
				float z = sinPhi * sinTheta;

				meshData.Vertices.push_back(x * radius);
				meshData.Vertices.push_back(y * radius - halfHeight);
				meshData.Vertices.push_back(z * radius);
				meshData.Vertices.push_back(x);
				meshData.Vertices.push_back(y);
				meshData.Vertices.push_back(z);
			}
		}

		// Generate bottom hemisphere indices
		for (int lat = 0; lat < hemisphereSegments; lat++)
		{
			for (int lon = 0; lon < segments; lon++)
			{
				int first = cylinderVertexCount + topHemisphereVertexCount + (lat * (segments + 1)) + lon;
				int second = first + segments + 1;

				meshData.Indices.push_back(first);
				meshData.Indices.push_back(second);
				meshData.Indices.push_back(first + 1);

				meshData.Indices.push_back(second);
				meshData.Indices.push_back(second + 1);
				meshData.Indices.push_back(first + 1);
			}
		}

		return meshData;
	}

}
