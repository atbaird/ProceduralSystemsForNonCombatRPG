#include "Engine/Window/Window.hpp"
#include "Engine/Core/Input.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Math/AABB2.hpp"

STATIC Window* Window::g_Window = nullptr;

Window::Window()
{
}

Window::Window(const char* appName, double viewLeft, double viewRight, double viewBottom, double viewTop,
	int offsetFromWindowsDesktop, int windowWidth, int windowHeight, const IntVector2& minSize)
	: m_appName(appName),
	m_ViewLeft(viewLeft),
	m_ViewRight(viewRight),
	m_ViewBottom(viewBottom),
	m_ViewTop(viewTop),
	m_offsetFromWindowsDesktop(offsetFromWindowsDesktop),
	m_windowWidth(windowWidth),
	m_windowHeight(windowHeight),
	m_minSize(minSize),
	m_WindowSize(windowWidth, windowHeight),
	m_OriginalExpectedWindowSize(windowWidth, windowHeight),
	m_WindowTopLeftPos(offsetFromWindowsDesktop / 2, offsetFromWindowsDesktop / 2)
{

}

//---------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------
//Setters
void Window::SetQuit(bool quit)
{
	m_quitting = quit;
}

//---------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------
//Getters
const IntVector2& Window::GetMinSize() const
{
	return m_minSize;
}

const IntVector2& Window::GetWindowSize() const
{
	return m_WindowSize;
}

const IntVector2& Window::GetOriginalExpectedWindowSize() const
{
	return m_OriginalExpectedWindowSize;
}

const IntVector2& Window::GetWindowTopLeftCorner() const
{
	return m_WindowTopLeftPos;
}

const IntVector2& Window::GetTrueCursorPosition() const
{
	return m_trueCursorPosition;
}

const IntVector2& Window::GetCursorPositionRelativeToWindow() const
{
	return m_cursorPositionRelativeToWind;
}

bool Window::GetQuitting() const
{
	return m_quitting;
}

bool Window::GetFocused() const
{
	return m_focused;
}

Vector2 Window::GetMouseVirtualCoordsRelativeToTheWindow(const Vector2& VirtualCoordScale,
	bool& mouseActuallyInUseAndOnWindow, float rightDirMult, float upDirMult) const
{
	static const Vector2 RelativeDirectionsMult = Vector2(1.f, -1.f);
	const Vector2 multipliers = Vector2(rightDirMult * RelativeDirectionsMult.x, 
		upDirMult * RelativeDirectionsMult.y);
	IntVector2 mousePos = m_cursorPositionRelativeToWind;

	mouseActuallyInUseAndOnWindow = (mousePos.x >= 0 && mousePos.x < m_windowWidth)
		&& (mousePos.y >= 0 && mousePos.y < m_windowHeight);


	Vector2 percentages = Vector2(((float)mousePos.x / (float)m_windowWidth),
		((float)mousePos.y / (float)m_windowHeight));
	Vector2 translatedCoords = Vector2((percentages.x * VirtualCoordScale.x), 
		(percentages.y * VirtualCoordScale.y));

	if (multipliers.x < 0.f)
	{
		translatedCoords.x = VirtualCoordScale.x - translatedCoords.x;
	}
	if (multipliers.y < 0.f)
	{
		translatedCoords.y = VirtualCoordScale.y - translatedCoords.y;
	}

	return translatedCoords;
}


//---------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------
//Operations
void Window::FlipBuffers() const
{
	SwapBuffers(m_displayDeviceContext);
}

void Window::UpdateInformationOnWindow()
{
	RECT rect;
	if (GetWindowRect(m_hWnd, &rect))
	{
		m_WindowTopLeftPos.x = rect.left;
		m_WindowTopLeftPos.y = rect.top;
		m_WindowSize.x = rect.right - rect.left;
		m_WindowSize.y = rect.bottom - rect.top;
	}
	POINT p;
	GetCursorPos(&p);
	m_trueCursorPosition = IntVector2(p.x, p.y);
	ScreenToClient(m_hWnd, &p);
	m_cursorPositionRelativeToWind = IntVector2(p.x, //m_trueCursorPosition.x - m_WindowTopLeftPos.x,
		p.y);//m_trueCursorPosition.y - m_WindowTopLeftPos.y);
	m_FullScreenResolution.x = GetSystemMetrics(SM_CXSCREEN);
	m_FullScreenResolution.y = GetSystemMetrics(SM_CYSCREEN);
	//if (g_Renderer != nullptr)
	//{
	//	g_Renderer->SetScreenStart(IntVector2::ivec2_zeros);
	//	g_Renderer->SetScreenEnd(m_WindowSize);
	//}
}

