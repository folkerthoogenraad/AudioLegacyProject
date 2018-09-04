#pragma once

#include <vector>

namespace apryx {

	class Grainer {
		std::vector<double> m_Samples;
		int m_SampleRate = 48000;
		
		double m_Pitch = 1;
		double m_Speed = 1;

		double m_MasterIndex = 0;

		double m_GrainerIndex = 0;
		bool m_GrainSwitch = false;

		double m_GrainerOffset1 = 0;
		double m_GrainerOffset2 = 0;

		double m_GrainerTimer1 = 0;
		double m_GrainerTimer2 = 0;

		double m_GrainSamples = 480; //10ms
	public:
		Grainer();

		void setSamples(std::vector<double> samples);

		const std::vector<double> &getSamples() const { return m_Samples; }
		std::vector<double> &getSamples() { return m_Samples; }

		void setSampleRate(int rate) { m_SampleRate = rate; }
		int getSampleRate() const { return m_SampleRate; }

		void configure(std::vector<double> samples, int sampleRate);

		//Maybe precompute as optimisation?
		inline double getSampleCount() const { return m_Samples.size(); }
		inline double getSampleLength() const { return getSampleCount() / (double)m_SampleRate; }

		void setSpeed(double speed) { m_Speed = speed; }
		void setPitch(double pitch) { m_Pitch = pitch; }
		void setGrainFrequency(double frequency) { m_GrainSamples = m_SampleRate / frequency; }

		inline double get() 
		{
			m_MasterIndex += m_Speed;

			m_GrainerIndex += 1;

			// Switching offsets and shit
			if (m_GrainerIndex > m_GrainSamples / 2) {
				m_GrainerIndex -= m_GrainSamples / 2;

				if (m_GrainSwitch) {
					m_GrainSwitch = false;
					m_GrainerOffset1 = m_MasterIndex;
					m_GrainerTimer1 = 0;
				}
				else {
					m_GrainSwitch = true;
					m_GrainerOffset2 = m_MasterIndex;
					m_GrainerTimer2 = 0;

				}

			}

			m_GrainerTimer1 += m_Pitch;
			m_GrainerTimer2 += m_Pitch;


			if (m_MasterIndex > getSampleCount())
				m_MasterIndex -= getSampleCount();
			if (m_MasterIndex < 0)
				m_MasterIndex += getSampleCount();


			if (m_GrainerTimer1 > getSampleCount())
				m_GrainerTimer1 -= getSampleCount();
			if (m_GrainerTimer1 < 0)
				m_GrainerTimer1 += getSampleCount();


			if (m_GrainerTimer2 > getSampleCount())
				m_GrainerTimer2 -= getSampleCount();
			if (m_GrainerTimer2 < 0)
				m_GrainerTimer2 += getSampleCount();

			double amount = m_GrainerIndex / (m_GrainSamples / 2);

			// 0 to 1 range

			double blend = pow(1 - pow(amount * 2 - 1, 2), 1.25);

			if (blend > 1)
				blend = 1;

			double value = 0;

			double sample1 = interpolate(m_GrainerOffset1 + m_GrainerTimer1);
			double sample2 = interpolate(m_GrainerOffset2 + m_GrainerTimer2);

			if (m_GrainSwitch) {
				value = sample1 + (sample2 - sample1) * blend;
			}
			else {
				value = sample2 + (sample1 - sample2) * blend;
			}


			return value;
		}

		inline double interpolate(double sample) {
			int n = m_Samples.size();

			float low = m_Samples[(int)floor(sample) % n];
			float high = m_Samples[(int)ceil(sample) % n];

			double distance = fmod(sample, 1);

			return low + (high - low) * distance;
		}
	};

}