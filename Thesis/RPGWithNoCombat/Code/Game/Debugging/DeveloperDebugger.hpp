#pragma once

#ifndef DEVELOPERDEBUGGER_HPP
#define DEVELOPERDEBUGGER_HPP
#include "Engine/Math/Vector2.hpp"
#include "Engine/Math/IntVector2.hpp"
#include "Game/Debugging/DeveloperDebugInfoBox.hpp"
#include "Engine/Renderer/3DModelling/Material.hpp"
#include "Engine/Renderer/3DModelling/Mesh.hpp"
#include <vector>

#define STATIC
#define TILE_COORDS IntVector2
class Font;
class ShaderProgram;
class Texture;
class Map;
class Matrix44;
struct Vertex_PCT;
class DeveloperDebugger
{
private:
protected:
	//Variables
	std::vector<std::vector<Vertex_PCT>> m_TextModel;
	std::vector<std::vector<unsigned int>> m_TextIndices;
	std::vector<Material> m_TextMaterial;
	std::vector<DeveloperDebugInfoBox> m_InfoBoxes;
	std::vector<Vertex_PCT> m_BoxModels;
	std::vector<float> m_BoxHeights;
	std::vector<unsigned int> m_BoxIndices;
	std::vector<Mesh*> m_TextMeshes;

	Matrix44 m_IdentityModelMatrix;
	Mesh m_BoxMesh;
	Material m_BoxMaterial;
	Vector2 m_CurrentMousePosition = Vector2::vec2_zeros;
	Vector2 m_CurrentMouseWorldPosition = Vector2::vec2_zeros;
	float m_fontSize = 0.01f;
	float m_maxBoxWidth = 4.5f;
	TILE_COORDS m_MouseTileCoords = TILE_COORDS::ivec2_zeros;
	Font* m_Font = nullptr;
	ShaderProgram* m_shaderProg = nullptr;
	const SpriteResource* m_WhiteBoxTex = nullptr;
	bool m_MouseOnScreen = false;
	bool m_AllowedToRender = false;
	bool m_RenderThisFrame = false;
	bool m_MouseIsOnATile = false;

public:
	//Static Variables
	static DeveloperDebugger* g_DeveloperDebugger;

	//Constructors
	DeveloperDebugger(float fontSize = 0.01f, Font* font = nullptr, 
		ShaderProgram* prog = nullptr, const SpriteResource* whiteBoxTex = nullptr);
	virtual ~DeveloperDebugger();

	//Setters
	void ToggleRenderAllowed() { m_AllowedToRender = !m_AllowedToRender; };

	//Updates and Render
	void Update(Map* map, bool currentlyLoadingAMap, const Vector2& cameraCenter);
protected:
	void UpdateMousePosition(const Vector2& cameraCenter);
	void UpdateGetAllEntitiesAndTilesAtCoords(Map* map);
	void UpdateModels();
public:
	void Render() const;
};
#endif