//---------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------
//Window Creation functions
LRESULT CALLBACK Window::WindowsMessageHandlingProcedure(HWND windowHandle, UINT wmMessageCode, WPARAM wParam, LPARAM lParam)
{
	unsigned char asKey = (unsigned char)wParam;
	switch (wmMessageCode)
	{
	case WM_CLOSE:
	case WM_DESTROY:
	case WM_QUIT:
		if (g_Window)
		{
			g_Window->m_quitting = true;
		}
		return 0;


	case WM_LBUTTONDOWN:
		if (g_Window->m_quitting == false && g_input != nullptr)
		{
			g_input->SetKeyDownStatus(VK_LBUTTON, true);
		}
		break;
	case WM_RBUTTONDOWN:
		if (g_Window->m_quitting == false && g_input != nullptr)
		{
			g_input->SetKeyDownStatus(VK_RBUTTON, true);
		}
		break;
	case WM_LBUTTONUP:
		if (g_Window->m_quitting == false && g_input != nullptr)
		{
			g_input->SetKeyDownStatus(VK_LBUTTON, false);
		}
		break;
	case WM_RBUTTONUP:
		if (g_Window->m_quitting == false && g_input != nullptr)
		{
			g_input->SetKeyDownStatus(VK_RBUTTON, false);
		}
		break;

	case WM_KEYDOWN:
		if (g_Window->m_quitting == false && g_input != nullptr)
		{
			g_input->SetKeyDownStatus(asKey, true);
		}
		//		g_keyDownArray[ asKey ] = true;
		break;

	case WM_KEYUP:
		if (g_Window->m_quitting == false && g_input != nullptr)
		{
			g_input->SetKeyDownStatus(asKey, false);
		}
		//		g_keyDownArray[ asKey ] = false;
		break;
	case WM_SETFOCUS:
	{
		if (g_Window)
		{
			g_Window->m_focused = true;
		}
		break;
	}
	case WM_KILLFOCUS:
	{
		if (g_Window)
		{
			g_Window->m_focused = false;
		}
		break;
	}

	case WM_MOUSEWHEEL:
		short scrollDir = GET_WHEEL_DELTA_WPARAM(wParam);
		if (scrollDir > 0)
		{
			g_input->setWheelScroll(true, false);
		}
		else if (scrollDir < 0)
		{
			g_input->setWheelScroll(false, true);
		}
		break;
	}

	return DefWindowProc(windowHandle, wmMessageCode, wParam, lParam);
}

void Window::CreateOpenGLWindow(HINSTANCE applicationInstanceHandle)
{
	// Define a window class
	WNDCLASSEX windowClassDescription;
	memset(&windowClassDescription, 0, sizeof(windowClassDescription));
	windowClassDescription.cbSize = sizeof(windowClassDescription);
	windowClassDescription.style = CS_OWNDC; // Redraw on move, request own Display Context
	windowClassDescription.lpfnWndProc = static_cast<WNDPROC>(WindowsMessageHandlingProcedure); // Assign a win32 message-handling function
	windowClassDescription.hInstance = GetModuleHandle(NULL);
	windowClassDescription.hIcon = NULL;
	windowClassDescription.hCursor = LoadCursor(NULL, IDC_ARROW);
	windowClassDescription.lpszClassName = TEXT("Simple Window Class");
	RegisterClassEx(&windowClassDescription);

	g_Renderer->CreateOpenGLWindow(applicationInstanceHandle, m_appName, m_offsetFromWindowsDesktop, m_windowWidth, m_windowHeight,
		m_hWnd, m_displayDeviceContext, m_openGLRenderingContext, m_ViewLeft, m_ViewRight, m_ViewTop, m_ViewBottom, windowClassDescription);
}


void Window::RunMessagePump()
{
	MSG queuedMessage;
	for (;;)
	{
		const BOOL wasMessagePresent = PeekMessage(&queuedMessage, NULL, 0, 0, PM_REMOVE);
		if (!wasMessagePresent)
		{
			break;
		}

		TranslateMessage(&queuedMessage);
		DispatchMessage(&queuedMessage);
	}
}