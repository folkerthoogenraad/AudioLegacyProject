#include "Grainer.h"

apryx::Grainer::Grainer()
{
	m_Samples.resize(1);
	m_Samples[0] = 0;
}

void apryx::Grainer::setSamples(std::vector<double> samples)
{
	m_Samples = std::move(samples);
}

void apryx::Grainer::configure(std::vector<double> samples, int sampleRate)
{
	setSamples(std::move(samples));
	setSampleRate(sampleRate);
}
