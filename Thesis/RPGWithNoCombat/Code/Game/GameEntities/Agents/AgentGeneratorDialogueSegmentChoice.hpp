#pragma once

#ifndef AGENTGENERATORDIALOGUECHOICE_HPP
#define AGENTGENERATORDIALOGUECHOICE_HPP
#include <string>

class AgentGeneratorDialogueSegmentChoice
{
private:
protected:
	//variables
	std::string m_DialogueSegment;
	int m_numberOfUsesLimit = 0;
	mutable int m_numberOfUsesSoFar = 0;
	bool m_limitUses = false;
public:
	//Constructors
	AgentGeneratorDialogueSegmentChoice(const std::string& dialogueChoice = "");
	virtual ~AgentGeneratorDialogueSegmentChoice();

	//Setters
	void AddDialogueSegment(const std::string choice);
	void SetNumberOfUsesAllowed(int limit);
	void SetNumberOfUsesSoFar(int uses);
	void IncrementNumberOfUsesSoFar() const;

	//Getters
	void WriteDialogueSegmentChoiceToString(std::string& str, int indentationLvl) const;
	const std::string& GetDialogueChoice() const;
	int GetNumberOfUsesLimit() const;
	int GetNumberOfUsesSoFar() const;
	bool GetLimitUses() const;
	bool GetShouldBeAllowedToUseThisChoice() const;
};
#endif