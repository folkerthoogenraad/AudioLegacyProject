#include "AudioSystem.h"

#include "test/iir/IIRFilter.h"

#include "AudioUtils.h"

#include <climits>
#include <Windows.h>

#define FLOAT_P

namespace apryx {

	static int processRTAudio(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
		double streamTime, RtAudioStreamStatus status, void *userData)
	{
		AudioSystem* system = (AudioSystem*)userData;

		const auto &source = system->getSource();

		// Should be allocating this buffer, but whatever atm

		int wantedSize = nBufferFrames * system->getAudioFormat().channels;

		if (system->m_Buffer.size() != wantedSize) {
			system->m_Buffer.resize(wantedSize);

			std::cout << "Setting stuff" << std::endl;

			SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST | THREAD_PRIORITY_TIME_CRITICAL);
		}

		std::fill(system->m_Buffer.begin(), system->m_Buffer.end(), 0);

		source->processSamples(system->m_Buffer, system->getAudioFormat());

#ifdef DOUBLE_P // Convert as double
		double *buffer = (double*)outputBuffer;
		for (int i = 0; i < system->m_Buffer.size(); i++) {
			buffer[i] = system->m_Buffer[i];
		}
#endif
#ifdef FLOAT_P // Convert as double
		float *buffer = (float*)outputBuffer;
		for (int i = 0; i < system->m_Buffer.size(); i++) {
			buffer[i] = (float)system->m_Buffer[i];
		}
#endif
#ifdef SHORT_P
		short *buffer = (short*)outputBuffer;
		for (int i = 0; i < system->m_Buffer.size(); i++) {
			buffer[i] = (short)(system->m_Buffer[i] * SHRT_MAX);
		}
#endif

#ifdef INT_P
		int *buffer = (int*)outputBuffer;
		for (int i = 0; i < system->m_Buffer.size(); i++) {
			buffer[i] = (int)(apryx::audioClamp(system->m_Buffer[i], -1, 1) * INT_MAX);
		}
#endif
		return 0;
	}

	AudioSystem::AudioSystem()
		:m_Dac(RtAudio(RtAudio::Api::WINDOWS_WASAPI))
	{ }

	AudioSystem::~AudioSystem()
	{
		stop();
	}

	void AudioSystem::play(AudioFormat format, std::shared_ptr<PCMSource> source)
	{
		m_Source = source;
		m_AudioFormat = format;

		int deviceCount = m_Dac.getDeviceCount();

		if (deviceCount < 1) {
			std::cout << "\nNo audio devices found!\n";
			std::cin.get();
			exit(0);
		}

		RtAudio::DeviceInfo info = m_Dac.getDeviceInfo(m_Dac.getDefaultOutputDevice());

		std::cout << info.preferredSampleRate << " -> " << format.sampleRate << std::endl;
		std::cout << info.outputChannels << " -> " << format.channels << std::endl;

		RtAudio::StreamOptions options;
		options.flags = RTAUDIO_MINIMIZE_LATENCY;
		options.numberOfBuffers = 0;
		options.streamName = "Best stream ever.";

		RtAudio::StreamParameters parameters;

		parameters.deviceId = m_Dac.getDefaultOutputDevice();
		parameters.nChannels = format.channels;
		parameters.firstChannel = 0;

		unsigned int sampleRate = format.sampleRate;// 44100;
		unsigned int bufferFrames = 256; // 256 sample frames

		try {
#ifdef DOUBLE_P
			int format = RTAUDIO_FLOAT64;
#endif
#ifdef SHORT_P
			int format = RTAUDIO_SINT16;
#endif
#ifdef INT_P
			int format = RTAUDIO_SINT32;
#endif
#ifdef FLOAT_P
			int format = RTAUDIO_FLOAT32;
#endif
			m_Dac.openStream(&parameters, NULL, format,
				sampleRate, &bufferFrames, &processRTAudio, (void *)this, &options);
			m_Dac.startStream();
		}
		catch (RtAudioError& e) {
			e.printMessage();
			std::cout << "Press any key to continue...";
			std::cin.get();
			exit(0);
		}
	}

	void AudioSystem::stop()
	{
		try {
			// Stop the stream
			m_Dac.stopStream();
		}
		catch (RtAudioError& e) {
			e.printMessage();
		}
		if (m_Dac.isStreamOpen()) m_Dac.closeStream();
	}


}