#pragma once

#ifndef QUESTTEMPLATE_HPP
#define QUESTTEMPLATE_HPP
#include "Game/Quest/QuestEvent.hpp"
#include "Game/Quest/QuestTemplate/SpecialVariableTypes.hpp"
#include "Game/Quest/MapQuestImportantData.hpp"


struct MapQuestImportantData;
class Quest;
class MapFiles;
class QuestTemplate
{
private:
protected:
	NamedProperties m_QuestVariables;
	std::string m_questTitle = "";
	std::string m_DialogueVersionOfQuestTitle = "";
	std::vector<QuestEvent*> m_QuestEvents;
	std::vector<XMLNode> m_AgentsToGenerate;
	std::vector<XMLNode> m_FeaturesToGenerate;
	std::vector<SpecialVariableTypes> m_SpecialVariablesToKeepTrackOf;
public:
	//Constructors
	QuestTemplate();
	QuestTemplate(const XMLNode& node);
	QuestTemplate(const QuestTemplate& other);
	virtual ~QuestTemplate();

protected:
	void ReadQuestXMLNodeAttributes(const XMLNode& node);
	void ReadQuestXMLNodeChildren(const XMLNode& node);
	void ReadVariablesXMLNodeChildren(const XMLNode& node);
	void ReadAgentsToGenerateXMLNodeChildren(const XMLNode& node);
	void ReadFeaturesToGenerateXMLNodeChildren(const XMLNode& node);
	void ReadEventsXMLNodeChildren(const XMLNode& node);
public:

	//Setters
	void SetQuestTitle(const std::string& questTitle);
	void AddQuestEvent(QuestEvent* questEvent);
	void SetNamedProperties(const NamedProperties& questVariables);
	template<typename T>
	void AddOrSetQuestVariable(const std::string& variableName, const T& var);


	//Getters
	const NamedProperties& GetNamedProperties() const;
	const std::string& GetQuestTitle() const;
	const std::vector<QuestEvent*>& GetQuestEvents() const;
	const std::vector<QuestEvent*>* GetQuestEventsPtr() const;
	std::vector<QuestEvent*>* GetQuestEventsEditablePtr();
	template<typename T>
	void GetQuestVariable(const std::string& variableName, T& var);
	template<typename T>
	void GetQuestVariable(const std::string& variableName, T& var, const T& defaultVar);

	//Operations
	void CopyVariablesOntoQuest(Quest* quest, const XMLNode& mapQuestNode,
		const std::string& currentMapFile, MapQuestImportantData& data,
		MapFiles* files) const;
	void WriteQuestTemplateToString(std::string& str, int indentationAmt) const;
};

//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//Setters
template<typename T>
void QuestTemplate::AddOrSetQuestVariable(const std::string& variableName, const T& var)
{
	m_QuestVariables.Set<T>(variableName, var);
}

//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//Getters
template<typename T>
void  QuestTemplate::GetQuestVariable(const std::string& variableName, T& var)
{
	m_QuestVariables.Get<T>(variableName, var);
}

template<typename T>
void  QuestTemplate::GetQuestVariable(const std::string& variableName, T& var, const T& defaultVar)
{
	m_QuestVariables.Get<T>(variableName, var, defaultVar);
}
#endif