#include "Game/Dialogue/TextPiece.hpp"
#include "Game/GameEntities/Agents/Agent.hpp"
#include "Game/Map/Map.hpp"
#include "Game/GameEntities/Agents/Player/Player.hpp"
#include "Game/GameEntities/Agents/NPCs/NPC.hpp"
#include "Game/Map/Map.hpp"


//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
//Static Variables
STATIC const std::string TextPiece::s_PlayerNameKey = "$playername$";
STATIC const std::string TextPiece::s_JobTitleNameKey = "$job$";
STATIC const std::string TextPiece::s_MyNameKey = "$myname$";
STATIC const std::string TextPiece::s_FirstNameKey = "$myfirstname$";
STATIC const std::string TextPiece::s_SecondNameKey = "$mysecondname$";
STATIC const std::string TextPiece::s_LastNameKey = "$mylastname$";
STATIC const std::string TextPiece::s_MapName = "$mapname$";
STATIC const std::string TextPiece::s_GlobalVariableKey = "globalvariable";

STATIC std::string TextPiece::s_playerName = "Player";
STATIC bool TextPiece::s_AddThreeDotsAtSpeechBubbleEnd;

//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
//Constructors
TextPiece::TextPiece(const std::string& val)
	: m_value(val)
{
	std::string lowerCaseValue = MakeLower(val);
	if (m_value.length() == 0)
	{
		return;
	}
	unsigned char lastChar = lowerCaseValue.at(m_value.length() - 1);
	if (lastChar == '.')
	{
		m_hasAPeriodAtEnd = true;
		m_value = m_value.substr(0, m_value.length() - 1);
	}
	else if (lastChar == ',')
	{
		m_hasACommaAtEnd = true;
		m_value = m_value.substr(0, m_value.length() - 1);
	}

	if (lowerCaseValue == s_PlayerNameKey)
	{
		m_replaceWithPlayerName = true;
	}
	else if (lowerCaseValue == s_JobTitleNameKey)
	{
		m_replaceWithJobTitle = true;
	}
	else if (lowerCaseValue == s_FirstNameKey)
	{
		m_replaceWithFirstName = true;
	}
	else if (lowerCaseValue == s_SecondNameKey)
	{
		m_replaceWithSecondName = true;
	}
	else if (lowerCaseValue == s_MyNameKey)
	{
		m_replaceWithFullName = true;
	}
	else if (lowerCaseValue == s_MapName)
	{
		m_replaceWithMapName = true;
	}
	else if (lowerCaseValue == s_LastNameKey)
	{
		m_replaceWithLastName = true;
	}
	else
	{
		ParseTextForQuestVariableInfo();
	}

}

//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//Setters
void TextPiece::ParseTextForQuestVariableInfo()
{
	m_checkQuestsForVariableVal = false;
	if (strcmp(m_value.substr(0, 1).c_str(), "$") != 0
		|| strcmp(m_value.substr(m_value.size() - 1, 1).c_str(), "$") != 0)
	{
		return;
	}

	std::string val = m_value.substr(1, m_value.size() - 1);
	val = val.substr(0, val.size() - 1);
	std::vector<std::string> parsed = ParseString(val, '.');
	if (parsed.size() == 2)
	{
		m_questTitleThatVariableIsReferencing = MakeLower(parsed[0]);
		m_nameOfQuestVariable = MakeLower(parsed[1]);

		if (m_questTitleThatVariableIsReferencing == s_GlobalVariableKey)
		{
			m_checkGlobalVariablesForTextValue = true;
		}
		else if (m_questTitleThatVariableIsReferencing == "npcattribute"
			|| m_questTitleThatVariableIsReferencing == "npcattributes")
		{
			m_checkNPCAttributeValue = true;
		}
		else
		{
			m_checkQuestsForVariableVal = true;
		}
	}
	else if (parsed.size() >= 3)
	{
		m_questTitleThatVariableIsReferencing = MakeLower(parsed[0]);
		m_nameOfQuestVariable = MakeLower(parsed[1]);
		m_nameOfVariableOffQuestVariableWanted = MakeLower(parsed[2]);
		m_checkQuestsForVariableVal = true;
	}

}

void TextPiece::SetTextProceededBy(TextProceededBy proceededBy)
{
	m_proceededBySpaceElseNewLine = proceededBy;
}

