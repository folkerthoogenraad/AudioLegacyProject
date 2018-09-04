#pragma once

#include <vector>
#include "audio/AudioUtils.h"

namespace apryx {
	class IIRDelay {
		std::vector<double> m_Samples;
		size_t m_Index = 0;

		double m_Feedback = 0.2;
	public:
		void setFeedback(double feedback) { m_Feedback = feedback; }
		void setDelay(size_t samples) { if (m_Samples.size() != samples) m_Samples.resize(samples); }

		inline double process(double in) {
			if (m_Samples.size() == 0)
				return in;

			// TODO what to do with this when the delay1 time is changing?
			m_Index %= m_Samples.size();

			double out = m_Samples[m_Index];

			m_Samples[m_Index] = audioMix(in, out * m_Feedback);

			m_Index++;

			return out;
		}
	};
}