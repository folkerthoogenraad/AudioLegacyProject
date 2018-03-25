#pragma once

#include "AudioFormat.h"
#include <vector>

namespace apryx {

	class PCMClip {
	protected:
		AudioFormat m_Format;
		std::vector<float> m_Samples;
	public:
		virtual ~PCMClip() = default;

		AudioFormat getFormat() const { return m_Format; };
		const std::vector<float> &getSamples() const { return m_Samples; };

	};

}