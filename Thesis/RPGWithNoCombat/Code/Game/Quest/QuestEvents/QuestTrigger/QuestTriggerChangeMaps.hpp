#pragma once

#ifndef QUESTTRIGGERCHANGEMAPS_HPP
#define QUESTTRIGGERCHANGEMAPS_HPP
#include "Game/Quest/QuestEvents/QuestTrigger/QuestTriggerRegistration.hpp"
#include "Game/Quest/QuestEvents/QuestTrigger.hpp"

//Registration Funcs
QuestTrigger* QuestTriggerChangeMapsFunc(const XMLNode& node);
QuestTrigger* QuestTriggerDefaultChangeMapsFunc();

class Quest;
class QuestTriggerChangeMaps : public QuestTrigger
{
private:
protected:
	//Static Variables
	static QuestTriggerRegistration s_ChangeMapsRegistration;

	//Variables
	std::string m_MapFileToChangeTo = "";
public:
	//Constructors
	QuestTriggerChangeMaps();
	QuestTriggerChangeMaps(const XMLNode& node, Quest* owningQuest = nullptr);
	QuestTriggerChangeMaps(const QuestTriggerChangeMaps& other, Quest* owningQuest = nullptr);
	virtual ~QuestTriggerChangeMaps();

	//Setters
	void SetMapFileToChangeTo(const std::string& mapFile);

	//Getters
	const std::string& GetMapFile() const;

	//Operations
	virtual bool PerformQuestTrigger() override;
	virtual QuestTrigger* Clone(Quest* owningQuest = nullptr) const override;
	virtual void WriteQuestTriggerToString(std::string& str, int indentationAmt) const override;
};
#endif