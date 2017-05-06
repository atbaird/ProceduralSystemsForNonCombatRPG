#pragma once

#ifndef TEXTPIECE_HPP
#define TEXTPIECE_HPP
#include <string>
#include <vector>
#define STATIC

enum TextProceededBy
{
	NOTHING,
	SPACE,
	NEW_LINE
};

class Agent;
struct TextPiece
{
protected:
	//Static Variables
	static const std::string s_PlayerNameKey;
	static const std::string s_JobTitleNameKey;
	static const std::string s_MyNameKey;
	static const std::string s_FirstNameKey;
	static const std::string s_SecondNameKey;
	static const std::string s_LastNameKey;
	static const std::string s_MapName;
	static const std::string s_GlobalVariableKey;

	static std::string s_playerName;
	static bool s_AddThreeDotsAtSpeechBubbleEnd;

	//Variables
	std::string m_value = "";
	std::string m_questTitleThatVariableIsReferencing = "";
	std::string m_nameOfQuestVariable = "";
	std::string m_nameOfVariableOffQuestVariableWanted = "";
	TextProceededBy m_proceededBySpaceElseNewLine = NOTHING;
	bool m_replaceWithPlayerName = false;
	bool m_replaceWithJobTitle = false;
	bool m_replaceWithFirstName = false;
	bool m_replaceWithSecondName = false;
	bool m_replaceWithFullName = false;
	bool m_replaceWithLastName = false;
	bool m_replaceWithMapName = false;
	bool m_hasACommaAtEnd = false;
	bool m_hasAPeriodAtEnd = false;
	bool m_checkQuestsForVariableVal = false;
	bool m_checkGlobalVariablesForTextValue = false;
	bool m_checkNPCAttributeValue = false;

public:
	//Constructors
	TextPiece(const std::string& val = "");

	//Setters
	void ParseTextForQuestVariableInfo();
	void SetTextProceededBy(TextProceededBy proceededBy);
	void SetQuestReferencing(const std::string& questReferencing);
	void SetNameofQuestVariable(const std::string& questVariable);
	void SetNameOfVariableOffQuestVariableWanted(const std::string& variableName);
	void SetReplaceWithPlayerName(bool replaceWithPlayerName);
	void SetReplaceWithJobTitle(bool replaceWithJobTitle);
	void SetReplaceWithFirstName(bool replaceWithFirstName);
	void SetReplaceWithSecondName(bool replaceWithSecondName);
	void SetReplaceWithLastName(bool replaceWithLastName);
	void SetReplaceWithMapName(bool replaceWithMapName);
	void SetHasACommaAtEnd(bool hasACommaAtEnd);
	void SetHasAPeriodAtEnd(bool hasAPeriodAtEnd);

	//Getters
	const std::string TryToGetValueOutOfQuest(bool& handleCasingAsPlease) const;
	const std::string& GetTrueValue() const;
	const std::string GetValue() const;
	const std::string& GetNameOfQuestReferencing() const;
	const std::string& GetNameOfQuestVariableReferencing() const;
	const std::string& GetNameOfVariableOffQuestVariableWanted() const;
	TextProceededBy GetTextProceededBy() const;
	bool GetReplaceWithPlayerName() const;
	bool GetReplaceWithJobTitle() const;
	bool GetReplaceWithFirstName() const;
	bool GetReplaceWithSecondName() const;
	bool GetReplaceWithLastName() const;
	bool GetReplaceWithMapName() const;
	size_t GetExpectedStringLength() const;

	//Static Methods
	static void SetAddThreeDotsAtTheEndOfDialogueSegment(bool set);
	static void ResetPlayerName();
	static void SetPlayerName(const std::string& name);
	static const std::string GetPlayerName();
	static std::string CompileStringFromGroupOfTextPieces(const std::vector<TextPiece>& textPieces, 
		int MaxCharactersPerLine,
		int MaxLinesPerTextBlurb,
		std::vector<std::string>& textLinesInGroupsOfLinesPerPage);
protected:
	static void HandlePuttingTextPiecesTogether(size_t idx, const TextPiece& piece,
		const std::vector<TextPiece>& textPieces,
		std::string& currentStr, std::string& fullCurrentStr,
		std::string& currentLine, std::string& out_val,
		int& currentLineIdx,
		int MaxCharactersPerLine,
		int MaxLinesPerTextBlurb,
		std::vector<std::string>& textLinesInGroupsOfLinesPerPage,
		bool& periodExistedLastWord,
		bool& handleCasingAsPleaseForThisPiece);
	static void HandleAlteringTextPiece(bool handleCasingAsPleaseForThisPiece,
		bool periodExistedLastWord,
		std::string& currentStr,
		std::string& fullCurrentStr);
	static bool TestPeriodEncounteredLastPiece(const std::string& fullCurrentStr, 
		std::string& lastChar,
		std::string& lastTwoChars);
	static bool HandleHowToParseText(std::string& currentStr,
		const TextPiece& piece,
		const Agent* agentPlayerTalkingTo, const Agent* playerAgent,
		bool& handleAsPlease,
		std::string& fullCurrentStr,
		std::string& currentLine,
		std::string& out_val,
		int& currentLineIdx,
		int MaxCharactersPerLine,
		int MaxLinesPerTextBlurb,
		std::vector<std::string>& textLinesInGroupsOfLinesPerPage);
	static void CompileStringFromGroupOfTextPiecesFinalize(std::string& out_val,
		std::string& fullCurrentStr,
		std::string& currentStr,
		std::string& currentLine,
		int MaxCharactersPerLine,
		int MaxLinesPerTextBlurb,
		int& currentLineIdx,
		std::vector<std::string>& textLinesInGroupsOfLinesPerPage);

};
#endif