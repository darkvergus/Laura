#pragma once
#include "glm/glm.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/euler_angles.hpp>

#include "Geometry/Triangle.h" // for mesh component
#include "Renderer/Material.h" // for material component

namespace Laura
{

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
	public:
		MeshComponent() = default;
		inline uint32_t GetID() { return m_ID; }
		inline void SetID(uint32_t id) { m_ID = id; }
	private:
		uint32_t m_ID;
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
		CameraComponent(float fov)
			: fov(fov) {};
		
		bool isMain{ false };
		float fov{ 90.0f };
		// since we transform the size of the screen in the compute shader to "normalized device coordinates" or NDC for short (-1, 1) 
		// half of the screen width is 1. Therefore (screen width / 2) / tan(FOV in radians / 2) can be simplified to 1 / tan(FOV_rad / 2)
		inline const float& GetFocalLength() const { return 1.0f/tan(glm::radians(fov)/2.0f); };
	};

	struct Script
	{	
		Script() = default;

		virtual void OnCreate() = 0;
		virtual void OnUpdate() = 0;
		virtual void OnDestroy() = 0;
	};

	struct ScriptComponent
	{
		ScriptComponent() = default;
		ScriptComponent(Script* script)
			: script(script) {}

		Script* script;
	};

	struct TagComponent
	{
		TagComponent() = default;
		TagComponent(const std::string& tag)
			: Tag(tag) {};

		std::string Tag;
	};
}