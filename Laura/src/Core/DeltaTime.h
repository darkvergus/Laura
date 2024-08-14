#pragma once

/**
* @brief DeltaTime class is used to calculate the time between frames to make the movement of objects independent of the frame rate.
 */
namespace Laura
{

	class DeltaTime
	{
	public:
		DeltaTime();
		// Calculates the time elapsed between the previous call of the Update() function and the current call.
		// Use Get() or Get_ms() to get this time
		void Update();
		
		inline float Get() const { return m_CurrTime - m_PrevTime; }
		inline float Get_ms() const { return (m_CurrTime - m_PrevTime) * 1000; }
	private:
		float m_PrevTime;
		float m_CurrTime;
	};

}