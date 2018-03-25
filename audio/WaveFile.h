#pragma once

#include <string>

#include "PCMClip.h"

namespace apryx {

	class WaveFile : public PCMClip {
	public:
		WaveFile(std::string file);


	};

}