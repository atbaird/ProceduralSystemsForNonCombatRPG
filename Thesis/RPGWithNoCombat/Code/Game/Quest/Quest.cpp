#include "Game/Quest/Quest.hpp"
#include "Game/Quest/QuestEvent.hpp"
#include "Game/GameEntities/BaseGameEntity.hpp"
#include "Game/GameEntities/Agents/NPCs/NPC.hpp"
#include "Game/GameEntities/Agents/Player/Player.hpp"
#include "Game/GameEntities/Features/Feature.hpp"
#include "Game/GameEntities/Agents/Names/NameGenerator.hpp"
#include "Game/Common/GameDebuggingCommons.hpp"
#include "Game/Debugging/NamedPropertiesToString.hpp"

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//Constructors
Quest::Quest()
{
	m_QuestVariables = new NamedProperties();
}

Quest::~Quest()
{
	if (m_QuestVariables != nullptr)
	{
		delete m_QuestVariables;
		m_QuestVariables = nullptr;
	}
	for (size_t questEventIdx = 0; questEventIdx < m_QuestEvents.size(); questEventIdx++)
	{
		QuestEvent* questEvent = m_QuestEvents[questEventIdx];
		if (questEvent == nullptr)
		{
			continue;
		}
		delete questEvent;
	}
	m_QuestEvents.clear();
}


//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//Setters
void Quest::AddQuestEvent(QuestEvent* questEvent)
{
	if (questEvent == nullptr)
	{
		return;
	}
	m_QuestEvents.push_back(questEvent);
}

void Quest::SetQuestTitle(const std::string& title)
{
	m_questTitle = title;
	m_lowerCaseQuestTitle = MakeLower(title);
}

void Quest::CopyQuestVariables(const NamedProperties* propertiesCopy)
{
	m_QuestVariables->CopyProperties(propertiesCopy);
}

void Quest::PlugInPlayerUnderVariableName(BaseGameEntity* player)
{
	if (m_variableNameForPlayer == "")
	{
		return;
	}
	SetVariable<BaseGameEntity*>(m_variableNameForPlayer, player);
}

void Quest::SetVariableNameForPlayer(const std::string& variableNameForPlayer)
{
	m_variableNameForPlayer = variableNameForPlayer;
}


//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//Getters
const bool Quest::GetIfQuestReferencesEntity(const BaseGameEntity* entity) const
{
	if (entity == nullptr
		|| m_QuestVariables == nullptr)
	{
		return false;
	}
	const std::map<std::string, NamedPropertyBase*>& variables = m_QuestVariables->GetAllProperties();
	for (std::map<std::string, NamedPropertyBase*>::const_iterator it = variables.begin();
		it != variables.end(); it++)
	{
		NamedPropertyBase* base = it->second;
		TypedNamedProperty<BaseGameEntity*>* entityTest = 
			dynamic_cast<TypedNamedProperty<BaseGameEntity*>*>(base);
		if (entityTest != nullptr)
		{
			if (entityTest->m_data == entity)
			{
				return true;
			}
		}
	}
	return false;
}

const std::string& Quest::GetQuestTitle() const
{
	return m_questTitle;
}

const std::string& Quest::GetLowerCaseQuestTitle() const
{
	return m_lowerCaseQuestTitle;
}

