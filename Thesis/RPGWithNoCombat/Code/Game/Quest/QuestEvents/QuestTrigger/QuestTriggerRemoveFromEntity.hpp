#pragma once

#ifndef QUESTTRIGGERREMOVEFROMENTITY_HPP
#define QUESTTRIGGERREMOVEFROMENTITY_HPP
#include "Game/Quest/QuestEvents/QuestTrigger/QuestTriggerRegistration.hpp"
#include "Game/Quest/QuestEvents/QuestTrigger.hpp"

//Registration Funcs
QuestTrigger* QuestTriggerRemoveFromEntityFunc(const XMLNode& node);
QuestTrigger* QuestTriggerDefaultRemoveFromEntityFunc();

class QuestTriggerRemoveFromEntity : public QuestTrigger
{
private:
protected:
	//Static Variables
	static QuestTriggerRegistration s_RemoveFromEntityRegistration;

	//Variables
	std::string m_EntityName = "";
	std::string m_ItemName = "";
	float m_quantityToRemove = 0.f;
public:
	//Constructors
	QuestTriggerRemoveFromEntity();
	QuestTriggerRemoveFromEntity(const XMLNode& node, Quest* owningQuest = nullptr);
	QuestTriggerRemoveFromEntity(const QuestTriggerRemoveFromEntity& other, Quest* owningQuest = nullptr);
	virtual ~QuestTriggerRemoveFromEntity();

	//Setters
	void SetEntityName(const std::string& entityName);
	void SetItemName(const std::string itemName);
	void SetQuantityToRemove(float quantityToRemove);

	//Getters
	const std::string& GetEntityName() const;
	const std::string& GetItemName() const;
	float GetQuantityToRemove() const;

	//Operations
	bool PerformQuestTrigger() override;
	QuestTrigger* Clone(Quest* owningQuest = nullptr) const override;
	virtual void WriteQuestTriggerToString(std::string& str, int indentationAmt) const override;
};
#endif