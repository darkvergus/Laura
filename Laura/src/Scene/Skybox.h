#pragma once

#include "lrpch.h"

namespace Laura 
{
	enum class SkyboxType
	{
		SKYBOX_TEXTURE = 0,
		SKYBOX_GRADIENT = 1
	};

	class Skybox
	{
	public:
		Skybox(uint32_t);
		Skybox(glm::vec3 groundColor, glm::vec3 horizonColor, glm::vec3 zenithColor);
		~Skybox();
		
		/// SETTERS ///
		inline void setID(uint32_t skyboxID){ m_ID = skyboxID; };
		inline void setGroundColor(glm::vec3 groundColor){ m_GroundColor = groundColor; };
		inline void setHorizonColor(glm::vec3 horizonColor){ m_HorizonColor = horizonColor; };
		inline void setZenithColor(glm::vec3 zenithColor){ m_ZenithColor = zenithColor; };
		inline void changeType(SkyboxType skyboxType){ m_SkyboxType = skyboxType; };

		/// GETTERS ///
		inline uint32_t getID()					   const { return m_ID; };
		inline const glm::vec3& getGroundColor()   const { return m_GroundColor; };
		inline const glm::vec3& getHorizonColor()  const { return m_HorizonColor; };
		inline const glm::vec3& getZenithColor()   const { return m_ZenithColor; };
		inline SkyboxType getType()                const { return m_SkyboxType; }
	
	private:
		SkyboxType m_SkyboxType;
		uint32_t m_ID;
		glm::vec3 m_GroundColor, m_HorizonColor, m_ZenithColor;
	};
}