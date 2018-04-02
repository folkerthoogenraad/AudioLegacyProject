#include "Win32Window.h"

#include <Windowsx.h>

#include "GL.h"

#define CLASS_NAME "Win32WindowOpenGL"

#include <iostream>

namespace apryx {

	static LRESULT CALLBACK DefaultWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		Win32Window *p = ((Win32Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA));

		if (p) {
			return p->handleWindowsMessage(uMsg, wParam, lParam);
		}

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

	static void initGL()
	{
		glewInit();

		glClearColor(0, 0, 1, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	Win32Window::Win32Window(std::string title, int width, int height, bool full)
		: Window(title,width,height,full)
	{
		m_Width = width;
		m_Height = height;

		SetProcessDPIAware();

		registerClass();

		// Make sure its the client size
		RECT wr = { 0, 0, width, height};    // set the size, but not the position
		AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);    // adjust the size
		RegisterTouchWindow(m_Hwnd, 0);

		m_Hwnd = CreateWindowEx(
			0,                              // Optional window styles.
			CLASS_NAME,                     // Window class
			m_Title.c_str(),				// Window text
			WS_OVERLAPPEDWINDOW,            // Window style

											// Size and position
			0, 0, wr.right - wr.left, wr.bottom - wr.top,

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

		// Get the device scaling stuff
		m_DPIScale = GetDeviceCaps(deviceContext, LOGPIXELSX) / 96.f;

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

		initGL();
	}

	void Win32Window::setVisible(bool visible, bool maximize)
	{
		if (maximize && visible) {
			ShowWindow(m_Hwnd, SW_MAXIMIZE);
		}
		else if (visible) {
			ShowWindow(m_Hwnd, SW_SHOW);
		}
		else if (!visible) {
			ShowWindow(m_Hwnd, SW_HIDE);
		}
	}

	bool Win32Window::isResized() const
	{
		return m_Resized;
	}

	bool Win32Window::isCloseRequested() const
	{
		return m_CloseRequested;
	}

	float Win32Window::getWidth() const
	{
		return m_Width / m_DPIScale;
	}

	float Win32Window::getHeight() const
	{
		return m_Height / m_DPIScale;
	}

	LRESULT Win32Window::handleWindowsMessage(UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{
		case WM_POINTERDOWN:
		{
			//https://msdn.microsoft.com/en-us/library/hh454928(v=vs.85).aspx


			POINT point;
			point.x = GET_X_LPARAM(lParam);
			point.y = GET_Y_LPARAM(lParam);

			ScreenToClient(m_Hwnd, &point);

		}
		break;
		case WM_CLOSE:
			m_CloseRequested = true;
			break;

		case WM_SIZE:
			int width = LOWORD(lParam);  // Macro to get the low-order word. 
			int height = HIWORD(lParam); // Macro to get the high-order word. 

			m_Width = width;
			m_Height = height;

			m_Resized = true;
			break;
		}

		return DefWindowProc(m_Hwnd, message, wParam, lParam);
	}

	void Win32Window::destroy()
	{
		wglDeleteContext(m_GLContext);
		CloseWindow(m_Hwnd);
	}

	float Win32Window::dpiScale() const
	{
		return m_DPIScale;
	}

	void Win32Window::poll()
	{
		m_CloseRequested = false;
		m_Resized = false;


		MSG msg = {};
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
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