void TextPiece::SetQuestReferencing(const std::string& questReferencing)
{
	m_questTitleThatVariableIsReferencing = questReferencing;
}

void TextPiece::SetNameofQuestVariable(const std::string& questVariable)
{
	m_nameOfQuestVariable = questVariable;
}

void TextPiece::SetNameOfVariableOffQuestVariableWanted(const std::string& variableName)
{
	m_nameOfVariableOffQuestVariableWanted = variableName;
}

void TextPiece::SetReplaceWithPlayerName(bool replaceWithPlayerName)
{
	m_replaceWithPlayerName = replaceWithPlayerName;
}

void TextPiece::SetReplaceWithJobTitle(bool replaceWithJobTitle)
{
	m_replaceWithJobTitle = replaceWithJobTitle;
}

void TextPiece::SetReplaceWithFirstName(bool replaceWithFirstName)
{
	m_replaceWithFirstName = replaceWithFirstName;
}

void TextPiece::SetReplaceWithSecondName(bool replaceWithSecondName)
{
	m_replaceWithSecondName = replaceWithSecondName;
}

void TextPiece::SetReplaceWithLastName(bool replaceWithLastName)
{
	m_replaceWithLastName = replaceWithLastName;
}

void TextPiece::SetReplaceWithMapName(bool replaceWithMapName)
{
	m_replaceWithMapName = replaceWithMapName;
}

void TextPiece::SetHasACommaAtEnd(bool hasACommaAtEnd)
{
	m_hasACommaAtEnd = hasACommaAtEnd;
}

void TextPiece::SetHasAPeriodAtEnd(bool hasAPeriodAtEnd)
{
	m_hasAPeriodAtEnd = hasAPeriodAtEnd;
}

//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//Getters
const std::string TextPiece::TryToGetValueOutOfQuest(bool& handleCasingAsPlease) const
{
	if (!m_checkQuestsForVariableVal
		|| m_nameOfQuestVariable == "")
	{
		return "";
	}

	const BaseGameEntity* entityPlayer = BaseGameEntity::GetPointerToPlayer();
	const Player* player = dynamic_cast<const Player*>(entityPlayer);
	const Map* map = Map::GetMap();
	if (player == nullptr
		|| map == nullptr)
	{
		return "";
	}
	const PlayerQuestManager* questManager = player->GetPlayerQuestManagerPtr();
	std::string value = "";
	if (m_questTitleThatVariableIsReferencing == "")
	{
		//Check all quests

		for (size_t questIdx = 0; questIdx < questManager->GetNumberOfHeldCompletedQuests(); questIdx++)
		{
			const Quest* quest = questManager->GetCompletedQuestAtIndex(questIdx);
			if (quest == nullptr)
				continue;

			value = quest->GetQuestVariableNameValue(m_nameOfQuestVariable,
				m_nameOfVariableOffQuestVariableWanted, handleCasingAsPlease);
			if (value != "")
			{
				return value;
			}
		}

		for (size_t questIdx = 0; questIdx < questManager->GetNumberOfHeldQuests(); questIdx++)
		{
			const Quest* quest = questManager->GetQuestAtIndex(questIdx);
			if(quest == nullptr)
				continue;

			value = quest->GetQuestVariableNameValue(m_nameOfQuestVariable, 
				m_nameOfVariableOffQuestVariableWanted, handleCasingAsPlease);
			if (value != "")
			{
				return value;
			}
		}

		const std::vector<Quest*>* mapQuests = map->GetQuestsPtr();
		for (size_t questIdx = 0; questIdx < mapQuests->size(); questIdx++)
		{
			const Quest* quest = mapQuests->at(questIdx);
			if (quest == nullptr)
				continue;

			value = quest->GetQuestVariableNameValue(m_nameOfQuestVariable,
				m_nameOfVariableOffQuestVariableWanted, handleCasingAsPlease);
			if (value != "")
			{
				return value;
			}
		}
	}
	else
	{
		//Check specific quest
		for (size_t questIdx = 0; questIdx < questManager->GetNumberOfHeldCompletedQuests(); questIdx++)
		{
			const Quest* quest = questManager->GetCompletedQuestAtIndex(questIdx);
			if (quest == nullptr
				|| quest->GetLowerCaseQuestTitle() != m_questTitleThatVariableIsReferencing)
				continue;

			value = quest->GetQuestVariableNameValue(m_nameOfQuestVariable,
				m_nameOfVariableOffQuestVariableWanted, handleCasingAsPlease);
			return value;
		}

		for (size_t questIdx = 0; questIdx < questManager->GetNumberOfHeldQuests(); questIdx++)
		{
			const Quest* quest = questManager->GetQuestAtIndex(questIdx);
			if (quest == nullptr
				|| quest->GetLowerCaseQuestTitle() != m_questTitleThatVariableIsReferencing)
				continue;

			value = quest->GetQuestVariableNameValue(m_nameOfQuestVariable,
				m_nameOfVariableOffQuestVariableWanted, handleCasingAsPlease);
				return value;
		}

		const std::vector<Quest*>* mapQuests = map->GetQuestsPtr();
		for (size_t questIdx = 0; questIdx < mapQuests->size(); questIdx++)
		{
			const Quest* quest = mapQuests->at(questIdx);
			if (quest == nullptr
				|| quest->GetLowerCaseQuestTitle() != m_questTitleThatVariableIsReferencing)
				continue;

			value = quest->GetQuestVariableNameValue(m_nameOfQuestVariable,
				m_nameOfVariableOffQuestVariableWanted, handleCasingAsPlease);
			return value;
		}
	}
	return value;
}

