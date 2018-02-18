#include "Timer.h"

namespace apryx {
	Timer::Timer()
	{
	}

	void Timer::start()
	{
		m_Running = true;

		m_Start = std::chrono::system_clock::now();
		m_Previous = m_Start;
	}

	double Timer::lap()
	{
		auto current = std::chrono::system_clock::now();

		std::chrono::duration<double> d = (current - m_Previous);
		double time = d.count();

		m_Previous = current;

		return time;
	}

	double Timer::runtime()
	{
		auto current = std::chrono::system_clock::now();

		std::chrono::duration<double> d = (current - m_Start);

		if (!m_Running) {
			d = (m_End - m_Start);
		}

		double time = d.count();

		return time;
	}

	void Timer::end()
	{
		m_Running = false;
		m_End = std::chrono::system_clock::now();
	}
}