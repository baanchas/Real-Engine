#pragma once

namespace RealEngine {

	class Timestep
	{
	public:
		Timestep(float timeStep = 0)
			: m_Time(timeStep) {};
		
		float GetSeconds() { return m_Time; }
		float GetMilliSeconds() { return m_Time / 1000.0f; }

	private:
		float m_Time;
	};

}