const std::string& TextPiece::GetTrueValue() const
{
	return m_value;
}

const std::string TextPiece::GetValue() const
{
	if (m_replaceWithPlayerName)
	{
		return s_playerName;
	}

	return m_value;
}

const std::string& TextPiece::GetNameOfQuestReferencing() const
{
	return m_questTitleThatVariableIsReferencing;
}

const std::string& TextPiece::GetNameOfQuestVariableReferencing() const
{
	return m_nameOfQuestVariable;
}

const std::string& TextPiece::GetNameOfVariableOffQuestVariableWanted() const
{
	return m_nameOfVariableOffQuestVariableWanted;
}

TextProceededBy TextPiece::GetTextProceededBy() const
{
	return m_proceededBySpaceElseNewLine;
}

bool TextPiece::GetReplaceWithPlayerName() const
{
	return m_replaceWithPlayerName;
}

bool TextPiece::GetReplaceWithJobTitle() const
{
	return m_replaceWithJobTitle;
}

bool TextPiece::GetReplaceWithFirstName() const
{
	return m_replaceWithFirstName;
}

bool TextPiece::GetReplaceWithSecondName() const
{
	return m_replaceWithSecondName;
}

bool TextPiece::GetReplaceWithLastName() const
{
	return m_replaceWithLastName;
}

bool TextPiece::GetReplaceWithMapName() const
{
	return m_replaceWithMapName;
}

size_t TextPiece::GetExpectedStringLength() const
{
	return GetValue().length();
}

//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//Static Methods
STATIC void TextPiece::SetAddThreeDotsAtTheEndOfDialogueSegment(bool set)
{
	s_AddThreeDotsAtSpeechBubbleEnd = set;
}

STATIC void TextPiece::ResetPlayerName()
{
	s_playerName = "Player";
}

STATIC void TextPiece::SetPlayerName(const std::string& name)
{
	s_playerName = name;
}

STATIC const std::string TextPiece::GetPlayerName()
{
	return s_playerName;
}

