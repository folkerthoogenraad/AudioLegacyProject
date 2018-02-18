#pragma once

#include <chrono>

namespace apryx {

	class Timer {
		bool m_Running = false;
		std::chrono::time_point<std::chrono::system_clock> m_Start;
		std::chrono::time_point<std::chrono::system_clock> m_Previous;
		std::chrono::time_point<std::chrono::system_clock> m_End;
	public:
		Timer();

		void start();
		
		// Both in seconds
		double lap();
		double runtime();
		
		void end();
	};

}