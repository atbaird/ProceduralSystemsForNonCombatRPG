#include "Game/Debugging/DeveloperDebugger.hpp"
#include "Engine/Renderer/Texture.hpp"
#include "Engine/Math/Vertex_PCT.hpp"
#include "Engine/Font/Font.hpp"
#include "Engine/Core/Input.hpp"
#include "Engine/Window/Window.hpp"
#include "Engine/Math/Matrix44.hpp"
#include "Game/Map/Map.hpp"

//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//Static Variables
STATIC DeveloperDebugger* DeveloperDebugger::g_DeveloperDebugger = nullptr;

//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//Constructors
DeveloperDebugger::DeveloperDebugger(float fontSize, Font* font,
	ShaderProgram* prog, const SpriteResource* whiteBoxTex)
	: m_fontSize(fontSize),
	m_Font(font),
	m_shaderProg(prog),
	m_WhiteBoxTex(whiteBoxTex)
{
	m_IdentityModelMatrix.MatrixMakeIdentity();
}

DeveloperDebugger::~DeveloperDebugger()
{
	for (size_t i = 0; i < m_TextMeshes.size(); i++)
	{
		if (m_TextMeshes[i] != nullptr)
		{
			delete m_TextMeshes[i];
			m_TextMeshes[i] = nullptr;
		}
	}
	m_TextMeshes.clear();
}


//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//Updates and Render
void DeveloperDebugger::Update(Map* map, bool currentlyLoadingAMap, const Vector2& cameraCenter)
{
	if (currentlyLoadingAMap
		|| map == nullptr
		|| !m_AllowedToRender)
	{
		m_RenderThisFrame = false;
		return;
	}
	m_RenderThisFrame = true;
	UpdateMousePosition(cameraCenter);
	if (!m_MouseOnScreen)
	{
		m_RenderThisFrame = false;
		return;
	}

	UpdateGetAllEntitiesAndTilesAtCoords(map);
	if (!m_MouseIsOnATile)
	{
		m_RenderThisFrame = false;
		return;
	}
	UpdateModels();
}

void DeveloperDebugger::UpdateMousePosition(const Vector2& cameraCenter)
{
	m_MouseOnScreen = false;
	m_CurrentMousePosition = Window::g_Window->GetMouseVirtualCoordsRelativeToTheWindow(
		Vector2(16.f, 9.f), m_MouseOnScreen);
	if (!m_MouseOnScreen)
		return;
	m_CurrentMousePosition -= Vector2(8.f, 4.5f);

	m_CurrentMouseWorldPosition = cameraCenter + m_CurrentMousePosition;
}

void DeveloperDebugger::UpdateGetAllEntitiesAndTilesAtCoords(Map* map)
{
	m_MouseTileCoords = map->ConvertPositionToTileCoords(m_CurrentMouseWorldPosition);
	TILE_COORDS mapWidthHeight = map->GetMapWidthHeight();
	m_InfoBoxes.clear();
	if (m_MouseTileCoords.x < 0
		|| m_MouseTileCoords.y < 0
		|| m_MouseTileCoords.x >= mapWidthHeight.x
		|| m_MouseTileCoords.y >= mapWidthHeight.y)
	{
		m_MouseIsOnATile = false;
		return;
	}
	map->GetAllEntitiesAndTileInfoForDeveloperDebug(m_InfoBoxes, m_MouseTileCoords);

	m_MouseIsOnATile = true;
}

