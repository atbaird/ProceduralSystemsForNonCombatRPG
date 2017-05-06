#include "Game/Quest/QuestEvents/QuestTrigger.hpp"
#include "Game/Quest/Quest.hpp"

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//-----------------------------------------------------------------
//Constructors
QuestTrigger::QuestTrigger()
{

}

QuestTrigger::QuestTrigger(const XMLNode& node, Quest* owningQuest)
	: m_owningQuest(owningQuest)
{
	node;
}

QuestTrigger::QuestTrigger(const QuestTrigger& other, Quest* owningQuest)
	: m_owningQuest(owningQuest),
	m_TriggerName(other.m_TriggerName)
{

}

QuestTrigger::~QuestTrigger()
{

}

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//-----------------------------------------------------------------
//Setters
void QuestTrigger::SetOwningQuest(Quest* quest)
{
	m_owningQuest = quest;
}


//-----------------------------------------------------------------
//-----------------------------------------------------------------
//-----------------------------------------------------------------
//Getters

const Quest* QuestTrigger::GetOwningQuest() const
{
	return m_owningQuest;
}

Quest* QuestTrigger::GetEditableOwningQuest()
{
	return m_owningQuest;
}

//-----------------------------------------------------------------
//-----------------------------------------------------------------
//-----------------------------------------------------------------
//Operations
bool QuestTrigger::PerformQuestTrigger()
{
	return false;
}