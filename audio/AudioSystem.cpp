#include "AudioSystem.h"

#include "test/iir/IIRFilter.h"

#include <climits>

#define DOUBLE_P false

namespace apryx {

	static int processRTAudio(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
		double streamTime, RtAudioStreamStatus status, void *userData)
	{
		AudioSystem* system = (AudioSystem*)userData;

		const auto &source = system->getSource();

		// Should be allocating this buffer, but whatever atm
		std::vector<double> samples(nBufferFrames * system->getAudioFormat().channels);

		
		source->get(samples, system->getAudioFormat());

#if DOUBLE_P // Convert as double
		double *buffer = (double*)outputBuffer;
		for (int i = 0; i < samples.size(); i++) {
			buffer[i] = samples[i];
		}
#else
		short *buffer = (short*)outputBuffer;
		for (int i = 0; i < samples.size(); i++) {
			buffer[i] = (short) (samples[i] * SHRT_MAX);
		}
#endif
		return 0;
	}

	AudioSystem::AudioSystem()
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
		RtAudio::StreamOptions options;
		options.flags = RTAUDIO_MINIMIZE_LATENCY;
		options.numberOfBuffers = 0;
		options.streamName = "Best stream ever.";

		RtAudio::StreamParameters parameters;
		parameters.deviceId = 1;//m_Dac.getDefaultOutputDevice();// 
		parameters.nChannels = format.channels;
		parameters.firstChannel = 0;
		unsigned int sampleRate = format.sampleRate;// 44100;
		unsigned int bufferFrames = 128; // 256 sample frames

		try {
			m_Dac.openStream(&parameters, NULL, DOUBLE_P ? RTAUDIO_FLOAT64 : RTAUDIO_SINT16,
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