const std::string Quest::GetQuestVariableNameValue(const std::string& questVariableName, 
	const std::string& variableNameOfQuestVariableNameWanted,
	bool& handleCasingAsPlease) const
{
	std::string questVariableNameLowerCase = MakeLower(questVariableName);
	std::string variableNameLowerCase = MakeLower(variableNameOfQuestVariableNameWanted);

	std::string value = "";
	if(variableNameLowerCase != "")
	{
		//Try Entity Test.
		BaseGameEntity* entity = nullptr;
		GetVariable<BaseGameEntity*>(questVariableNameLowerCase, entity, nullptr);
		Player* player = dynamic_cast<Player*>(entity);
		NPC* npc = dynamic_cast<NPC*>(entity);
		Feature* feature = dynamic_cast<Feature*>(entity);
		if (player != nullptr)
		{
			return GetQuestVariableNameValueOffAgent(variableNameLowerCase, player, handleCasingAsPlease);
		}
		else if (npc != nullptr)
		{
			return GetQuestVariableNameValueOffAgent(variableNameLowerCase, npc, handleCasingAsPlease);
		}
		else if (feature != nullptr)
		{
			if (variableNameLowerCase == "name")
			{
				handleCasingAsPlease = false;
				return feature->GetName();
			}
		}
	}
	else
	{
		GetVariable<std::string>(questVariableNameLowerCase, value);
	}

	return value;
}

const std::string Quest::GetQuestVariableNameValueOffAgent(const std::string& variableNameLowerCase,
	Agent* agent, bool& handleCasingAsPlease) const
{
	if (agent == nullptr)
	{
		return "";
	}
	if (variableNameLowerCase == "name"
		|| variableNameLowerCase == "myname")
	{
		handleCasingAsPlease = false;
		return agent->GetName();
	}
	else if (variableNameLowerCase == "firstname"
		|| variableNameLowerCase == "myfirstname")
	{
		handleCasingAsPlease = false;
		return agent->GetFirstName();
	}
	else if (variableNameLowerCase == "secondname"
		|| variableNameLowerCase == "mysecondname")
	{
		handleCasingAsPlease = false;
		return agent->GetSecondName();
	}
	else if (variableNameLowerCase == "lastname"
		|| variableNameLowerCase == "mylastname")
	{
		handleCasingAsPlease = false;
		return agent->GetLastName();
	}
	else if (variableNameLowerCase == "sibling"
		|| variableNameLowerCase == "siblingtitle")
	{
		handleCasingAsPlease = true;
		bool isMale = agent->GetIsMale();
		if (isMale)
		{
			return NameGenerator::GlobalGetWhatReferMaleSiblingAs(agent->GetNameSet());
		}
		else
		{
			return NameGenerator::GlobalGetWhatReferFemaleSiblingAs(agent->GetNameSet());
		}
	}
	else if (variableNameLowerCase == "gender"
		|| variableNameLowerCase == "gendertitle")
	{
		handleCasingAsPlease = true;
		bool isMale = agent->GetIsMale();
		if (isMale)
		{
			return NameGenerator::GlobalGetWhatReferMaleAs(agent->GetNameSet());
		}
		else
		{
			return NameGenerator::GlobalGetWhatReferFemaleAs(agent->GetNameSet());
		}
	}
	else if (variableNameLowerCase == "genderpronounowning"
		|| variableNameLowerCase == "genderpronoun")
	{
		handleCasingAsPlease = true;
		bool isMale = agent->GetIsMale();
		if (isMale)
		{
			return NameGenerator::GlobalGetMaleOwningPronoun(agent->GetNameSet());
		}
		else
		{
			return NameGenerator::GlobalGetFemaleOwningPronoun(agent->GetNameSet());
		}
	}
	else if (variableNameLowerCase == "genderpronounis")
	{
		handleCasingAsPlease = true;
		bool isMale = agent->GetIsMale();
		if (isMale)
		{
			return NameGenerator::GlobalGetMaleIsPronoun(agent->GetNameSet());
		}
		else
		{
			return NameGenerator::GlobalGetFemaleIsPronoun(agent->GetNameSet());
		}
	}
	else
	{
		//Try for NPC Attribute
		const NPC* npc = dynamic_cast<const NPC*>(agent);
		if (npc != nullptr)
		{
			std::string testVal = npc->GetNPCAttributeValue(variableNameLowerCase);
			if (testVal != "")
			{
				return testVal;
			}
		}
	}
	return "";
}

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//Operations
bool Quest::CheckMeetsAnyOfAnEventsRequirements() const
{
	for (size_t i = 0; i < m_QuestEvents.size(); i++)
	{
		QuestEvent* even = m_QuestEvents[i];
		if (even == nullptr)
		{
			continue;
		}
		if (even->TestIfMatchesQuestEventsRequirements())
		{
			m_LastEventThatCouldRunWhenTested = i;
			return true;
		}
	}
	return false;
}

