#pragma once

#ifndef SPEECHBUBBLE_HPP
#define SPEECHBUBBLE_HPP
#include "Engine/core/EngineXMLParser.hpp"
#include "Engine/Renderer/3DModelling/MeshBuilder.hpp"
#include "Engine/2D/SpriteResource.hpp"
#include <vector>
#include <string>

struct SpeechBubbleLoadingData
{
	std::vector<std::string> m_files;
	std::string m_fileLocation = "Data/XML/SpeechBubbles/";
	double m_amountOfTimeToTakeForLoadingInSpeechBubbles = 0.25;
	int m_index = 0;
	bool m_readInFiles = false;
};

class SpeechBubble
{
private:
protected:
	//Static Variables
	static std::vector<SpeechBubble*>* s_SpeechBubbles;
	static SpeechBubbleLoadingData* s_loadingData;
	static const char* s_CommonSpeechBubblesXMLFolder;
	static const char* s_SpeechBubbleRootXML;
	static const Vector2 s_CentralBubbleAdjustmentPercentagesFromCenterOfScreen;
	static const Vector2 s_CentralChoiceBubbleAdjustmentPercentagesFromCenterOfScreen;

	//Variables
	std::vector<SpriteVert> m_vertices;
	std::vector<SpriteVert> m_choiceVertices;
	std::vector<unsigned int> m_indices;
	std::vector<unsigned int> m_choiceIndices;
	Matrix44 m_modelMatrix;
	Matrix44 m_choiceModelMatrix;
	Mesh m_mesh;
	Mesh m_choiceMesh;
	Material m_material;
	const SpriteResource* m_resource = nullptr;
	std::string m_bubbleName = "";
public:
	//Constructors
	SpeechBubble();
	SpeechBubble(const XMLNode& node);
	SpeechBubble(const SpeechBubble& other);
	virtual ~SpeechBubble();
	void BuildMesh();
	void BuildChoiceMesh();
protected:
	void AddVerticesWith(std::vector<SpriteVert>& vertices,
		const Vector2& minsCoords, const Vector2& maxsCoords, const Vector2& translation, const Vector2& size);
public:

	//Updates and Render
	void Render() const;
	void RenderChoiceBubble() const;

	//Setters
	void SetBubbleName(const std::string& name);
	void SetResource(const SpriteResource* res);

	//Getters
	const std::string& GetBubbleName() const;
	const SpriteResource* GetSpriteResource() const;
	void WriteSpeechBubbleToString(std::string& str, int indentationAmt) const;

	//Static Methods
	static void WriteAllSpeechBubblesToString(std::string& str, int indentationAmt);
	static void ReadInAllSpeechBubblesFromDefaultFolderXMLAllAtOnce();
	static bool OnUpdateReadInAllSpeechBubblesFromDefaultFolderXML();
	static void ReadInSpeechBubbleXMLNodeFromFileLocation(std::string fileStr);
	static void ClearAllSpeechBubbles();
	static const SpeechBubble* GetSpeechBubbleByName(const std::string& name);
	static bool RegisterSpeechBubbleByName(const std::string& name, SpeechBubble* bubble);
	static bool RegisterSpeechBubbleByName(const std::string& name, const SpeechBubble& bubble);
	static Vector2 GetCentralBubblePosition();
	static Vector2 GetCentralChoiceBubblePosition();
};

#endif