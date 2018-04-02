#pragma once

namespace apryx {

	typedef double AudioSample;

	struct AudioFormat{
		unsigned int channels = 2;
		unsigned int sampleRate = 48000;
	};

}