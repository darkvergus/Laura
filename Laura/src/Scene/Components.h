#pragma once
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>

#include "Geometry/Triangle.h" // for mesh component
#include "Renderer/Material.h" // for material component

namespace Laura
{

	struct TransformComponent
	{
		TransformComponent() = default;
		TransformComponent(const glm::mat4& transform)
			: transform(transform) {}

		glm::mat4 transform{ 1.0f };
	};

	namespace TransformHandler
	{
		void Rotate(TransformComponent& transform, glm::vec3& angles);

		void Translate(TransformComponent& transform, const glm::vec3& translation);

		void Scale(TransformComponent& transform, const glm::vec3& scale);
	}


	struct MeshComponent
	{
		MeshComponent() = default;
		MeshComponent(const std::vector<Triangle>& mesh)
			: mesh(mesh) {}

		std::vector<Triangle> mesh;
	};

	struct MaterialComponent
	{
		MaterialComponent() = default;
		MaterialComponent(glm::vec3 color, float emissionStrength, glm::vec3 emissionColor)
			: color(color), emissionStrength(emissionStrength), emissionColor(emissionColor) {}
			
		glm::vec3 color;              // offset 0   // alignment 16 // size 12 // total 12 bytes
		float emissionStrength;       // offset 12  // alignment 4  // size 4  // total 16 bytes
		glm::vec3 emissionColor;      // offset 16  // alignment 16 // size 12 // total 28 bytes
		float std140padding;          // offset 28  // alignment 4  // size 4  // total 32 bytes
	};

	struct CameraComponent
	{
		CameraComponent() = default;
		CameraComponent(float fov, float aspectRatio)
			: fov(fov), aspectRatio(aspectRatio) {};

		float fov{ 80.0f };
		float aspectRatio{ 16.0f/9.0f };
		// since we transform the size of the screen in the compute shader to "normalized device coordinates" or NDC for short (-1, 1) 
		// half of the screen width is 1. Therefore (screen width / 2) / tan(FOV in radians / 2) can be simplified to 1 / tan(FOV_rad / 2)
		inline const float& GetFocalLength() const { return 1.0f/tan(glm::radians(fov)/2.0f); };
	};

}