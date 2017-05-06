#pragma once

#ifndef QUESTTRIGGERGIVEENTITY_HPP
#define QUESTTRIGGERGIVEENTITY_HPP
#include "Game/Quest/QuestEvents/QuestTrigger/QuestTriggerRegistration.hpp"
#include "Game/Quest/QuestEvents/QuestTrigger.hpp"

//Registration Funcs
QuestTrigger* QuestTriggerGiveEntityFunc(const XMLNode& node);
QuestTrigger* QuestTriggerDefaultGiveEntityFunc();

class QuestTriggerGiveEntity : public QuestTrigger
{
private:
protected:
	//Static Variables
	static QuestTriggerRegistration s_TriggerRegistration;

	//Variables
	std::string m_EntityName = "";
	std::string m_ItemName = "";
	float m_amountToGiveEntity = 0.f;
public:
	//Constructors
	QuestTriggerGiveEntity();
	QuestTriggerGiveEntity(const XMLNode& node, Quest* owningQuest = nullptr);
	QuestTriggerGiveEntity(const QuestTriggerGiveEntity& other, Quest* owningQuest = nullptr);
	virtual ~QuestTriggerGiveEntity();

	//Setters
	void SetEntityName(const std::string& name);
	void SetItemName(const std::string& itemName);
	void SetAmountToGiveEntity(float amount);

	//Getters
	const std::string& GetEntityName() const;
	const std::string& GetItemName() const;
	float GetAmountToGiveEntity() const;

	//Operations
	virtual bool PerformQuestTrigger() override;
	virtual QuestTrigger* Clone(Quest* owningQuest = nullptr) const override;
	virtual void WriteQuestTriggerToString(std::string& str, int indentationAmt) const override;
};
#endif