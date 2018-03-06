
#include <iostream>
#include <cstdlib>
#include <signal.h>
#include <bitset>
#include <array>
#include <memory>

#include "audio/AudioUtils.h"

#include "audio/MidiController.h"
#include "engine/Timer.h"

#include "audio/rtaudio/RtAudio.h"
#include "audio/AudioSystem.h"
#include <iostream>
#include <cstdlib>

#include "MidiSource.h"



class TestSource : public apryx::PCMSource{
	double phase = 0;
	double frequency = 440;

	std::shared_ptr<apryx::MidiController> midiIn;
public:
	TestSource(std::shared_ptr<apryx::MidiController> midi) : midiIn(midi) {}

	virtual bool get(std::vector<double> &values, apryx::AudioFormat format) 
	{
		for (auto &event : midiIn->poll()) {
			if (event.isControlChange()) {
				std::cout << event.getControlIndex() << " = " << event.getControlValue() << std::endl;
			}
			if (event.isProgramChange()) {
				std::cout << "Goto program " << event.getProgramNumber() << std::endl;
			}
			if (event.isNoteOn()) {
				frequency = event.getKeyFrequency();
			}
		}

		for (int i = 0; i < values.size() / format.channels; i++) {

			double value = apryx::audioSine(phase) * 0.2;

			for(int j = 0; j < format.channels; j++)
				values[i * format.channels + j] = value;

			phase += frequency / (double)format.sampleRate;
		}
		return true;
	};
};

int main() 
{
	using namespace apryx;

	// =====================================================//
	// Midi stuff
	// =====================================================//
	auto midiIn = std::make_shared<MidiController>();

	unsigned int nPorts = midiIn->getPortCount();
	if (nPorts == 0) {
		std::cout << "No ports available!\n";
		return -1;
	}

	midiIn->openPort(0);


	// =====================================================//
	// Audio stuff
	// =====================================================//

	AudioFormat format;
	auto source = std::make_shared<MidiSource>(midiIn);

	AudioSystem system;

	system.play(format, source);

	std::cin.get();
}

#if 0
int main2()
{
	// =====================================================//
	// RTMidi stuff
	// =====================================================//
	midiIn = std::make_shared<apryx::MidiController>();

	unsigned int nPorts = midiIn->getPortCount();
	if (nPorts == 0) {
		std::cout << "No ports available!\n";
		return -1;
	}

	midiIn->openPort(0);


	// =====================================================//
	// RTAudio stuff
	// =====================================================//
	RtAudio dac;

	int deviceCount = dac.getDeviceCount();
	auto info = dac.getDeviceInfo(0);

	std::cout << info.name << std::endl;

	if (deviceCount < 1) {
		std::cout << "\nNo audio devices found!\n";
		std::cin.get();
		exit(0);
	}
	RtAudio::StreamOptions options;
	options.flags = RTAUDIO_MINIMIZE_LATENCY;
	options.numberOfBuffers = 1;
	options.streamName = "Best stream ever.";

	RtAudio::StreamParameters parameters;
	parameters.deviceId = 0;// dac.getDefaultOutputDevice();
	parameters.nChannels = 2;
	parameters.firstChannel = 0;
	unsigned int sampleRate = requestSampleRate;// 44100;
	unsigned int bufferFrames = 128; // 256 sample frames
	double data[2];
	try {
		dac.openStream(&parameters, NULL, RTAUDIO_FLOAT64,
			sampleRate, &bufferFrames, &saw, (void *)&data, &options);
		dac.startStream();
	}
	catch (RtAudioError& e) {
		e.printMessage();
		std::cin.get();
		exit(0);
	}

	char input;
	std::cout << "\nPlaying ... press <enter> to quit.\n";
	std::cin.get(input);
	try {
		// Stop the stream
		dac.stopStream();
	}
	catch (RtAudioError& e) {
		e.printMessage();
	}
	if (dac.isStreamOpen()) dac.closeStream();

	return 0;
}




int midi()
{
	RtMidiIn *midiin = new RtMidiIn();
	std::vector<unsigned char> message;
	int nBytes, i;
	double stamp;

	// Check available ports.
	unsigned int nPorts = midiin->getPortCount();
	if (nPorts == 0) {
		std::cout << "No ports available!\n";
		goto cleanup;
	}
	midiin->openPort(0);

	// Don't ignore sysex, timing, or active sensing messages.
	midiin->ignoreTypes(false, false, false);

	// Periodically check input queue.
	std::cout << "Reading MIDI from port ... quit with Ctrl-C.\n";
	while (true) {
		bool cont = true;
		while (cont) {
			stamp = midiin->getMessage(&message);

			nBytes = message.size();
			for (i = 0; i < nBytes; i++)
				std::cout << "Byte " << i << " = " << ((int)message[i]) << std::dec <<  ", ";
			if (nBytes > 0)
				std::cout << "stamp = " << stamp << std::endl;
			if (nBytes == 0)
				cont = false;
		}
		apryx::Timer::sleep(1/60.f);
	}

	// Clean up
cleanup:
	delete midiin;

	std::cin.get();

	return 0;
}

#endif