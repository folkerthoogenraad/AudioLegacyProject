
// Holy moly clean this shit up bro

#include <iostream>
#include <cstdlib>
#include <signal.h>
#include <bitset>
#include <array>
#include <memory>

#include "audio/AudioUtils.h"

#include "audio/MidiController.h"
#include "audio/PCMTestSource.h"
#include "engine/Timer.h"

#include "audio/rtaudio/RtAudio.h"
#include "audio/AudioSystem.h"
#include <iostream>
#include <cstdlib>

#include "MidiSource.h"

#include "engine/Win32Window.h"

#include <iostream>

#include "engine/GLShaderProgram.h"
#include "engine/GLVertexBufferObject.h"

#include "engine/GL.h"

#include "math/Matrix4.h"

#include "engine/Image.h"
#include "engine/GLTexture.h"

#include "engine/Graphics2D.h"

#include "engine/Timer.h"

#include "engine/GLBatch.h"

#include "audio/Distortion.h"
#include "audio/BitCrusher.h"
#include "audio/SpaceDelay.h"

#include "audio/WaveFile.h"
#include "audio/PCMClipSource.h"

#include "audio/PCMDelay.h"
#include "audio/PCMMixer.h"

#include "engine/GLGraphicsContext.h"

#include "Wavetable.h"
#include "iir\Grainer.h"

namespace apryx {
	class WavetableOscillator : public PCMSource{

		double phase = 0;

	public:
		Wavetable<256> waveTable;
		double frequency;
		double gain;

		double blend = 0.5;
		double feedback = 0.5;

		Grainer grainer;

		IIRDelay delay1;
		IIRDelay delay2;
		IIRDelay delay3;

		int offset = 0;

		double old = 0;

		WavetableOscillator() {
			size_t base = 48000;

			delay1.setFeedback(feedback);
			delay1.setDelay(3491);

			delay2.setFeedback(feedback);
			delay2.setDelay(1109);

			delay3.setFeedback(feedback);
			delay3.setDelay(223);

		}

		virtual bool get(std::vector<double> &values, AudioFormat format) 
		{
			for (int i = 0; i < values.size() / format.channels; i++) 
			{
#if 0
				double value = waveTable.interpolate(phase) * gain;

				phase += frequency / format.sampleRate;

				phase = fmod(phase, 1);

				
#endif
				double value = grainer.get();

				value = (value * blend + old * (1 - blend));
				old = value;

				//value = audioMix(delay2.process(value), value);
				//value = audioMix(delay3.process(value), value);

				for (int j = 0; j < format.channels; j++) {
					values[i * 2 + j] = value;
				}
			}

			return true;
		}
	};

}

#define MIDI_STUFF true

int main() 
{
#if MIDI_STUFF
	using namespace apryx;

	// =====================================================//
	// Midi stuff
	// =====================================================//
	auto midiIn = std::make_shared<MidiController>();

	unsigned int nPorts = midiIn->getPortCount();
	
	if (nPorts == 0) {
		std::cout << "No ports available!\n";
		std::cin.get();
		return -1;
	}

	midiIn->openPort(0);

	AudioSystem system;

	AudioFormat format;
	format.channels = 2;
	format.sampleRate = 48000;

	auto ptr = std::make_shared<WavetableOscillator>();

	{
	}

	// create grainer
	{
		std::vector<double> samples;
		
		WaveFile file("wav/promise.wav");
		AudioFormat format = file.getFormat();

		for (int i = 0; i < file.getSamples().size(); i += format.channels) 
		{
			double value = 0;

			for (int j = 0; j < format.channels; j++) 
			{
				value += file.getSamples()[i + j];
			}

			value /= format.channels;

			samples.push_back(value);
		}

		ptr->grainer.configure(samples, format.sampleRate);
	}

	ptr->gain = 0.5;
	ptr->frequency = 220;

	for (int i = 0; i < ptr->waveTable.size(); i++) {
		double timer = i / (double)ptr->waveTable.size();

		ptr->waveTable.set(i, audioSawtooth(timer));
	}

	system.play(format, std::make_shared<MidiSource>(midiIn));

	Timer midiTimer;
	midiTimer.start();

	std::cin.get();

	while (true) {
		for (auto value : midiIn->poll()) {
			if (value.isControlChange() && value.getControlIndex() == 2)
			{
				ptr->grainer.setSpeed(value.getControlValue() * value.getControlValue());
			}

			if (value.isControlChange() && value.getControlIndex() == 3)
			{
				ptr->grainer.setPitch(value.getControlValue() * 2);
			}

			if (value.isControlChange() && value.getControlIndex() == 4)
			{
				ptr->blend = value.getControlValue() * value.getControlValue();
			}

			if (value.isControlChange() && value.getControlIndex() == 5)
			{
				ptr->feedback = value.getControlValue();
			}

			if (value.isNoteOn()) {
				ptr->grainer.setGrainFrequency(value.getKeyFrequency());
			}
		}

		
		Timer::sleep(1 / 100.0);
	}

	std::cin.get();
	if (true)
		return 0;

#endif

	using namespace apryx;

	// =====================================================//
	// Audio stuff
	// =====================================================//

	Win32Window window("OpenGL", 1280, 720, false);

	auto context = std::make_shared<GLGraphicsContext>();

	auto graphics = context->createGraphics();

	float dpi = window.dpiScale();

	std::cout << window.getWidth() << " : " << window.getRawWidth() << std::endl;

	Timer timer;
	double timeSum = 0;
	double frameSum = 0;
	int updateCount = 0;
	int frameCount = 0;

	glClearColor(0, 0, 1, 1);

	Image image = Image::checkerboard(16, 16);

	window.setVisible(true, false);

	GLBatch batch;
	{
		glViewport(0, 0, window.getRawWidth(), window.getRawHeight());
		batch.setSize(window.getWidth(), window.getHeight());
		graphics->setSize(window.getWidth(), window.getHeight());
	}


	timer.start();

	while (!window.isCloseRequested()) {
		window.poll();

		if (window.isResized()) {
			glViewport(0, 0, window.getRawWidth(), window.getRawHeight());
			batch.setSize(window.getWidth(), window.getHeight());
			graphics->setSize(window.getWidth(), window.getHeight());
		}

		double delta = timer.lap();
		
		frameSum += delta * 20;
		timeSum += delta;
		if (timeSum > 1) {
			std::cout << "FPS : " << frameCount << " | UPS : " << updateCount << std::endl;
			frameCount = 0;
			timeSum -= 1;
			updateCount = 0;
		}

		updateCount++;

		if (frameSum > 1) {
			frameSum -= 1;
			frameCount++;

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			Paint redPaint;
			redPaint.setColor(Color32::red());

			Paint whitePaint;
			whitePaint.setColor(Color32::white());

			graphics->drawRectangle(redPaint, Rectanglef::centered(256, 256, 128, 128));
			graphics->drawImage(whitePaint, image, Vector2f(0,0));

			graphics->flush();
			window.swap();
		}

		Timer::sleep(1 / 20.0);
	}

	window.destroy();

	return 0;
}