#ifndef CAMERA_H
#define CAMERA_H

#include "lrpch.h"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/constants.hpp>

namespace Laura {
	class Camera3D
	{
	public:
		virtual void Rotate(float pitch, float yaw, float roll) = 0;
		virtual void Translate(glm::vec3 translation) = 0;
		virtual float GetFocalLength(uint16_t screenWidth, uint16_t screenHeight) = 0;

		/// GETTERS ///
		inline const glm::vec3& GetPosition() const { return m_Position; }
		inline const float GetFov() const { return m_Fov; }
		inline const float GetAspectRatio() const { return m_AspectRatio; }

		inline const glm::mat4 GetTransformationMatrix() const
		{
			glm::mat4 rotationMatrix = glm::toMat4(m_Rotation);
			glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), m_Position);
			return rotationMatrix * translationMatrix; // potentially wrong order
		}

		/// SETTERS ///
		inline void SetPosition(const glm::vec3& position) { m_Position = position; }
		inline void SetFov(float fov) { m_Fov = fov; }
		inline void SetAspectRatio(float aspectRatio) { m_AspectRatio = aspectRatio; }

	protected:
		glm::quat m_Rotation;
		glm::vec3 m_Position;
		float m_Fov;
		float m_AspectRatio;
	};


	class FlyThroughCamera : public Camera3D
	{
	public:
		FlyThroughCamera(float fov, float aspectRatio);

		virtual void Rotate(float pitch, float yaw, float roll) override;
		virtual inline void Translate(glm::vec3 translation) override { m_Position += translation; };

		inline void MoveForward(float distance) { m_Position += glm::normalize(glm::vec3(m_Rotation * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f))) * distance; }
		inline void MoveBackward(float distance) { m_Position -= glm::normalize(glm::vec3(m_Rotation * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f))) * distance; }
		inline void MoveUp(float distance) { m_Position += glm::normalize(glm::vec3(m_Rotation * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f))) * distance; }
		inline void MoveDown(float distance) { m_Position -= glm::normalize(glm::vec3(m_Rotation * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f))) * distance; }
		inline void MoveRight(float distance) { m_Position += glm::normalize(glm::vec3(m_Rotation * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f))) * distance; }
		inline void MoveLeft(float distance) { m_Position -= glm::normalize(glm::vec3(m_Rotation * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f))) * distance; }
	};

}

#endif // CAMERA_H