STATIC std::string TextPiece::CompileStringFromGroupOfTextPieces(const std::vector<TextPiece>& textPieces,
	int MaxCharactersPerLine,
	int MaxLinesPerTextBlurb,
	std::vector<std::string>& textLinesInGroupsOfLinesPerPage)
{
	textLinesInGroupsOfLinesPerPage.clear();
	std::string out_val = "";
	std::string currentLine = "";
	std::string currentStr = "";
	std::string fullCurrentStr = "";
	const BaseGameEntity* playerEntity = BaseGameEntity::GetPointerToPlayer();
	const Agent* playerAgent = dynamic_cast<const Agent*>(playerEntity);
	const Agent* agentPlayerTalkingTo = nullptr;
	int currentLineIdx = 0;
	if (playerAgent != nullptr)
	{
		agentPlayerTalkingTo = playerAgent->GetTalkingTo();
	}

	bool periodExistedLastWord = true;

	bool handleCasingAsPleaseForThisPiece = true;
	for (size_t idx = 0; idx < textPieces.size(); idx++)
	{
		handleCasingAsPleaseForThisPiece = true;
		const TextPiece& piece = textPieces[idx];
		if (HandleHowToParseText(currentStr, piece,
			agentPlayerTalkingTo, playerAgent,  handleCasingAsPleaseForThisPiece,
			fullCurrentStr, currentLine, out_val, currentLineIdx,
			MaxCharactersPerLine, MaxLinesPerTextBlurb,
			textLinesInGroupsOfLinesPerPage))
		{
			continue;
		}

		HandlePuttingTextPiecesTogether(idx, piece, textPieces,
			currentStr, fullCurrentStr, currentLine, out_val,
			currentLineIdx, MaxCharactersPerLine, MaxLinesPerTextBlurb,
			textLinesInGroupsOfLinesPerPage, periodExistedLastWord, handleCasingAsPleaseForThisPiece);
	}
	HandleAlteringTextPiece(handleCasingAsPleaseForThisPiece, periodExistedLastWord,
		currentStr, fullCurrentStr);
	CompileStringFromGroupOfTextPiecesFinalize(out_val, fullCurrentStr, currentStr, currentLine,
		MaxCharactersPerLine, MaxLinesPerTextBlurb, currentLineIdx, textLinesInGroupsOfLinesPerPage);

	return out_val;
}

STATIC void TextPiece::HandleAlteringTextPiece(bool handleCasingAsPleaseForThisPiece,
	bool periodExistedLastWord,
	std::string& currentStr,
	std::string& fullCurrentStr)
{
	if (handleCasingAsPleaseForThisPiece)
	{
		if (currentStr == "I"
			|| currentStr == "i")
		{
			currentStr = MakeUpper(currentStr);
		}
		else
		{
			currentStr = MakeLower(currentStr);
		}
		if (periodExistedLastWord && fullCurrentStr.length() == 0)
		{
			std::string allLetters = currentStr;
			currentStr = (MakeUpper(allLetters.substr(0, 1)));
			if (allLetters.length() > 0)
			{
				currentStr += (allLetters.substr(1, allLetters.length() - 1));
			}
		}
	}
}

STATIC void TextPiece::HandlePuttingTextPiecesTogether(size_t idx, const TextPiece& piece,
	const std::vector<TextPiece>& textPieces,
	std::string& currentStr, std::string& fullCurrentStr,
	std::string& currentLine, std::string& out_val,
	int& currentLineIdx,
	int MaxCharactersPerLine,
	int MaxLinesPerTextBlurb,
	std::vector<std::string>& textLinesInGroupsOfLinesPerPage,
	bool& periodExistedLastWord,
	bool& handleCasingAsPleaseForThisPiece)
{
	if (idx != (textPieces.size() - 1))
	{
		HandleAlteringTextPiece(handleCasingAsPleaseForThisPiece, periodExistedLastWord,
			currentStr, fullCurrentStr);
		fullCurrentStr += currentStr;
		if (piece.m_proceededBySpaceElseNewLine == NOTHING)
		{
		}
		else if (piece.m_proceededBySpaceElseNewLine == NEW_LINE)
		{
			out_val += currentLine + fullCurrentStr;
			currentLineIdx++;
			if (currentLineIdx >= MaxLinesPerTextBlurb)
			{
				if (s_AddThreeDotsAtSpeechBubbleEnd)
				{
					out_val += "...";
				}
				textLinesInGroupsOfLinesPerPage.push_back(out_val);
				out_val = "";
				currentLineIdx = 0;
			}
			currentLine = "";

			std::string lastChar = "";
			std::string lastTwoChars = "";

			periodExistedLastWord = TestPeriodEncounteredLastPiece(fullCurrentStr, lastChar, lastTwoChars);
			fullCurrentStr = "";
			out_val += "\n";
		}
		else if (piece.m_proceededBySpaceElseNewLine == SPACE)
		{
			if (currentLine.length() + fullCurrentStr.length() + 1 <= (size_t)MaxCharactersPerLine)
			{
				fullCurrentStr += " ";
				currentLine += fullCurrentStr;
			}
			else
			{
				{
					out_val += currentLine + "\n";
					fullCurrentStr += " ";
				}
				currentLineIdx++;
				if (currentLineIdx >= MaxLinesPerTextBlurb)
				{
					if (s_AddThreeDotsAtSpeechBubbleEnd)
					{
						out_val += "...";
					}
					textLinesInGroupsOfLinesPerPage.push_back(out_val);
					out_val = "";
					currentLineIdx = 0;
				}
				currentLine = fullCurrentStr;
			}
			std::string lastChar = "";// currentStr.substr(currentStr.length() - 1, 1);
			std::string lastTwoChars = "";// currentStr.substr(currentStr.length() - 2, 2);
			periodExistedLastWord = TestPeriodEncounteredLastPiece(fullCurrentStr, lastChar, lastTwoChars);
			fullCurrentStr = "";
		}
		currentStr = "";
	}
}

