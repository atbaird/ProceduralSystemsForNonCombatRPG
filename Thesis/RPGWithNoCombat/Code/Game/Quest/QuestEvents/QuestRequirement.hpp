#pragma once

#ifndef QUESTREQUIREMENT_HPP
#define QUESTREQUIREMENT_HPP
#include "Engine/EventSystem/NamedProperties.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Game/Quest/QuestEvents/QuestRequirement/QuestRequirementType.hpp"
#include "Game/Map/TileDefinitionSpecialCase/SpecialCaseOperations.hpp"

enum QuestRequirementCheckVariableType;
struct XMLNode;
class Quest;
class QuestRequirement
{
private:
protected:
	Quest* m_owningQuest = nullptr;
	QuestRequirementType m_questRequirementType = INVALID_QUEST_REQUIREMENT_TYPE;
public:
	//Constructors
	QuestRequirement(QuestRequirementType requirementType = INVALID_QUEST_REQUIREMENT_TYPE);
	QuestRequirement(QuestRequirementType requirementType, const XMLNode& node);
	QuestRequirement(const QuestRequirement& other, Quest* owningQuest = nullptr);
	virtual ~QuestRequirement();

	//Setters
	void SetQuestOwnedBy(Quest* quest);
	void SetQuestRequirementType(QuestRequirementType type);

	//Getters
	Quest* GetQuestOwnedBy() const;
	QuestRequirementType GetQuestRequirementType() const;


	//operations
	virtual const bool DeterminePlayerMeetsQuestRequirement() const;
	virtual QuestRequirement* Clone(Quest* owningQuest = nullptr) const = 0;
	virtual void WriteQuestRequirementToString(std::string& str, int indentationAmt) const = 0;

	//Static Functions
	static QuestRequirementType GetQuestRequirementTypeShouldBe(const XMLNode& node);
	static QuestRequirementCheckVariableType GetTypeOfCheckVariableShouldBe(const XMLNode& node);
	static std::string GetQuestRequirementTypeAndQuestRequirementCheckVariableType(const XMLNode& node);
	static SpecialCaseOperations GetSpecialCaseOperationForStringContents(const std::string& contents);
	static void ParseTextForSpecialCaseOperationsAndValue(const std::string& str,
		SpecialCaseOperations& operation, float& value);
	static void ParseTextForSpecialCaseOperationsAndValue(const std::string& str,
		SpecialCaseOperations& operation, int& value);

};
#endif