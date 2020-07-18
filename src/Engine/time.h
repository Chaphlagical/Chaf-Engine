#pragma once

namespace Chaf
{
	class Timestep
	{
	public:
		Timestep(float time = 0.0f)
			: m_Time(time)
		{
		}

		operator float() const { return m_Time; }

		float GetSeconds() const { return m_Time; }
		float GetMilliseconds() const { return m_Time * 1000.0f; }
		void Update(float lastFrameTime = 0.0f);

	private:
		float m_Time;
	};
}