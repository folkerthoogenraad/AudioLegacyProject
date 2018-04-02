#pragma once

#include "PCMSource.h"

namespace apryx {

	class PCMTestSource : public PCMSource {
		bool m_Playing = false;

		double m_Frequency = 220;
		double m_Gain = 0.5;

		double m_Phase = 0;
	public:
		PCMTestSource(double frequency = 220, double gain = 0.5) : m_Frequency(frequency), m_Gain(gain) {}

		void setFrequency(double freq) { m_Frequency = freq; }
		double getFrequency() const { return m_Frequency; }

		// audioUtils toGain(double db) to get the db amount.
		void setGain(double gain) { m_Gain = gain; }
		double getGain() const { return m_Gain; }

		void setPlaying(bool p) { m_Playing = p; }
		bool isPlaying() const { return m_Playing; }

		virtual bool get(std::vector<double> &values, AudioFormat format);
	};

}