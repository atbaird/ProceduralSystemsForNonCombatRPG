#pragma once

#ifndef QUESTREQUIREMENTCHECKPLAYERHASITEM_HPP
#define QUESTREQUIREMENTCHECKPLAYERHASITEM_HPP
#include "Game/Quest/QuestEvents/QuestRequirement.hpp"
#include "Game/Quest/QuestEvents/QuestRequirement/QuestRequirementRegistration.hpp"

//Creation Funcs
QuestRequirement* CheckPlayerHasItemCreationFunc(const XMLNode& node);
QuestRequirement* CheckPlayerHasItemDefaultCreationFunc();

class QuestRequirementCheckPlayerHasItem : public QuestRequirement
{
private:
protected:
	//Static Variables
	static QuestRequirementRegistration s_CheckPlayerStatsRegistration;

	//Variables
	std::string m_itemName = "";
	float m_amountWanted = 0.0;
	SpecialCaseOperations m_specialCaseOperation = IS_SAME;
public:
	//Constructors
	QuestRequirementCheckPlayerHasItem();
	QuestRequirementCheckPlayerHasItem(const XMLNode& node);
	QuestRequirementCheckPlayerHasItem(const QuestRequirementCheckPlayerHasItem& other,
		Quest* owningQuest = nullptr);
	virtual ~QuestRequirementCheckPlayerHasItem();


	//Operations
	virtual void WriteQuestRequirementToString(std::string& str, int indentationAmt) const override;
	virtual const bool DeterminePlayerMeetsQuestRequirement() const override;
	virtual QuestRequirement* Clone(Quest* owningQuest = nullptr) const override;
};
#endif