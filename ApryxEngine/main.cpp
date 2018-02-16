#include "Win32Window.h"

int main() {
	using namespace apryx;

	Win32Window window("OpenGL?");

	window.setVisible(true);

	while (true)
		window.poll();

	window.destroy();
}