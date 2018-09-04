#pragma once

#include "rtaudio/RtAudio.h"
#include "AudioFormat.h"
#include "PCMSource.h"

#include <memory>

namespace apryx {

	class AudioSystem {

		RtAudio m_Dac;
		AudioFormat m_AudioFormat;

		std::shared_ptr<PCMSource> m_Source;

	public:// SHOULD NOT BE PUBLIC, TODO FIX
		std::vector<double> m_Buffer;
	public:
		AudioSystem();
		~AudioSystem();

		void play(AudioFormat format, std::shared_ptr<PCMSource> source);
		void stop();

		const std::shared_ptr<PCMSource> &getSource() const { return m_Source; };
		const AudioFormat &getAudioFormat() const { return m_AudioFormat; }
	};

}