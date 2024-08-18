#include "Scene/Skybox.h"

namespace Laura
{
	Skybox::Skybox()
		: m_SkyboxType(SkyboxType::SKYBOX_TEXTURE),
		m_GroundColor(glm::vec3(0.63f, 0.58f, 0.63f)),
		m_HorizonColor(glm::vec3(1.00f, 1.00f, 1.00f)),
		m_ZenithColor(glm::vec3(0.48f, 0.71f, 0.92f)),
		m_SkyboxFilepath(std::string(LR_RESOURCES_PATH "Skyboxes/Metro_default.hdr"))
	{
	}

	Skybox::Skybox(std::string& skyboxFilepath)
		: m_SkyboxFilepath(skyboxFilepath), 
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
		m_SkyboxFilepath(std::string(LR_RESOURCES_PATH "Skyboxes/Metro_default.hdr"))
	{
	}

	Skybox::~Skybox()
	{
	}
}