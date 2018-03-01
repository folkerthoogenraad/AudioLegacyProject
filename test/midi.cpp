
#include <iostream>
#include <cstdlib>
#include <signal.h>
#include <bitset>
#include <array>

#include "audio/AudioSystem.h"
#include "audio/AudioBuffer.h"
#include "audio/AudioSource.h"
#include "audio/AudioUtils.h"

#include "audio/rtmidi/RtMidi.h"
#include "engine/Timer.h"

bool done;
static void finish(int ignore) { done = true; }

int main() {
	using namespace apryx;

	const double frameRate = 240;

	const int bufferSize = 256;
	const int bufferCount = 4;

	// =====================================================//
	// RTMidi stuff
	// =====================================================//
	auto midiIn = std::make_shared<RtMidiIn>();

	unsigned int nPorts = midiIn->getPortCount();
	if (nPorts == 0) {
		std::cout << "No ports available!\n";
	}
	midiIn->openPort(0);

	// Don't ignore sysex, timing, or active sensing messages.
	midiIn->ignoreTypes(false, false, false);


	// =====================================================//
	// Audio format stuff
	// =====================================================//
	AudioFormat format(44100, 16, AudioFormat::Mono);

	auto system = std::make_shared<AudioSystem>(format);
	auto source = std::make_shared<AudioSource>(system);

	std::vector<double> audioSamples;
	audioSamples.resize(bufferSize);

	std::array<std::shared_ptr<AudioBuffer>, bufferCount> audioBuffers;
	for (int i = 0; i < audioBuffers.size(); i++) {
		audioBuffers[i] = std::make_shared<AudioBuffer>(system);

		audioBuffers[i]->setDataNormalized(audioSamples, format);

		source->queueBuffer(*audioBuffers[i]);
	}

	source->play();

	double phase = 0;
	double frequency = 200;

	int pCount = 0;
	while (true) {


		// Reading midi stuff
		bool cont = true;
		while (cont) {
			std::vector<unsigned char> message;
			float stamp = midiIn->getMessage(&message);

			int nBytes = message.size();
			/*for (int i = 0; i < nBytes; i++)
				std::cout << "Byte " << i << " = " << ((int)message[i]) << std::dec << ", ";

			if (nBytes > 0)
				std::cout << "stamp = " << stamp << std::endl;
			if (nBytes == 0)
				cont = false;*/

			if (nBytes != 0)
			{
				if(message[0] == 144){
					frequency = midiToFrequency(message[1]);
				}
			}
			else {
				cont = false;
			}
		}

		if (!source->isPlaying()) {
			source->play();
		}

		int processed = source->getBuffersProcessed();

		// Writing audio stuff
		while (processed > 0) {
			source->unqueueBuffer(*audioBuffers[pCount]);

			double phaseAdd = frequency / format.getSampleRate();

			for (int i = 0; i < audioSamples.size(); i++) {

				audioSamples[i] = audioSine(phase);

				phase += phaseAdd;
			}

			audioBuffers[pCount]->setDataNormalized(audioSamples, format);

			source->queueBuffer(*audioBuffers[pCount]);

			pCount = (pCount + 1) % audioBuffers.size();

			processed -= 1;
		}

		Timer::sleep(1 / frameRate);
	}

	std::cout << "Welp its done now bitch" << std::endl;

	std::cin.get();

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

	// Install an interrupt handler function.
	done = false;
	(void)signal(SIGINT, finish);

	// Periodically check input queue.
	std::cout << "Reading MIDI from port ... quit with Ctrl-C.\n";
	while (!done) {
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