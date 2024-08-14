#include "DeltaTime.h"

#include <GLFW/glfw3.h>

namespace Laura
{

	DeltaTime::DeltaTime()
		: m_PrevTime(0.0f), m_CurrTime(0.0f)
	{
	}

	void DeltaTime::Update()
	{
		m_PrevTime = m_CurrTime;
		m_CurrTime = glfwGetTime();
	}

}