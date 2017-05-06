#pragma once

#ifndef DIALOGUE_HPP
#define DIALOGUE_HPP
#include "Game/Dialogue/DialogueChoices/DialogueChoice.hpp"
#include <string>

#define STATIC

class Font;
class DialogueSegment;
class Dialogue
{
private:
protected:
	//Static Variables
	static Dialogue* s_Dialogue;
	static bool s_ReadingSpeechBubbleData;

	//Variables
	DialogueSegment* m_currentDialogueSegment = nullptr;
public:
	//Constructors
	Dialogue();
	virtual ~Dialogue();

	//Updates and Render
	bool StartDialogueSegment(const std::string& dialogueSegmentCodeName);
	const DialogueChoice TriggerDialogueSegmentStep();
	void Render() const;
	bool GetIfCurrentDialogueSegmentIsFinished() const;
	void MarkCurrentDialogueSegmentFinished();

	//Getters
	DialogueSegment* GetCurrentDialogueSegment() const;

	//Static Methods
	static Dialogue* SpawnOrGetCentralDialogue();
	static void SetDefaultFont(Font* font);
	static Font* GetDefaultFont();
	static void LoadAllDialoguePiecesAtOnce();
	static bool OnUpdateLoadDialoguePieces();
	static void ClearAllDialoguePieces();
	static void ResetPlayerName();
	static void SetPlayerName(const std::string& name);
	static const std::string GetPlayerName();
};

#endif