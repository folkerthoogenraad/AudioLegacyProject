
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

#define MIDI_STUFF false

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
		return -1;
	}

	midiIn->openPort(0);

#endif

	using namespace apryx;

	// =====================================================//
	// Audio stuff
	// =====================================================//

	Win32Window window("OpenGL", 1280, 720, false);

	auto context = std::make_shared<GLGraphicsContext>();

	auto graphics = context->createGraphics();

	float dpi = window.dpiScale();

	Timer timer;
	double timeSum = 0;
	double frameSum = 0;
	int updateCount = 0;
	int frameCount = 0;

	glClearColor(0, 0, 1, 1);

	Image image = Image::checkerboard(16, 16);

	window.setVisible(true, true);

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