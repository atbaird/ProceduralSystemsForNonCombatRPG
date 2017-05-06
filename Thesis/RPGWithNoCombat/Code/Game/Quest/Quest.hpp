#pragma once

#ifndef QUEST_HPP
#define QUEST_HPP
#include "Engine/EventSystem/NamedProperties.hpp"
#include "Engine/Core/StringUtils.hpp"
#include <string>
#include <vector>


class BaseGameEntity;
class QuestEvent;
class Agent;
class Quest
{
private:
protected:
	std::vector<QuestEvent*> m_QuestEvents;
	NamedProperties* m_QuestVariables = nullptr;
	std::string m_questTitle = "";
	std::string m_lowerCaseQuestTitle = "";
	std::string m_variableNameForPlayer = "";
	mutable int m_LastEventThatCouldRunWhenTested = -1;
	mutable int m_EventTriggeredBeforeLast = -1;
	mutable int m_LastEventTriggered = -1;
public:
	//Constructors
	Quest();
	virtual ~Quest();

	//Setters
	void AddQuestEvent(QuestEvent* questEvent);
	void SetQuestTitle(const std::string& questTitle);
	void CopyQuestVariables(const NamedProperties* properties);
	void PlugInPlayerUnderVariableName(BaseGameEntity* player);
	void SetVariableNameForPlayer(const std::string& variableNameForPlayer);
	template<typename T>
	void SetVariable(const std::string& variableName, const T& variable);

	//Getters
	const bool GetIfQuestReferencesEntity(const BaseGameEntity* entity) const;
	const std::string& GetQuestTitle() const;
	const std::string& GetLowerCaseQuestTitle() const;
	const std::string GetQuestVariableNameValue(const std::string& questVariableName,
		const std::string& variableNameOfQuestVariableNameWanted,
		bool& handleCasingAsPlease) const;
protected:
	const std::string GetQuestVariableNameValueOffAgent(const std::string& variableNameLowerCase,
		Agent* agent, bool& handleCasingAsPlease) const;

public:
	template<typename T>
	PropertyGetResult GetVariable(const std::string& variableName, T& variable) const;
	template<typename T>
	PropertyGetResult GetVariable(const std::string& variableName, T& variable, const T& defaul) const;

	//Operations
	bool CheckMeetsAnyOfAnEventsRequirements() const;
	int GetFirstEventThatHasRequirementsMet() const;
	void TriggerFirstEventThatHasRequirmentsMet();
	void TriggerEventByIdx(size_t idx);
	void TriggerFirstPostEventThatHasRequirmentsMet();
	void TriggerPostEventByIdx(size_t idx);
	void WriteQuestToString(std::string& str, int indentationAmt) const;
};

//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//Setters
template<typename T>
void Quest::SetVariable(const std::string& variableName, const T& variable)
{
	std::string lowerCase = MakeLower(variableName);
	m_QuestVariables->Set<T>(lowerCase, variable);
}


//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//Getters
template<typename T>
PropertyGetResult Quest::GetVariable(const std::string& variableName, T& variable) const
{
	std::string lowerCase = MakeLower(variableName);
	return m_QuestVariables->Get<T>(lowerCase, variable);
}

template<typename T>
PropertyGetResult Quest::GetVariable(const std::string& variableName, T& variable, const T& defaul) const
{
	std::string lowerCase = MakeLower(variableName);
	return m_QuestVariables->Get<T>(lowerCase, variable, defaul);
}

#endif