int Quest::GetFirstEventThatHasRequirementsMet() const
{
	for (size_t i = 0; i < m_QuestEvents.size(); i++)
	{
		QuestEvent* even = m_QuestEvents[i];
		if (even == nullptr)
		{
			continue;
		}
		if (even->TestIfMatchesQuestEventsRequirements())
		{
			m_LastEventThatCouldRunWhenTested = i;
			return i;
		}
	}

	return -1;
}


void Quest::TriggerFirstEventThatHasRequirmentsMet()
{
	for (size_t i = 0; i < m_QuestEvents.size(); i++)
	{
		QuestEvent* even = m_QuestEvents[i];
		if (even == nullptr)
		{
			continue;
		}
		if (even->TestIfMatchesQuestEventsRequirements())
		{
			m_LastEventThatCouldRunWhenTested = i;
			m_EventTriggeredBeforeLast = m_LastEventTriggered;
			m_LastEventTriggered = i;
			even->TriggerEvent();
			return;
		}
	}
}

void Quest::TriggerEventByIdx(size_t idx)
{
	if (idx >= m_QuestEvents.size())
	{
		return;
	}
	m_EventTriggeredBeforeLast = m_LastEventTriggered;
	m_LastEventTriggered = idx;
	m_QuestEvents[idx]->TriggerEvent();
}


void Quest::TriggerFirstPostEventThatHasRequirmentsMet()
{
	for (size_t i = 0; i < m_QuestEvents.size(); i++)
	{
		QuestEvent* even = m_QuestEvents[i];
		if (even == nullptr)
		{
			continue;
		}
		if (even->TestIfMatchesQuestEventsRequirements())
		{
			even->PostTriggerEvent();
			m_LastEventThatCouldRunWhenTested = i;
			m_EventTriggeredBeforeLast = m_LastEventTriggered;
			m_LastEventTriggered = i;
			return;
		}
	}
}

void Quest::TriggerPostEventByIdx(size_t idx)
{
	if (idx >= m_QuestEvents.size())
	{
		return;
	}
	m_LastEventTriggered = idx;
	m_QuestEvents[idx]->PostTriggerEvent();
}


void Quest::WriteQuestToString(std::string& str, int indentationAmt) const
{
	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Quest Title: " + m_questTitle + "\n";

	indentationAmt += 3;
	int nextLevel = indentationAmt + 3;

	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Last Event Index tested could run: " + std::to_string(m_LastEventThatCouldRunWhenTested) + "\n";

	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Last Event ran: " + std::to_string(m_LastEventTriggered) + "\n";

	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Event ran before last: " + std::to_string(m_EventTriggeredBeforeLast) + "\n";

	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Quest Variables: ";
	if (m_QuestVariables == nullptr)
	{
		str += "NONE\n";
	}
	else
	{
		str += "\n";
		NamedPropertiesToString::ConvertAllKnownNamedPropertyValuesToString(m_QuestVariables, str, nextLevel);
	}

	if (m_QuestEvents.size() != 0)
	{
		GameDebuggingCommons::AddIndentation(str, indentationAmt);
		str += "Quest Events: \n";
		for (size_t i = 0; i < m_QuestEvents.size(); i++)
		{
			const QuestEvent* even = m_QuestEvents[i];
			GameDebuggingCommons::AddIndentation(str, nextLevel);
			str += "Event Index: " + std::to_string(i) + "\n";
			if (even == nullptr)
			{
				GameDebuggingCommons::AddIndentation(str, nextLevel);
				str += "NULL QUEST EVENT\n";
			}
			else
			{
				even->WriteQuestEventToString(str, nextLevel);
			}
		}
	}
}