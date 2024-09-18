#include "Scene/Skybox.h"

namespace Laura
{
	Skybox::Skybox(uint32_t ID)
		: m_ID(ID),
		m_SkyboxType(SkyboxType::SKYBOX_TEXTURE), 
		m_GroundColor(glm::vec3(0.63f, 0.58f, 0.63f)),
		m_HorizonColor(glm::vec3(1.00f, 1.00f, 1.00f)),
		m_ZenithColor(glm::vec3(0.48f, 0.71f, 0.92f))
	{
	}

	Skybox::Skybox(glm::vec3 groundColor, glm::vec3 horizonColor, glm::vec3 zenithColor)
		: m_GroundColor(groundColor),
		m_HorizonColor(horizonColor),
		m_ZenithColor(zenithColor),
		m_SkyboxType(SkyboxType::SKYBOX_GRADIENT),
		m_ID(0)
	{
	}

	Skybox::~Skybox()
	{
	}
}