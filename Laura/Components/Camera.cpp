#include "Components/Camera.h"

namespace Laura 
{
	Camera::Camera()
		: m_FOV(70.0f), m_FocalLength(1 / tan(70.0f / 2))
	{
	}

	void Camera::setFOV(float fov)
	{
		m_FOV = fov;
		// since we transform the size of the screen in the compute shader to "normalized device coordinates" or NDC for short (-1, 1) 
		// half of the screen width is 1. Therefore (screen width / 2) / tan(FOV in radians / 2) can be simplified to 1 / tan(FOV_rad / 2)
		m_FocalLength = 1 / tan(fov / 2);
	}
	float Camera::getFOV() const
	{
		return m_FOV;
	}
	float Camera::getFocalLength() const
	{
		return m_FocalLength;
	}
}



