#include "Game/Quest/QuestEvents/QuestTrigger/QuestTriggerCompleteQuest.hpp"
#include "Game/Quest/Quest.hpp"
#include "Game/GameEntities/Agents/Player/Player.hpp"
#include "Game/Common/GameDebuggingCommons.hpp"

//---------------------------------------------------------
//---------------------------------------------------------
//---------------------------------------------------------
//Registration Funcs
QuestTrigger* QuestTriggerCompleteQuestsFunc(const XMLNode& node)
{
	return new QuestTriggerCompleteQuest(node);
}

QuestTrigger* QuestTriggerDefaultCompleteQuestsFunc()
{
	return new QuestTriggerCompleteQuest();
}

//---------------------------------------------------------
//---------------------------------------------------------
//---------------------------------------------------------
//Static Variables
STATIC QuestTriggerRegistration 
	QuestTriggerCompleteQuest::s_CompleteQuestsRegistration 
	= QuestTriggerRegistration("CompleteQuest",
		QuestTriggerCompleteQuestsFunc, QuestTriggerDefaultCompleteQuestsFunc);

//---------------------------------------------------------
//---------------------------------------------------------
//---------------------------------------------------------
//Constructors
QuestTriggerCompleteQuest::QuestTriggerCompleteQuest()
	: QuestTrigger()
{

}

QuestTriggerCompleteQuest::QuestTriggerCompleteQuest(const XMLNode& node, Quest* owningQuest)
	: QuestTrigger(node, owningQuest)
{

}

QuestTriggerCompleteQuest::QuestTriggerCompleteQuest(const QuestTriggerCompleteQuest& other, Quest* owningQuest)
	: QuestTrigger(other, owningQuest)
{

}

QuestTriggerCompleteQuest::~QuestTriggerCompleteQuest()
{
	QuestTrigger::~QuestTrigger();
}

//---------------------------------------------------------
//---------------------------------------------------------
//---------------------------------------------------------
//Setters

//---------------------------------------------------------
//---------------------------------------------------------
//---------------------------------------------------------
//Getters

//---------------------------------------------------------
//---------------------------------------------------------
//---------------------------------------------------------
//Operations
bool QuestTriggerCompleteQuest::PerformQuestTrigger()
{
	BaseGameEntity* player = BaseGameEntity::GetEditablePointerToPlayer();
	if (player == nullptr
		|| m_owningQuest == nullptr)
	{
		return false;
	}
	Player* playerPtr = dynamic_cast<Player*>(player);
	if (playerPtr == nullptr)
	{
		return false;
	}
	PlayerQuestManager* questManager = playerPtr->GetEditablePlayerQuestManagerPtr();
	if (questManager == nullptr)
	{
		return false;
	}

	std::vector<Quest*>* activeQuests = questManager->GetQuestsEditablePtr();
	std::vector<Quest*>* completedQuests = questManager->GetCompletedQuestsEditablePtr();
	if (activeQuests == nullptr
		|| completedQuests == nullptr)
	{
		return false;
	}
	for (size_t i = 0; i < activeQuests->size(); i++)
	{
		Quest* currentQuest = activeQuests->at(i);
		if (m_owningQuest == currentQuest)
		{
			activeQuests->erase(activeQuests->begin() + i);
			completedQuests->push_back(currentQuest);
			return true;
		}
	}
	return false;
}

QuestTrigger* QuestTriggerCompleteQuest::Clone(Quest* owningQuest) const
{
	return new QuestTriggerCompleteQuest(*this, owningQuest);
}

void QuestTriggerCompleteQuest::WriteQuestTriggerToString(std::string& str, int indentationAmt) const
{
	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Quest Trigger Name: Complete Quest\n";
}