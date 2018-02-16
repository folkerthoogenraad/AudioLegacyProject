#include "Win32Window.h"

#define CLASS_NAME "Win32WindowOpenGL"

#include <iostream>

namespace apryx {

	static LRESULT CALLBACK DefaultWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	static inline PIXELFORMATDESCRIPTOR win32GetPixelFormatDescriptor()
	{
		static  PIXELFORMATDESCRIPTOR pfd =              // pfd Tells Windows How We Want Things To Be
		{
			sizeof(PIXELFORMATDESCRIPTOR),              // Size Of This Pixel Format Descriptor
			1,                                          // Version Number
			PFD_DRAW_TO_WINDOW |                        // Format Must Support Window
			PFD_SUPPORT_OPENGL |                        // Format Must Support OpenGL
			PFD_DOUBLEBUFFER,                           // Must Support Double Buffering
			PFD_TYPE_RGBA,                              // Request An RGBA Format
			32,                                         // Select Our Color Depth
			0, 0, 0, 0, 0, 0,                           // Color Bits Ignored
			0,                                          // No Alpha Buffer
			0,                                          // Shift Bit Ignored
			0,                                          // No Accumulation Buffer
			0, 0, 0, 0,                                 // Accumulation Bits Ignored
			24,                                         // 16Bit Z-Buffer (Depth Buffer)  
			8,                                          // No Stencil Buffer
			0,                                          // No Auxiliary Buffer
			PFD_MAIN_PLANE,                             // Main Drawing Layer
			0,                                          // Reserved
			0, 0, 0                                     // Layer Masks Ignored
		};

		return pfd;
	}

	static void registerClass() 
	{
		WNDCLASS wc = {};

		wc.lpfnWndProc = DefaultWindowProc;
		wc.hInstance = GetModuleHandle(NULL);
		wc.lpszClassName = CLASS_NAME;

		RegisterClass(&wc);

	}

	Win32Window::Win32Window(std::string title, int width, int height, bool full)
		: Window(title,width,height,full)
	{
		registerClass();

		m_Hwnd = CreateWindowEx(
			0,                              // Optional window styles.
			CLASS_NAME,                     // Window class
			m_Title.c_str(),				// Window text
			WS_OVERLAPPEDWINDOW,            // Window style

											// Size and position
			0, 0, m_Width, m_Height,

			NULL,       // Parent window    
			NULL,       // Menu
			GetModuleHandle(NULL),  // Instance handle
			NULL        // Additional application data
		);

		if (!m_Hwnd) {
			MessageBox(NULL, "Failed to create window.", "Error", MB_OK | MB_ICONEXCLAMATION);
			return;
		}

		// Set the user data
		SetWindowLongPtr(m_Hwnd, GWLP_USERDATA, (LONG_PTR)this);

		PIXELFORMATDESCRIPTOR descriptor = win32GetPixelFormatDescriptor();

		int pixelFormat;

		HDC deviceContext = GetDC(m_Hwnd);

		if (!deviceContext) {
			MessageBox(NULL, "Failed to obtain the device context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
			return;                               // Return FALSE
		}

		pixelFormat = ChoosePixelFormat(deviceContext, &descriptor);

		if (!pixelFormat)
		{
			MessageBox(NULL, "Failed to find a correct pixel format.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
			return;
		}

		if (!SetPixelFormat(deviceContext, pixelFormat, &descriptor))
		{
			MessageBox(NULL, "Failed to set the pixel format.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
			return;
		}

		m_GLContext = wglCreateContext(deviceContext);

		if (!m_GLContext)
		{
			MessageBox(NULL, "Failed to create OpenGL context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
			return;
		}

		if (!wglMakeCurrent(deviceContext, m_GLContext))
		{
			MessageBox(NULL, "Failed to make OpenGL context active on this thread.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
			return;
		}

		ReleaseDC(m_Hwnd, deviceContext);
	}

	void Win32Window::setVisible(bool visible)
	{
		ShowWindow(m_Hwnd, visible ? SW_SHOW : SW_HIDE);
	}

	void Win32Window::destroy()
	{
		wglDeleteContext(m_GLContext);
		CloseWindow(m_Hwnd);
	}

	void Win32Window::poll()
	{
		MSG msg = {};
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_CREATE) {
				std::cout << "Boem bats lekker luchtdicht" << std::endl;
			}
			if (msg.message == WM_KEYDOWN) {
				std::cout << "Je moeder" << std::endl;
			}
			if (msg.message == WM_CHAR) {
				std::cout << "char bitch" << std::endl;
			}

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	void Win32Window::swap()
	{
		HDC dc = GetDC(m_Hwnd);
		SwapBuffers(dc);
		ReleaseDC(m_Hwnd, dc);
	}

}
