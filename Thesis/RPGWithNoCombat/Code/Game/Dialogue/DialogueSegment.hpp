#pragma once

#ifndef DIALOGUESEGMENT_HPP
#define DIALOGUESEGMENT_HPP
#include "Game/Dialogue/SpeechBubble.hpp"
#include "Game/Dialogue/TextPiece.hpp"
#include "Engine/Font/Font.hpp"
#include "Game/Dialogue/DialogueGroupings.hpp"
#include "Game/Dialogue/DialogueChoices/DialogueChoice.hpp"

struct DialogueSegmentAndGroupingLoadData
{
	std::vector<std::string> m_dialogueSegmentFiles;
	std::vector<std::string> m_dialogueGroupingFiles;
	std::string m_commonDialogueSegmentsXMLFile = "Data/XML/DialogueSegments/";
	std::string m_commonDialogueGroupingsXMLFile = "Data/XML/DialogueGroupings/";
	std::string m_dialogueSegmentsRootXMLName = "DialogueSegments";
	std::string m_dialogueGroupingsRootXMLName = "DialogueGroupings";
	int m_index = 0;
	double m_amountOfTimeToTakeReadingDialogueSegmentsPerFrame = 0.25;
	double m_amountOfTimeToTakeReadingDialogueGroupingsPerFrame = 0.25;
	bool m_readingDialogueGroupings = true;
	bool m_readDialogueGroupingFiles = false;
	bool m_readDialogueSegmentFiles = false;
};

class DialogueRequirement;
class DialogueSegment
{
private:
protected:
	//Static Variables
	static Matrix44 s_UpArrowMatrix;
	static Matrix44 s_DownArrowMatrix;
	static Matrix44 s_SelectionMatrix;
	static Mesh s_UpArrowMesh;
	static Mesh s_DownArrowMesh;
	static Mesh s_SelectionMesh;
	static const Vector2 s_UpArrowChoicePosition;
	static Vector2 s_SelectionPosition;
	static const Vector2 s_Choice1ArrowChoicePosition;
	static const Vector2 s_Choice2ArrowChoicePosition;
	static const Vector2 s_Choice3ArrowChoicePosition;
	static const Vector2 s_DownArrowChoicePosition;
	static constexpr float s_DialogueSegmentFontSize = 0.013f;
	static DialogueSegmentAndGroupingLoadData* s_loadingData;
	static const SpeechBubble* s_speechBubble;
	static Font* s_defaultFont;
	static std::vector<DialogueGroupings>* s_SpecialDialogueGroupings;
	static std::vector<DialogueSegment>* s_DialogueSegments;
	static Material* s_UpArrowMaterial;
	static Material* s_DownArrowMaterial;
	static Material* s_SelectionMaterial;
	static ShaderProgram* s_TextShader;
	static int s_MaxCharactersPerLine;
	static int s_MaxLinesPerDialogueBox;
	static constexpr int s_ChoicesPerPage = 3;

	//variables
	Matrix44 m_generalMatrix;
	std::vector<Material> m_textMats;
	std::vector<Mesh*> m_textMeshes;
	std::string m_DialogueSegmentName = "";
	std::string m_FullUnparsedText = "";
	std::string m_CurrentTextForFont = "";
	std::string m_SpecialGrouping = "";
	std::string m_lowerCaseGroupingName = "";
	std::string m_dialogueSegmentToOpenNext = "";
	Rgba m_fontColor = Rgba(0.f, 0.f, 0.f, 1.f);
	const SpeechBubble* m_speechBubble = nullptr;
	std::vector<TextPiece> m_dialoguePiecesPerThisDialogueSegment; 
	std::vector<std::string> m_textLinesInGroupsOfLinesPerPage;
	std::vector<std::vector<Vertex_PCT>> m_vertices;
	std::vector<std::vector<unsigned int>> m_indices;
	std::vector<DialogueChoice> m_dialogueChoices;
	std::vector<DialogueRequirement*> m_DialogueRequirementsForIfInADialogueGroup;
	std::vector<DialogueTrigger*> m_actionsForWhenDialogueClosed;
	std::vector<int> m_ActiveDialogueChoices;
	Font* m_font = nullptr;
	int m_CurrentTextSegmentToRender = 0;
	int m_currentPlayerSelection = 0;
	int m_choicePage = 0;
	int m_numberOfChoicePages = 0;
	int m_numberOfActiveChoicePages = 0;
	bool m_renderChoices = false;
	bool m_finished = false;
public:
	//Constructors
	DialogueSegment();
	DialogueSegment(const XMLNode& node);
	DialogueSegment(const DialogueSegment& other);
	virtual ~DialogueSegment();

protected:
	void ReadXMLAttributes(const XMLNode& node);
	void ReadXMLChildren(const XMLNode& node);
	void AddXMLChoices(const XMLNode& node);
	void AddXMLRequirementsForWhenInGroup(const XMLNode& node);
	void ReadChildForDialogueActionsWhenDialogueCloses(const XMLNode& child);
public:

