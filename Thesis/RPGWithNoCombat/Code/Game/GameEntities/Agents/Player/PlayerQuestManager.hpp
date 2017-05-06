#pragma once

#ifndef PLAYERQUESTMANAGER_HPP
#define PLAYERQUESTMANAGER_HPP
#include "Game/Quest/Quest.hpp"

struct QuestEventReqsMet
{
	//Variables
	int m_questIdx = -1;
	int m_eventIdx = -1;

	//Constructors
	QuestEventReqsMet() {};
};

class PlayerQuestManager
{
private:
protected:
	//Variables
	std::vector<Quest*> m_completedQuests;
	std::vector<Quest*> m_quests;
public:
	//Constructors
	PlayerQuestManager();
	virtual ~PlayerQuestManager();

	//Operations
	void TriggerQuest(size_t questIdx, size_t eventIdx);
	void PostTriggerQuest(size_t questIdx, size_t eventIdx);
	void CompletedTriggerQuest(size_t questIdx, size_t eventIdx);
	void CompletedPostTriggerQuest(size_t questIdx, size_t eventIdx);

	//Setters
	void AddQuest(Quest* quest);
	void AddQuestByTitle(const std::string& questTitle);
	void CompleteQuest(Quest* quest);
	void CompleteQuestByTitle(const std::string& questTitle);

	//Getters
	void AddAllQuestsThatReferenceGivenEntityToList(const BaseGameEntity* entity, 
		std::vector<Quest*>& questReferences) const;
	size_t GetNumberOfHeldQuests() const;
	size_t GetNumberOfHeldCompletedQuests() const;
	const Quest* GetQuestAtIndex(size_t idx) const;
	Quest* GetEditableQuestAtIndex(size_t idx);
	const Quest* GetCompletedQuestAtIndex(size_t idx) const;
	Quest* GetEditableCompeltedQuestAtIndex(size_t idx);
	void WritePlayerQuestManagerToString(std::string& str, int indentationAmt) const;

	const Quest* GetQuestByName(const std::string& name) const;
	Quest* GetEditableQuestByName(const std::string& name);
	const Quest* GetCompletedQuestByName(const std::string& name) const;
	Quest* GetEditableCompeltedQuestByName(const std::string& name);

	const std::vector<Quest*>& GetQuests() const;
	const std::vector<Quest*>* GetQuestsPtr() const;
	std::vector<Quest*>* GetQuestsEditablePtr();
	const std::vector<Quest*>& GetCompletedQuests() const;
	const std::vector<Quest*>* GetCompletedQuestsPtr() const;
	std::vector<Quest*>* GetCompletedQuestsEditablePtr();

	bool WriteQuestByNameToString(std::string& str, const std::string& questName, int indentationAmt = 0) const;

};
#endif