#pragma once

#ifndef DIALOGUEGROUPINGS_HPP
#define DIALOGUEGROUPINGS_HPP
#include <vector>

class DialogueTrigger;
struct XMLNode;
#define STATIC
struct DialogueGroupings
{
public:
	//variables
	std::vector<std::string> m_allDialogueSegmentNames;
	std::string m_GroupingName = "";
	std::string m_lowerCaseGroupingName = "";
	std::vector<DialogueTrigger*> m_actionsForWhenDialogueClosed;
	mutable int m_lastDialogueSegmentIdxUsed = -1;
public:
	//Constructors
	DialogueGroupings(const std::string& groupingName = ""); //Make sure grouping name does not have #'s at end
	DialogueGroupings(const XMLNode& node);
	DialogueGroupings(const DialogueGroupings& other);
	virtual ~DialogueGroupings();

protected:
	void ReadXMLChildren(const XMLNode& node);
	void ReadXMLForDialogueSegmentNames(const XMLNode& node);
	void ReadXMLForDialogueActions(const XMLNode& node);
public:

	//Operations
	void TriggerActionsForAfterMemberOfGroupingCloses();

	//Setters
	void SetName(const std::string& groupingName);
	void AddDialogueSegmentName(const std::string& name);
	
	//Getters
	const std::string& GetGroupingName() const;
	const std::string& GetLowerCaseGroupingName() const;
	int GetIdxOfNameIfExists(const std::string& name) const;
	std::string GetRandomNameFromListingWithListeningForRequirements() const;
	std::string GetRandomNameFromListingWithoutListeningForRequirements() const;
	void PullOutAllAvailableDialogueSegmentsBasedOnRequirementsMet(std::vector<std::string>& list,
		std::vector<int>& indexes) const;
	void WriteDialogueGroupToString(std::string& str, int indentationAmt) const;


	//Static Functions
	static const std::string GetDialogueSegmentNameWithoutNumber(const std::string& name);
};


#endif