	//Updates and Render
	void Render() const;

	//Operations
	void TriggerClosingActions();
	void MoveSelectionUp();
	void MoveSelectionDown();
	void ResetCurrentTextSegmentToRender();
	void RecalculateAvailableDialogueChoices();
	const DialogueChoice IncrementCurrentTextSegmentToRender();
protected:
	const DialogueChoice UpdateMeshWithCurrentTextSegment();
	void SetPositionOfSelected();
public:

	//Setters
	void MarkSegmentFinished();
	void SetDialogueSegmentName(const std::string& dialogueSegmentName);
	void SetFullUnparsedTextAndParseIt(const std::string& fullUnparsedText);
	void SetSpeechBubble(const SpeechBubble* bubble);
	void ParseText();
protected:
	void LoadInAPiece(std::string& currentWord, std::string& currentLine, size_t& charIdx,
		int& lineCnt, unsigned char& car, unsigned char& nextCar, bool hollahDollahModeCurrentlyOn);
public:

	//Getters
	const std::string& GetDialogueToOpenNext() const;
	const std::string& GetDialogueSegmentName() const;
	const std::string& GetFullUnparsedText() const;
	const SpeechBubble* GetSpeechBubble() const;
	const bool GetIsFinished() const;
	bool GetHasDialogueChoice() const;
	int GetIndexOfChoice(int index) const;
	int GetIndexOfActiveChoice(int index) const;
	int GetIndexOfChoiceWithPage(int index, int page) const;
	int GetIndexOfActiveChoiceWithPage(int index, int page) const;
	bool DetermineDialogueRequirementsMet() const;
	void WriteDialogueSegmentToString(std::string& str, int indentationAmt) const;

	//Static Methods
	static void WriteAllDialogueGroupsToString(std::string& str, int indentationAmt);
	static void WriteAllDialogueSegmentsToString(std::string& str, int indentationAmt);
	static void ReadInAllDialogueSegmentsFromDefaultFolderAllAtOnce();
	static bool OnUpdateReadInAllDialogueSegmentsFromDefaultFolder();
protected:
	static bool ActuallyOnUpdateReadInAllDialogueSegmentsFromDefaultFolder();
	static void ReadDialogueSegmentFromXMLFileLocation(const std::string& fileStr,
		const std::string& dialogueSegmentsRootXMLName);
	static void ReadInAllDialogueGroupingsFromDefaultFolder();
	static bool OnUpdateReadInAllDialogueGroupingsFromDefaultFolder();
	static void ReadDialogueGroupFromXMLFileLocation(const std::string& fileStr,
		const std::string& dialogueGroupingsRootXMLName);
public:
	static void ClearAllDialogueSegments();
	static void SetDefaultFont(Font* font);
	static Font* GetDefaultFont();
	static DialogueSegment* GetDialogueSegmentByName(const std::string& name);
	static bool RegisterDialogueSegmentByName(const std::string& name, DialogueSegment& segment);
	static bool RegisterDialogueSegmentNameIntoSpecialGrouping(const std::string& groupingName,
		const std::string& DialogueSegmentName);
	
protected:
	static void AddArrowQuad(std::vector<SpriteVert>& vertices, const AABB2& textureCoords,
		const Vector2& size = Vector2(1.f,1.f));
	static void AddArrowIndices(std::vector<unsigned int>& indices);
	static void RecalcSelectionMatrix();
public:
	static void SetUpCommonMeshes();
	static void SetDefaultTextShader(ShaderProgram* prog);
	static ShaderProgram* GetTextShaderProgram();

};

#endif