void DeveloperDebugger::UpdateModels()
{
	m_BoxHeights.clear();
	m_BoxHeights.resize(m_InfoBoxes.size(), 1.f);
	m_BoxModels.clear();
	m_BoxIndices.clear();

	int numberOfPages = 0;
	if (m_Font != nullptr)
	{
		numberOfPages = m_Font->GetPageCount();
	}

	m_TextModel.clear();
	m_TextIndices.clear();
	m_TextModel.resize(numberOfPages);
	m_TextIndices.resize(numberOfPages);
	if (m_TextMeshes.size() < (size_t)numberOfPages)
	{
		m_TextMeshes.resize(numberOfPages, nullptr);
	}

	m_BoxMaterial.SetProgram(m_shaderProg);

	if (m_WhiteBoxTex != nullptr)
	{
		if (m_BoxMaterial.SetTexture(m_WhiteBoxTex->GetSpriteSheet(), 0, "gDiffuseTex", 0) == false)
		{
			m_BoxMaterial.AddTexture(m_WhiteBoxTex->GetSpriteSheet(), 0, "gDiffuseTex");
		}
	}

	Vector2 absMousePos = Vector2(m_CurrentMousePosition.x, m_CurrentMousePosition.y);
	float fullHeight = 0.f;
	float FullWidth = m_maxBoxWidth;
	for (size_t boxIdx = 0; boxIdx < m_InfoBoxes.size(); boxIdx++)
	{
		Vector2 currentPosition = Vector2(absMousePos.x, absMousePos.y - fullHeight);
		float height = m_InfoBoxes[boxIdx].Update(m_Font, currentPosition, m_BoxModels, m_BoxIndices,
			m_TextModel, m_TextIndices, m_fontSize, m_maxBoxWidth, m_WhiteBoxTex, FullWidth);
		m_BoxHeights[boxIdx] = height;
		fullHeight += height;
	}
	Vector2 brPos = absMousePos + Vector2(FullWidth, -fullHeight);
	Vector2 amtToAdjust = Vector2::vec2_zeros;
	if (brPos.x > 8.f)
	{
		amtToAdjust.x = 8.f - brPos.x;
	}
	if (brPos.y < -4.5f)
	{
		amtToAdjust.y = -(brPos.y + 4.5f);
	}

	m_IdentityModelMatrix.MatrixMakeIdentity();
	Matrix44::MakeEulerRotation(m_IdentityModelMatrix, 0.f, 0.f, 0.f, amtToAdjust);

	m_TextMaterial.resize(numberOfPages);
	for (size_t pageIdx = 0; pageIdx < m_TextMeshes.size() && pageIdx < (size_t)numberOfPages; pageIdx++)
	{
		if (m_TextMeshes[pageIdx] == nullptr)
		{
			m_TextMeshes[pageIdx] = new Mesh();
		}
		m_TextMeshes[pageIdx]->setVBO(m_TextModel[pageIdx]);
		m_TextMeshes[pageIdx]->setIBO(m_TextIndices[pageIdx]);

		if (m_Font == nullptr)
		{
			continue;
		}
		const Texture* page = m_Font->GetPageForIdx(pageIdx);
		m_TextMaterial[pageIdx].SetProgram(m_shaderProg);
		m_TextMaterial[pageIdx].SetOrAddTexture(page, 0, "gTextureDiffuse", 0);
	}

	m_BoxMesh.setVBO(m_BoxModels);
	m_BoxMesh.setIBO(m_BoxIndices);
}

void DeveloperDebugger::Render() const
{
	if (!m_RenderThisFrame
		|| m_shaderProg == nullptr)
	{
		return;
	}

	m_BoxMaterial.SetModelMatrix(m_IdentityModelMatrix);
	m_BoxMesh.DrawWith(&m_BoxMaterial);

	if (m_Font == nullptr)
	{
		return;
	}

	for (size_t pageIdx = 0; pageIdx < (size_t)m_Font->GetPageCount() 
		&& pageIdx < m_TextMeshes.size(); pageIdx++)
	{
		if (m_TextMeshes[pageIdx] != nullptr)
		{
			m_TextMaterial[pageIdx].SetModelMatrix(m_IdentityModelMatrix);
			m_TextMeshes[pageIdx]->DrawWith(&m_TextMaterial[pageIdx]);
		}
	}

}