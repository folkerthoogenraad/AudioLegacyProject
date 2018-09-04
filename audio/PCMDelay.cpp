#include "PCMDelay.h"

namespace apryx {
	
	PCMDelay::PCMDelay(size_t samples, float feedback)
		: m_Feedback(feedback)
	{
		setDelaySamples(samples);
	}

	void PCMDelay::setDelaySamples(size_t samples)
	{
		// TODO downsizing
		if (samples > m_Samples.size()) {
			m_Samples.resize(samples);
		}

		m_Size = samples;
	}

	bool PCMDelay::get(std::vector<double>& values, AudioFormat format)
	{
		for (int i = 0; i < values.size(); i++) {
			if (m_Index >= m_Size)
				m_Index = 0;

			AudioSample v = values[i] + m_Samples[m_Index] * m_Feedback;

			m_Samples[m_Index] = v;
			values[i] = v;

			m_Index++;
		}
		return true;
	}

}