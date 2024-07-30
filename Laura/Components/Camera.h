#ifndef CAMERA_H
#define CAMERA_H

#include "lrpch.h"

#include "Components/TransformComponent.h"

namespace Laura 
{
	class Camera
	{
	public:
		Camera();

		/// SETTERS ///
		void setFOV(float fov);
		
		/// GETTERS ///
		float getFOV() const;
		float getFocalLength() const;

		TransformComponent transform;
	private:
		float m_FOV, m_FocalLength;
	};
}

#endif // CAMERA_H