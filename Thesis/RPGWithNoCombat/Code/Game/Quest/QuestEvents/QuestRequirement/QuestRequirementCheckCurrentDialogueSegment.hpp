#pragma once

#ifndef QUESTREQUIREMENTCHECKCURRENTDIALOGUESEGMENT_HPP
#define QUESTREQUIREMENTCHECKCURRENTDIALOGUESEGMENT_HPP
#include "Game/Quest/QuestEvents/QuestRequirement.hpp"
#include "Game/Quest/QuestEvents/QuestRequirement/QuestRequirementRegistration.hpp"

//Creation Funcs
QuestRequirement* CheckCurrentDialogueSegmentCreationFunc(const XMLNode& node);
QuestRequirement* CheckCurrentDialogueSegmentDefaultCreationFunc();

class QuestRequirementCheckCurrentDialogueSegment : public QuestRequirement
{
private:
protected:
	//Static Variables
	static QuestRequirementRegistration s_CheckPlayerStatsRegistration;
	
	//Variables
	std::string m_wantedDialogueSegmentName = "";
public:
	//Constructors
	QuestRequirementCheckCurrentDialogueSegment();
	QuestRequirementCheckCurrentDialogueSegment(const XMLNode& node);
	QuestRequirementCheckCurrentDialogueSegment(const QuestRequirementCheckCurrentDialogueSegment& other,
		Quest* owningQuest = nullptr);
	virtual ~QuestRequirementCheckCurrentDialogueSegment();

	//Operations
	virtual void WriteQuestRequirementToString(std::string& str, int indentationAmt) const override;
	virtual const bool DeterminePlayerMeetsQuestRequirement() const override;
	virtual QuestRequirement* Clone(Quest* owningQuest = nullptr) const override;
};

#endif