#pragma once

#include "PCMSource.h"

namespace apryx {

	class PCMDelay : public PCMEffect {

		std::vector<AudioSample> m_Samples;
		size_t m_Size;
		size_t m_Index;

		float m_Feedback = 0.5f;
	public:
		PCMDelay();

		// Sets the amount of samples for the delay. 
		// This should always be a multiple of the amount of channels
		// Use AudioUtils::toSamples
		void setDelaySamples(size_t samples);
		size_t getDelaySamples() const { return m_Size; }

		// Set the amount of feedback
		// Use AudioUtils::toGain(db) to enter db settings
		void setFeedback(float feedback) { m_Feedback = feedback; }
		float getFeedback() const { return m_Feedback; }

		virtual bool get(std::vector<double> &values, AudioFormat format);
	};

}