#pragma once

#include "audio/PCMSource.h"

#include "iir/IIRDelay.h"

namespace apryx {

	class CombEffect : public PCMEffect {
	public:
		virtual bool get(std::vector<double> &values, AudioFormat format);
	};
}