STATIC bool TextPiece::TestPeriodEncounteredLastPiece(const std::string& fullCurrentStr,
	std::string& lastChar, 
	std::string& lastTwoChars)
{
	if (fullCurrentStr.length() > 0)
	{
		lastChar = fullCurrentStr.substr(fullCurrentStr.length() - 1, 1);
		if (fullCurrentStr.length() > 1)
		{
			lastTwoChars = fullCurrentStr.substr(fullCurrentStr.length() - 2, 2);
		}
	}
	if (fullCurrentStr.length() > 0
		&& (lastChar == "." || lastChar == "!" || lastChar == "?"
			|| lastTwoChars == ". " || lastTwoChars == "! " || lastTwoChars == "? "
			|| lastTwoChars == ".\n" || lastTwoChars == "!\n" || lastTwoChars == "?\n"))
	{
		return true;
	}
	else
	{
		return false;
	}
}

STATIC bool TextPiece::HandleHowToParseText(std::string& currentStr, 
	const TextPiece& piece,
	const Agent* agentPlayerTalkingTo, const Agent* playerAgent,
	bool& handleCasingAsPleaseForThisPiece,
	std::string& fullCurrentStr,
	std::string& currentLine,
	std::string& out_val,
	int& currentLineIdx,
	int MaxCharactersPerLine,
	int MaxLinesPerTextBlurb,
	std::vector<std::string>& textLinesInGroupsOfLinesPerPage)
{
	playerAgent;
	handleCasingAsPleaseForThisPiece = true;
	if (piece.m_replaceWithPlayerName)
	{
		currentStr = s_playerName;
		handleCasingAsPleaseForThisPiece = false;
	}
	else if (piece.m_replaceWithFirstName)
	{
		if (agentPlayerTalkingTo != nullptr)
		{
			currentStr += agentPlayerTalkingTo->GetFirstName();
			handleCasingAsPleaseForThisPiece = false;
		}
		else
		{
			currentStr += piece.m_value;
		}
	}
	else if (piece.m_replaceWithSecondName)
	{
		if (agentPlayerTalkingTo != nullptr)
		{
			currentStr += agentPlayerTalkingTo->GetSecondName();
			handleCasingAsPleaseForThisPiece = false;
		}
		else
		{
			currentStr += piece.m_value;
		}
	}
	else if (piece.m_replaceWithFullName)
	{
		if (agentPlayerTalkingTo != nullptr)
		{
			std::string name = agentPlayerTalkingTo->GetName();
			std::vector<std::string> parsedName = ParseString(name, ' ');
			for (size_t i = 0; i < parsedName.size(); i++)
			{
				std::string nameVal = parsedName[i];
				currentStr += nameVal;
				if (i == parsedName.size())
				{
					break;

				}
				fullCurrentStr += currentStr;
				currentStr = "";
				if (i != parsedName.size() - 1)
				{
					fullCurrentStr += " ";
				}
				
				if (currentLine.length() + fullCurrentStr.length() <= (size_t)MaxCharactersPerLine)
				{
					currentLine += fullCurrentStr;
				}
				else
				{
					currentLineIdx++;
					if (currentLineIdx >= MaxLinesPerTextBlurb)
					{
						if (s_AddThreeDotsAtSpeechBubbleEnd)
						{
							out_val += currentLine + "...\n";
						}
						else
						{
							out_val += currentLine + "\n";
						}
						textLinesInGroupsOfLinesPerPage.push_back(out_val);
						out_val = "";
						currentLineIdx = 0;
					}
					else
					{
						out_val += currentLine + "\n";
					}
					currentLine = fullCurrentStr;
				}
				std::string lastChar = "";// currentStr.substr(currentStr.length() - 1, 1);
				std::string lastTwoChars = "";// currentStr.substr(currentStr.length() - 2, 2);
				fullCurrentStr = "";
			}
			handleCasingAsPleaseForThisPiece = false;
		}
		else
		{
			currentStr += piece.m_value;
		}
	}
	else if (piece.m_replaceWithLastName)
	{
		if (agentPlayerTalkingTo != nullptr)
		{
			currentStr += agentPlayerTalkingTo->GetLastName();
			handleCasingAsPleaseForThisPiece = false;
		}
		else
		{
			currentStr += piece.m_value;
		}
	}
	else if (piece.m_replaceWithJobTitle)
	{
		if (agentPlayerTalkingTo != nullptr)
		{
			currentStr += agentPlayerTalkingTo->GetJobTitle();
			//handleCasingAsPleaseForThisPiece = false;
		}
		else
		{
			currentStr += piece.m_value;
		}
	}
	else if (piece.m_replaceWithMapName)
	{
		Map* map = Map::GetEditableMap();
		if (map == nullptr)
		{
			//Why are you calling this when the map is null?
			return true;
		}
		currentStr += map->GetMapDialogueName();
		handleCasingAsPleaseForThisPiece = false;
	}
	else if (piece.m_checkGlobalVariablesForTextValue)
	{
		std::string globalVariable = "";
		PropertyGetResult result = Map::GetGlobalVariable(piece.m_nameOfQuestVariable, globalVariable, "");
		handleCasingAsPleaseForThisPiece = false;
		if (result == PROPERTY_FETCH_SUCCESS)
		{
			currentStr += globalVariable;
		}
		else
		{
			currentStr += piece.m_value;
		}
	}
	else if (piece.m_checkQuestsForVariableVal)
	{
		std::string testVal = piece.TryToGetValueOutOfQuest(handleCasingAsPleaseForThisPiece);
		if (testVal == "")
		{
			currentStr += piece.m_value;
			handleCasingAsPleaseForThisPiece = false;
		}
		else
		{
			currentStr += testVal;
		}
	}
	else if (piece.m_checkNPCAttributeValue)
	{
		const NPC* npc = dynamic_cast<const NPC*>(agentPlayerTalkingTo);
		if (npc != nullptr)
		{
			std::string testVal = npc->GetNPCAttributeValue(piece.m_nameOfQuestVariable);
			if (testVal != "")
			{
				currentStr += testVal;
			}
			else
			{
				currentStr += piece.m_value;
			}
		}
		else
		{
			currentStr += piece.m_value;
		}
	}
	else
	{
		currentStr += piece.m_value;
	}

	if (piece.m_hasACommaAtEnd)
	{
		currentStr += ",";
	}
	else if (piece.m_hasAPeriodAtEnd)
	{
		currentStr += ".";
	}
	else
	{
	}

	return false;
}

