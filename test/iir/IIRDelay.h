#pragma once

#include <vector>

namespace apryx {
	class IIRDelay {
		std::vector<double> m_Samples;
		size_t m_Index = 0;

		double m_Feedback = 0.2;
	public:

		void setFeedback(double feedback) { m_Feedback = feedback; }
		void setDelay(size_t samples) { if(m_Samples.size() != samples) m_Samples.resize(samples); }

		inline double process(double in) {
			if (m_Samples.size() == 0)
				return in;

			m_Index = m_Index % m_Samples.size();

			double out = m_Samples[m_Index] + in;
			m_Samples[m_Index] = out * m_Feedback;

			m_Index++;

			return out;
		}
	};
}