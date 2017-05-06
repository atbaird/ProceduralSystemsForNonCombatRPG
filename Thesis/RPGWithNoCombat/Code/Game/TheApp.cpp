#include "Game/TheApp.hpp"
#include "Engine/Console/Console.hpp"
#include "Engine/Math/Vertices/Vertex_PCTT.hpp"

//--------------------------------------------
TheApp* g_TheApp = nullptr;

//--------------------------------------------
TheApp::TheApp() : m_focus(true),
isTildeHeldDown(false)
{
}
TheApp::~TheApp()
{
	if (m_frameBuffer != nullptr)
	{
		FramebufferDelete(m_frameBuffer);
	}
	if (m_ScreenPassThrough != nullptr)
	{
		delete m_ScreenPassThrough;
	}
	if (myMat != nullptr)
	{
		delete myMat;
	}
	if (myMesh != nullptr)
	{
		delete myMesh;
	}
}

void TheApp::Update(const float& deltaSeconds)
{
	if (m_focus)
	{
		if (g_Console != nullptr && isTildeHeldDown == false && g_input->IsKeyDown(tilde) == true)
		{
			isTildeHeldDown = true;
			g_Console->SetConsoleActive(!g_Console->IsConsoleActive());
		}

		if (g_Console == nullptr || g_Console->IsConsoleActive() == false)
		{
			g_TheGame->Update(deltaSeconds, m_focus);
		}
		else if (g_Console != nullptr && g_Console->IsConsoleActive() == true)
		{
			g_TheGame->Update(0.f, false);
			g_Console->Update(deltaSeconds);
		}
	}
	else
	{
		g_TheGame->Update(0.f, m_focus);
	}
	if (g_input->IsKeyDown(tilde) == false)
	{
		isTildeHeldDown = false;
	}
}

void TheApp::Render() const
{
	g_Renderer->clearScreen(Rgba(0.0f, 0.f, 0.f, 1.f), true);
	if (g_TheGame != nullptr)
	{
		//if (m_useFrameBuffer == true)
		//{
		//	myMat->SetProgram(m_ScreenPassThrough);
		//	g_Renderer->FramebufferBind(m_frameBuffer);
		//	g_Renderer->clearScreen(Rgba::s_Black, true);
		//}
		g_TheGame->Render();
		//if (m_useFrameBuffer == true)
		//{
		//	g_Renderer->FramebufferBind(nullptr);
		//	g_Renderer->RenderPostProcess(m_frameBuffer, myMat, myMesh);
		//}
	}
	if (g_Console != nullptr && g_Console->IsConsoleActive())
	{
		g_Console->Render();
	}
}

const bool TheApp::isQuitting() const
{
	if (g_TheGame->isQuitting() || g_Console->IsQuitting())
	{
		return true;
	}
	return false;
}

void TheApp::SetUpFrameBuffer()
{
	return;
	//m_useFrameBuffer = true;
	//Texture** colors = new Texture*[1];
	//colors[0] = new Texture(1600, 900, eTextureFormat_RGBA8);
	//Texture* stencil = new Texture(1600, 900, eTextureFormat_D24S8);
	//m_ScreenPassThrough = new ShaderProgram("Data/Shaders/Framebuffers/ScreenPassthrough.vert", "Data/Shaders/Framebuffers/ScreenPassthrough.frag");
	//myMat = new Material();
	//myMesh = new Mesh();
	//std::vector<Vertex_PCTT> quad;
	//quad.push_back(Vertex_PCTT(Vector3(-1.f, 1.f, 0.f), Rgba(1.f, 0.f, 1.f, 1.f), Vector2(0.f, 0.f), Vector3(1.f, 0.f, 0.f), Vector3(0.f, 0.f, 1.f))); //TL
	//quad.push_back(Vertex_PCTT(Vector3(-1.f, -1.f, 0.f), Rgba(0.f, 1.f, 1.f, 1.f), Vector2(0.f, 1.f), Vector3(1.f, 0.f, 0.f), Vector3(0.f, 0.f, 1.f))); //BL
	//quad.push_back(Vertex_PCTT(Vector3(1.f, -1.f, 0.f), Rgba(1.f, 1.f, 0.f, 1.f), Vector2(1.f, 1.f), Vector3(1.f, 0.f, 0.f), Vector3(0.f, 0.f, 1.f))); //BR
	//quad.push_back(Vertex_PCTT(Vector3(1.f, 1.f, 0.f), Rgba(1.f, 1.f, 1.f, 1.f), Vector2(1.f, 0.f), Vector3(1.f, 0.f, 0.f), Vector3(0.f, 0.f, 1.f))); //TR
	//myMesh->setVBO(quad);
	//
	//std::vector<unsigned int> indices;
	//indices.push_back(0);
	//indices.push_back(1);
	//indices.push_back(2);
	//indices.push_back(2);
	//indices.push_back(3);
	//indices.push_back(0);
	//myMesh->setIBO(indices);
	//
	//unsigned int eh = 0;
	//g_Renderer->GenerateVAO(eh);
	//myMesh->setVAOID(eh);
	//m_frameBuffer = FramebufferCreate(1, colors, stencil);
	//int bind = 0;
	//int depthBind = 0;
	//myMat->AddTexture(m_frameBuffer->get_color_texture(0), bind, "gTexDiffuse");
	//myMat->AddTexture(m_frameBuffer->get_depth_stencil_target(), depthBind, "gTexDepth");
	//
	//
	//Rgba white = Rgba(1.f, 0.f, 1.f, 1.f);
	//m_ScreenPassThrough->SetProgramValueInt("gDiffuseTex", &bind);
	//unsigned int sampler = g_Renderer->CreateSampler(NEAREST_DISTANCE_TYPE, NEAREST_DISTANCE_TYPE, REPEAT_DISTANCE_TYPE, REPEAT_DISTANCE_TYPE);
	//myMesh->SetSamplerID(sampler);
}