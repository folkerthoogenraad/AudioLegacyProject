#include "PCMTestSource.h"

#include "AudioUtils.h"

bool apryx::PCMTestSource::get(std::vector<double>& values, AudioFormat format)
{
	if (!m_Playing)
		return false;

	double phaseIncrement = m_Frequency / format.sampleRate;

	int sampleCount = values.size() / format.channels;

	for (int i = 0; i < sampleCount; i++) {
		double phase = m_Phase + phaseIncrement * i;

		double value = audioSine(phase) * m_Gain;

		// Set the value for all the channels
		for (int j = 0; j < format.channels; j++) {
			values[i * format.channels + j] = value;
		}
	}

	m_Phase += phaseIncrement * sampleCount;

	return true;
}
