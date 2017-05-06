#pragma once

#ifndef WINDOW_HPP
#define WINDOW_HPP
#include <windows.h>
#define WIN32_LEAN_AND_MEAN
#include "Engine/Math/IntVector2.hpp"
#include "Engine/Math/AABB2.hpp"

class Window
{
private:
protected:
	IntVector2 m_minSize = IntVector2(0, 0);
	IntVector2 m_WindowSize = IntVector2(1600, 900);
	IntVector2 m_OriginalExpectedWindowSize = IntVector2(1600, 900);
	IntVector2 m_FullScreenResolution = IntVector2(0, 0);
	IntVector2 m_WindowTopLeftPos = IntVector2(0, 0);
	IntVector2 m_trueCursorPosition = IntVector2(0, 0);
	IntVector2 m_cursorPositionRelativeToWind = IntVector2(0, 0);
	IntVector2 m_windowBarSize = IntVector2(16, 39);
	HWND m_hWnd = nullptr;
	HDC m_displayDeviceContext = nullptr;
	HGLRC m_openGLRenderingContext = nullptr;
	double m_ViewLeft = 0.0;
	double m_ViewRight = 1600.0;
	double m_ViewBottom = 0.0;
	double m_ViewTop = 1600.0 * static_cast<double>(1600) / static_cast<double>(900);
	const char* m_appName = "Win32 OpenGL Test App";
	int m_offsetFromWindowsDesktop = 50;
	int m_windowWidth = 1600;
	int m_windowHeight = 900;
	bool m_quitting = false;
	bool m_focused = false;


public:
	static Window* g_Window;

	Window();
	Window(const char* appName, double viewLeft, double viewRight, double viewBottom, double viewTop,
		int offsetFromWindowsDesktop, int windowWidth, int windowHeight, const IntVector2& minSize = IntVector2(0,0));

	//Setters
	void SetQuit(bool quit);

	//Getters
	const IntVector2& GetMinSize() const;
	const IntVector2& GetWindowSize() const;
	const IntVector2& GetOriginalExpectedWindowSize() const;
	const IntVector2& GetWindowTopLeftCorner() const;
	const IntVector2& GetTrueCursorPosition() const;
	const IntVector2& GetCursorPositionRelativeToWindow() const;
	bool GetQuitting() const;
	bool GetFocused() const;
	Vector2 GetMouseVirtualCoordsRelativeToTheWindow(const Vector2& VirtualCoordScale, bool& mouseActuallyInUse, 
		float rightDirMult = 1.f, float upDirMult = 1.f) const;

	//Operations
	void FlipBuffers() const;
	void UpdateInformationOnWindow();


	//Window Creation functions
	static LRESULT CALLBACK WindowsMessageHandlingProcedure(HWND windowHandle, UINT wmMessageCode,
		WPARAM wParam, LPARAM lParam);
	void CreateOpenGLWindow(HINSTANCE applicationInstanceHandle);
	void RunMessagePump();
};
#endif