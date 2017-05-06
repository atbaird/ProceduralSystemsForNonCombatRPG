#include <windows.h>
#include <math.h>
#include <cassert>
#include <crtdbg.h>
#include "Game/TheApp.hpp"
#include "Engine/Renderer/FrameBuffer.hpp"
#include "Engine/Renderer/3DModelling/Mesh.hpp"
#include "Engine/Utils/AllocationsConsoleCommands.hpp"
#include "Engine/Utils/Allocations.hpp"
#include "Engine/Utils/Logger.hpp"
#include "Engine/Core/JobSystem/JobSystem.hpp"
#include "Engine/Core/Performance/Profiler.hpp"
#include "Engine/EventSystem/EventSystemConsoleCommands.hpp"
#include "Engine/Window/Window.hpp"
#include "Engine/Console/Console.hpp"
#include "Engine/EventSystem/EventSystem.hpp"
#pragma comment( lib, "opengl32" ) // Link in the OpenGL32.lib static library

/*
NOTE:
for whatever reason upon running, at least on my computer, it appears as though the .exe file will open but the focus will try to remain on
Visual Studio 2013.  There appears to be no errors or warnings that appear to hint towards why this is... So for the time being
if you encounter this, just click on VS2013, then on the .exe file presently running (it should be visible on the window's toolbar),
and you'll be able to see and interact with protogame.
*/


//-----------------------------------------------------------------------------------------------
#define UNUSED(x) (void)(x);


//-----------------------------------------------------------------------------------------------
const int OFFSET_FROM_WINDOWS_DESKTOP = 50;
const int WINDOW_PHYSICAL_WIDTH = 1600;
const int WINDOW_PHYSICAL_HEIGHT = 900;
const double VIEW_LEFT = 0.0;
const double VIEW_RIGHT = 1600.0;
const double VIEW_BOTTOM = 0.0;
const double VIEW_TOP = VIEW_RIGHT * static_cast<double>(WINDOW_PHYSICAL_HEIGHT) / static_cast<double>(WINDOW_PHYSICAL_WIDTH);


bool g_isQuitting = false;
const char* APP_NAME = "RPGWithNoCombat";


//-----------------------------------------------------------------------------------------------
void Update()
{
	static double s_timeLastFrameBegan = GetCurrentTimeSeconds(); // "static" local variables are initialized once, when first encountered, thus this function call only happens once
	double timeThisFrameBegan = GetCurrentTimeSeconds();
	float deltaSeconds = static_cast<float>(timeThisFrameBegan - s_timeLastFrameBegan);
	s_timeLastFrameBegan = timeThisFrameBegan;
	g_TheApp->Update(deltaSeconds);

	if (g_TheGame->isQuitting() == true)
	{
		g_isQuitting = true;
	}
}

//-----------------------------------------------------------------------------------------------
void Render()
{
	g_TheApp->Render();
	Window::g_Window->FlipBuffers();
	//SwapBuffers(g_displayDeviceContext);
}

void UpdateWindow()
{
	Window::g_Window->UpdateInformationOnWindow();
	Window::g_Window->RunMessagePump();
	if (Window::g_Window->GetFocused())
	{
		g_TheApp->OnGainedFocus();
	}
	else
	{
		g_TheApp->OnLostFocus();
	}
	g_isQuitting = Window::g_Window->GetQuitting();
	IntVector2 WindowPosition = Window::g_Window->GetWindowTopLeftCorner();
	IntVector2 windowSize = Window::g_Window->GetWindowSize();
	IntVector2 originalSize = Window::g_Window->GetOriginalExpectedWindowSize();
	g_Renderer->SetScreenStart(WindowPosition);
	g_Renderer->SetScreenEnd(WindowPosition + windowSize);
}
//-----------------------------------------------------------------------------------------------
void RunFrame()
{
	UpdateWindow();
	Update();
	Render();
}

//-----------------------------------------------------------------------------------------------
void Initialize()
{
	//CreateOpenGLWindow(applicationInstanceHandle);
	//Profiler::RegisterProfilerConsoleCommands();
	g_Renderer = new Renderer();
	//g_Console = new Console();
	//AllocationsConsoleCommands::AddConsoleCommands();

	//SetUpFrameBuffer();
	g_TheGame = new TheGame();
	g_TheApp->SetUpFrameBuffer();
	GameSettings* settings = TheGame::GetGameSettings();
	std::string fontContainingFolder = "Data/Font/";
	std::string fontFNTFile = "DebugFont.fnt";
	if (settings != nullptr)
	{
		fontContainingFolder = settings->m_FontContainingFolder;
		fontFNTFile = settings->m_FontFNTFileName;
	}

	Font* theFont = Font::CreateOrGetFont(fontContainingFolder, fontFNTFile);
	g_TheApp->m_font = theFont;

	g_TheGame->SetFont(theFont);
	g_TheGame->StepsForAfterConstruction();
	//EventSystemConsoleCommands::RegisterEventSystemConsoleCommands();
	//JobSystem::StartupJobSystem(-1);
}


//-----------------------------------------------------------------------------------------------
void Shutdown()
{
	//JobSystem::ShutdownJobSystem();
	delete g_TheGame;
	delete g_TheApp;
	delete g_Renderer;
	delete g_input;
	delete g_debugRenderer;
	if (g_Console != nullptr)
	{
		delete g_Console;
		g_Console = nullptr;
	}
	g_TheGame = nullptr;
	g_TheApp = nullptr;
	g_Renderer = nullptr;
	g_input = nullptr;
	g_debugRenderer = nullptr;
	EventSystem::DestroyGlobalEventSystem();
	Font::ClearFonts();
	ShaderProgram::DeletAllShaderPrograms();

	Texture::DeleteAllTextures();
}

void SpawnWindow(HINSTANCE applicationInstanceHandle)
{
	EventSystem::CreateGlobalEventSystem();
	g_Console = nullptr;
	g_input = new Input(1, 1);
	g_input->ActivateDPIAwareness(); // this tells Windows not to lie about mouse position and input.
	g_debugRenderer = new DebugRenderring();
	g_TheApp = new TheApp();
	Window::g_Window = new Window(APP_NAME, VIEW_LEFT, VIEW_RIGHT, VIEW_BOTTOM, VIEW_TOP, OFFSET_FROM_WINDOWS_DESKTOP,
		WINDOW_PHYSICAL_WIDTH, WINDOW_PHYSICAL_HEIGHT);
	Window::g_Window->CreateOpenGLWindow(applicationInstanceHandle);
}


//-----------------------------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE applicationInstanceHandle, HINSTANCE, LPSTR commandLineString, int)
{
	UNUSED(commandLineString);
	SpawnWindow(applicationInstanceHandle);
	Logger::GlobalStart();
	Allocations::MemoryAnalyticsStartup();
	Initialize();
	Profiler::ProfilerSystemStartup();

	while (!g_isQuitting)
	{
		RunFrame();
	}

	Profiler::ProfilerSystemsShutdown();
	Shutdown();
	Allocations::MemoryAnalyticsShutdown();
	Logger::GlobalEnd();
	delete Window::g_Window;
	Window::g_Window = nullptr;
	return 0;
}


