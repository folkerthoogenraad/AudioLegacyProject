#include "PCMMixer.h"

namespace apryx {

	bool PCMMixer::get(std::vector<double>& values, AudioFormat format)
	{
		// This is a bad plan, allocation here
		std::vector<AudioSample> samples(values.size());

		for (auto &v : m_Sources) {
			v->processSamples(samples, format);
			
			for (int i = 0; i < samples.size(); i++) {
				values[i] += samples[i];
				samples[i] = 0;
			}
		}

		return true;
	}
}
