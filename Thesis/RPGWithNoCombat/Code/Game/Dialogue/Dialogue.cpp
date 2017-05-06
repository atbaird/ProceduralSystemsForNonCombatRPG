#include "Game/Dialogue/Dialogue.hpp"
#include "Game/Dialogue/SpeechBubble.hpp"
#include "Game/Dialogue/DialogueSegment.hpp"
#include "Game/Dialogue/TextPiece.hpp"
#include "Engine/Font/Font.hpp"

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//Static Variables
STATIC Dialogue* Dialogue::s_Dialogue = nullptr;
STATIC bool Dialogue::s_ReadingSpeechBubbleData = true;

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//Constructors
Dialogue::Dialogue()
{

}

Dialogue::~Dialogue()
{

}

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//Updates and Render
bool Dialogue::StartDialogueSegment(const std::string& dialogueSegmentCodeName)
{
	bool spawnedIt = false;
	DialogueSegment* segment = DialogueSegment::GetDialogueSegmentByName(dialogueSegmentCodeName);
	if (segment != nullptr)
	{
		segment->ResetCurrentTextSegmentToRender();
		spawnedIt = true;
	}
	m_currentDialogueSegment = segment;
	return spawnedIt;
}

const DialogueChoice Dialogue::TriggerDialogueSegmentStep()
{
	if (m_currentDialogueSegment == nullptr)
	{
		return DialogueChoice::s_EmptyChoice;
	}
	return m_currentDialogueSegment->IncrementCurrentTextSegmentToRender();
}

void Dialogue::Render() const
{
	if (m_currentDialogueSegment != nullptr && m_currentDialogueSegment->GetIsFinished() == false)
	{
		m_currentDialogueSegment->Render();
	}
}

bool Dialogue::GetIfCurrentDialogueSegmentIsFinished() const
{
	if (m_currentDialogueSegment == nullptr)
	{
		return true;
	}
	return m_currentDialogueSegment->GetIsFinished();
}

void Dialogue::MarkCurrentDialogueSegmentFinished()
{
	if (m_currentDialogueSegment != nullptr)
	{
		m_currentDialogueSegment->MarkSegmentFinished();
	}
}

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//Getters
DialogueSegment* Dialogue::GetCurrentDialogueSegment() const
{
	return m_currentDialogueSegment;
}


//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//Static Methods

STATIC Dialogue* Dialogue::SpawnOrGetCentralDialogue()
{
	if (s_Dialogue == nullptr)
	{
		s_Dialogue = new Dialogue();
	}
	return s_Dialogue;
}

STATIC void Dialogue::SetDefaultFont(Font* font)
{
	DialogueSegment::SetDefaultFont(font);
}

STATIC Font* Dialogue::GetDefaultFont()
{
	return DialogueSegment::GetDefaultFont();
}

STATIC void Dialogue::LoadAllDialoguePiecesAtOnce()
{
	SpeechBubble::ReadInAllSpeechBubblesFromDefaultFolderXMLAllAtOnce();
	DialogueSegment::ReadInAllDialogueSegmentsFromDefaultFolderAllAtOnce();
}

STATIC bool Dialogue::OnUpdateLoadDialoguePieces()
{
	if (s_ReadingSpeechBubbleData)
	{
		if (SpeechBubble::OnUpdateReadInAllSpeechBubblesFromDefaultFolderXML())
		{
			s_ReadingSpeechBubbleData = false;
		}
		return false;
	}
	else
	{
		return DialogueSegment::OnUpdateReadInAllDialogueSegmentsFromDefaultFolder();
	}
}

STATIC void Dialogue::ClearAllDialoguePieces()
{
	if (s_Dialogue != nullptr)
	{
		delete s_Dialogue;
		s_Dialogue = nullptr;
	}
	DialogueSegment::ClearAllDialogueSegments();
	SpeechBubble::ClearAllSpeechBubbles();
}

STATIC void Dialogue::ResetPlayerName()
{
	TextPiece::ResetPlayerName();
}

STATIC void Dialogue::SetPlayerName(const std::string& name)
{
	TextPiece::SetPlayerName(name);
}

STATIC const std::string Dialogue::GetPlayerName()
{
	return TextPiece::GetPlayerName();
}