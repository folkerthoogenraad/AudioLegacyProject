#pragma once

#include "AudioSystem.h"
#include "AudioBuffer.h"
#include <memory>

namespace apryx {

	class AudioSource {
	private:
		unsigned int m_SourceID;
		std::weak_ptr<AudioSystem> m_System;
	public:
		AudioSource(std::weak_ptr<AudioSystem> system);

		void play();
		void play(AudioBuffer &clip);
		void queueBuffer(AudioBuffer &buffer);
		void unqueueBuffer(AudioBuffer &buffer);

		bool isPlaying() const;

		int getBuffersProcessed();

		~AudioSource();
	};

}