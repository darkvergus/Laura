#pragma once
#include "Core/GUID.h"

#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/euler_angles.hpp>


namespace Laura
{
	struct GUIDComponent
	{
		LR_GUID guid;
	};

	struct TagComponent
	{
		TagComponent() = default;
		TagComponent(const std::string& tag)
			: Tag(tag) {};

		std::string Tag;
	};

	struct TransformComponent
	{
	public:
		TransformComponent();
		operator glm::mat4() const;

		inline const glm::vec3& GetRotation() { return glm::degrees(m_Rotation); }
		inline const glm::vec3& GetTranslation() { return m_Translation; }
		inline const glm::vec3& GetScale() { return m_Scale; }

		// Returns the 4x4 Local to World Matrix
		glm::mat4 GetMatrix() const;

		void SetRotation(const glm::vec3& angles);
		void SetTranslation(const glm::vec3& translation);
		void SetScale(const glm::vec3& scale);

		void IncrementRotation(const glm::vec3& delta);
		void IncrementTranslation(const glm::vec3& delta);
		void IncrementScale(const glm::vec3& delta);

	private:
		mutable bool m_MatrixDirty;
		mutable glm::mat4 m_ModelMatrix;

		glm::vec3 m_Rotation;
		glm::vec3 m_Translation;
		glm::vec3 m_Scale;
	};

	struct MeshComponent
	{
		LR_GUID guid = LR_GUID(0);
	};

	struct MaterialComponent
	{
		LR_GUID guid = LR_GUID(0);
	};

	struct CameraComponent
	{
		CameraComponent() = default;
		CameraComponent(float fov)
			: fov(fov) {};
		
		bool isMain{ false };
		float fov{ 90.0f };
		// since we transform the size of the screen in the compute shader to "normalized device coordinates" or NDC for short (-1, 1) 
		// half of the screen width is 1. Therefore (screen width / 2) / tan(FOV in radians / 2) can be simplified to 1 / tan(FOV_rad / 2)
		inline const float& GetFocalLength() const { return 1.0f/tan(glm::radians(fov)/2.0f); };
	};
}