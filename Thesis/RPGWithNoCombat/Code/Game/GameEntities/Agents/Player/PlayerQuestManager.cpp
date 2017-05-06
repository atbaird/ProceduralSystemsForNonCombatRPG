#include "Game/GameEntities/Agents/Player/PlayerQuestManager.hpp"
#include "Game/GameEntities/Agents/Player/Player.hpp"
#include "Game/Quest/QuestGenerator.hpp"
#include "Game/Common/GameDebuggingCommons.hpp"

//------------------------------------------------------------------------
//------------------------------------------------------------------------
//------------------------------------------------------------------------
//Constructors
PlayerQuestManager::PlayerQuestManager()
{

}

PlayerQuestManager::~PlayerQuestManager()
{
	for (size_t i = 0; i < m_quests.size(); i++)
	{
		delete m_quests[i];
	}
	m_quests.clear();

	for (size_t i = 0; i < m_completedQuests.size(); i++)
	{
		delete m_completedQuests[i];
	}
	m_completedQuests.clear();
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
//------------------------------------------------------------------------
//Operations
void PlayerQuestManager::TriggerQuest(size_t questIdx, size_t eventIdx)
{
	if (questIdx >= m_quests.size())
	{
		return;
	}
	Quest* quest = m_quests[questIdx];
	if (quest == nullptr)
	{
		return;
	}
	quest->TriggerEventByIdx(eventIdx);
}

void PlayerQuestManager::PostTriggerQuest(size_t questIdx, size_t eventIdx)
{
	if (questIdx >= m_quests.size())
	{
		return;
	}
	Quest* quest = m_quests[questIdx];
	if (quest == nullptr)
	{
		return;
	}
	quest->TriggerPostEventByIdx(eventIdx);
}

void PlayerQuestManager::CompletedTriggerQuest(size_t questIdx, size_t eventIdx)
{
	if (questIdx >= m_completedQuests.size())
	{
		return;
	}
	Quest* quest = m_completedQuests[questIdx];
	if (quest == nullptr)
	{
		return;
	}
	quest->TriggerEventByIdx(eventIdx);
}

void PlayerQuestManager::CompletedPostTriggerQuest(size_t questIdx, size_t eventIdx)
{
	if (questIdx >= m_completedQuests.size())
	{
		return;
	}
	Quest* quest = m_completedQuests[questIdx];
	if (quest == nullptr)
	{
		return;
	}
	quest->TriggerPostEventByIdx(eventIdx);
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
//------------------------------------------------------------------------
//Setters
void PlayerQuestManager::AddQuest(Quest* quest)
{
	if (quest == nullptr)
	{
		return;
	}
	m_quests.push_back(quest);
}

void PlayerQuestManager::AddQuestByTitle(const std::string& questTitle)
{
	std::string lowerCase = MakeLower(questTitle);
	for (size_t i = 0; i < m_quests.size(); i++)
	{
		Quest* quest = m_quests[i];
		if (quest != nullptr && quest->GetLowerCaseQuestTitle() == lowerCase)
		{
			m_quests.erase(m_quests.begin() + i);
			m_completedQuests.push_back(quest);
			break;
		}
	}
}

void PlayerQuestManager::CompleteQuest(Quest* quest)
{
	if (quest == nullptr)
	{
		return;
	}
	bool found = false;
	for (size_t i = 0; i < m_quests.size(); i++)
	{
		Quest* testQuest = m_quests[i];
		if (testQuest != nullptr && quest == testQuest)
		{
			found = true;
			m_quests.erase(m_quests.begin() + i);
			break;
		}
	}

	m_completedQuests.push_back(quest);
}

void PlayerQuestManager::CompleteQuestByTitle(const std::string& questTitle)
{
	std::string lowerCase = MakeLower(questTitle);
	bool found = false;
	for (size_t i = 0; i < m_quests.size(); i++)
	{
		Quest* testQuest = m_quests[i];
		if (testQuest != nullptr && testQuest->GetLowerCaseQuestTitle() == lowerCase)
		{
			found = true;
			m_quests.erase(m_quests.begin() + i);
			m_completedQuests.push_back(testQuest);
			break;
		}
	}

}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
//------------------------------------------------------------------------
//Getters
void PlayerQuestManager::AddAllQuestsThatReferenceGivenEntityToList(const BaseGameEntity* entity,
	std::vector<Quest*>& questReferences) const
{
	for (size_t i = 0; i < m_quests.size(); i++)
	{
		Quest* quest = m_quests[i];
		if (quest != nullptr && quest->GetIfQuestReferencesEntity(entity))
		{
			questReferences.push_back(quest);
		}
	}

	for (size_t i = 0; i < m_completedQuests.size(); i++)
	{
		Quest* quest = m_completedQuests[i];
		if (quest != nullptr && quest->GetIfQuestReferencesEntity(entity))
		{
			questReferences.push_back(quest);
		}
	}

}

size_t PlayerQuestManager::GetNumberOfHeldQuests() const
{
	return m_quests.size();
}

size_t PlayerQuestManager::GetNumberOfHeldCompletedQuests() const
{
	return m_completedQuests.size();
}

const Quest* PlayerQuestManager::GetQuestAtIndex(size_t idx) const
{
	return m_quests[idx];
}

Quest* PlayerQuestManager::GetEditableQuestAtIndex(size_t idx)
{
	return m_quests[idx];
}

const Quest* PlayerQuestManager::GetCompletedQuestAtIndex(size_t idx) const
{
	return m_completedQuests[idx];
}

Quest* PlayerQuestManager::GetEditableCompeltedQuestAtIndex(size_t idx)
{
	return m_completedQuests[idx];
}

void PlayerQuestManager::WritePlayerQuestManagerToString(std::string& str, int indentationAmt) const
{
	int nextLevel = indentationAmt + 3;
	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Active Quests on Player: ";
	if (m_quests.size() != 0)
	{
		str += "\n";
		for (size_t i = 0; i < m_quests.size(); i++)
		{
			const Quest* quest = m_quests[i];
			if (quest != nullptr)
			{
				quest->WriteQuestToString(str, nextLevel);
			}
			else
			{
				GameDebuggingCommons::AddIndentation(str, nextLevel);
				str += "NULL QUEST\n";
			}
		}
	}
	else
	{
		str += "NONE\n";
	}



	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Completed Quests on Player: ";
	if (m_completedQuests.size() != 0)
	{
		str += "\n";
		for (size_t i = 0; i < m_completedQuests.size(); i++)
		{
			const Quest* quest = m_completedQuests[i];
			if (quest != nullptr)
			{
				quest->WriteQuestToString(str, nextLevel);
			}
			else
			{
				GameDebuggingCommons::AddIndentation(str, nextLevel);
				str += "NULL QUEST\n";
			}
		}
	}
	else
	{
		str += "NONE\n";
	}


}

const Quest* PlayerQuestManager::GetQuestByName(const std::string& name) const
{
	std::string lowerCase = MakeLower(name);
	for (size_t i = 0; i < m_quests.size(); i++)
	{
		Quest* quest = m_quests[i];
		if (quest != nullptr && quest->GetLowerCaseQuestTitle() == lowerCase)
		{
			return quest;
		}
	}
	return nullptr;
}

Quest* PlayerQuestManager::GetEditableQuestByName(const std::string& name)
{
	std::string lowerCase = MakeLower(name);
	for (size_t i = 0; i < m_quests.size(); i++)
	{
		Quest* quest = m_quests[i];
		if (quest != nullptr && quest->GetLowerCaseQuestTitle() == lowerCase)
		{
			return quest;
		}
	}
	return nullptr;
}

const Quest* PlayerQuestManager::GetCompletedQuestByName(const std::string& name) const
{
	std::string lowerCase = MakeLower(name);
	for (size_t i = 0; i < m_quests.size(); i++)
	{
		Quest* quest = m_completedQuests[i];
		if (quest != nullptr && quest->GetLowerCaseQuestTitle() == lowerCase)
		{
			return quest;
		}
	}
	return nullptr;
}

Quest* PlayerQuestManager::GetEditableCompeltedQuestByName(const std::string& name)
{
	std::string lowerCase = MakeLower(name);
	for (size_t i = 0; i < m_quests.size(); i++)
	{
		Quest* quest = m_completedQuests[i];
		if (quest != nullptr && quest->GetLowerCaseQuestTitle() == lowerCase)
		{
			return quest;
		}
	}
	return nullptr;
}

const std::vector<Quest*>& PlayerQuestManager::GetQuests() const
{
	return m_quests;
}

const std::vector<Quest*>* PlayerQuestManager::GetQuestsPtr() const
{
	return &m_quests;
}

std::vector<Quest*>* PlayerQuestManager::GetQuestsEditablePtr()
{
	return &m_quests;
}

const std::vector<Quest*>& PlayerQuestManager::GetCompletedQuests() const
{
	return m_completedQuests;
}

const std::vector<Quest*>* PlayerQuestManager::GetCompletedQuestsPtr() const
{
	return &m_completedQuests;
}

std::vector<Quest*>* PlayerQuestManager::GetCompletedQuestsEditablePtr()
{
	return &m_completedQuests;
}

bool PlayerQuestManager::WriteQuestByNameToString(std::string& str, const std::string& questName, 
	int indentationAmt) const
{
	std::string lowerCaseName = MakeLower(questName);
	for (size_t i = 0; i < m_quests.size(); i++)
	{
		const Quest* quest = m_quests[i];
		if (quest != nullptr
			&& lowerCaseName == quest->GetLowerCaseQuestTitle())
		{
			GameDebuggingCommons::AddIndentation(str, indentationAmt);
			str += "Quest Found in: Player Quest List\n";
			quest->WriteQuestToString(str, indentationAmt + 3);
			return true;
		}
	}

	for (size_t i = 0; i < m_completedQuests.size(); i++)
	{
		const Quest* quest = m_quests[i];
		if (quest != nullptr
			&& lowerCaseName == quest->GetLowerCaseQuestTitle())
		{
			GameDebuggingCommons::AddIndentation(str, indentationAmt);
			str += "Quest Found in: Player Completed Quest List\n";
			quest->WriteQuestToString(str, indentationAmt + 3);
		}
	}

	return false;
}