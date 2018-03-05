#pragma once

#include <vector>

#include "AudioFormat.h"

namespace apryx {

	class PCMSource {
	public:
		virtual bool get(std::vector<double> &values, AudioFormat format) { return false; };
	};

}