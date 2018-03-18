
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

#include "engine/SpriteBatch.h"

#define GRAPHICS_ONLY true

int main() 
{
#if !GRAPHICS_ONLY
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
	auto source = std::make_shared<PCMTestSource>();

	AudioSystem system;

	system.play(format, source);

#endif
	using namespace apryx;

	Win32Window window("OpenGL", 1280, 720, false);

	Timer timer;
	double timeSum = 0;
	int frameCount = 0;


	glClearColor(0, 0, 0, 1);

	Image image = Image::checkerboard(16, 16);

	GLTexture texture;
	texture.setFiltering(GLTexture::NearestNeighbour);
	texture.setWrapping(GLTexture::Clamp);

	texture.setData(image);

	window.setVisible(true);

	SpriteBatch batch;

	timer.start();

	while (!window.isCloseRequested()) {
		window.poll();

		if (window.isResized()) {
			glViewport(0, 0, window.getWidth(), window.getHeight());
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		batch.begin();

		batch.color(Color32::white());

		batch.uv(Vector2f(0, 0));
		batch.vertex(Vector3f(0, 0));

		batch.uv(Vector2f(0, 1));
		batch.vertex(Vector3f(0, 1));

		batch.color(Color32::red());

		batch.uv(Vector2f(1, 1));
		batch.vertex(Vector3f(1, 1));

		batch.uv(Vector2f(1, 0));
		batch.vertex(Vector3f(1, 0));

		batch.end();

		timeSum += timer.lap();
		frameCount++;
		if (timeSum > 1) {
			std::cout << "FPS : " << frameCount << " " << timeSum << std::endl;
			frameCount = 0;
			timeSum -= 1;
		}

		window.swap();
		Timer::sleep(1 / 240.f);
}

	window.destroy();

	return 0;
}