STATIC void TextPiece::CompileStringFromGroupOfTextPiecesFinalize(std::string& out_val,
	std::string& fullCurrentStr,
	std::string& currentStr,
	std::string& currentLine,
	int MaxCharactersPerLine,
	int MaxLinesPerTextBlurb,
	int& currentLineIdx,
	std::vector<std::string>& textLinesInGroupsOfLinesPerPage)
{
	fullCurrentStr += currentStr;
	if (currentLine != "")
	{
		if (fullCurrentStr != "")
		{
			if (currentLine.length() + fullCurrentStr.length() <= (size_t)MaxCharactersPerLine)
			{
				currentLine += fullCurrentStr;
			}
			else
			{
				out_val += currentLine;
				currentLineIdx++;
				if (currentLineIdx >= MaxLinesPerTextBlurb)
				{
					if (s_AddThreeDotsAtSpeechBubbleEnd)
					{
						out_val += "...";
					}
					textLinesInGroupsOfLinesPerPage.push_back(out_val);
					out_val = "";
					currentLineIdx = 0;
				}
				currentLine = fullCurrentStr;
			}
		}
		out_val += currentLine;
	}
	else if (fullCurrentStr != "")
	{
		out_val += fullCurrentStr;
	}
	textLinesInGroupsOfLinesPerPage.push_back(out_val);
}