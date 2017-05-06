#include "Game/Quest/QuestEvents/QuestTrigger/QuestTriggerChangeDialogue.hpp"
#include "Game/Quest/Quest.hpp"
#include "Game/GameEntities/Agents/Agent.hpp"
#include "Game/Common/GameDebuggingCommons.hpp"

//------------------------------------------------------------
//------------------------------------------------------------
//------------------------------------------------------------
//Registration Funcs
QuestTrigger* QuestTriggerChangeDialogueFunc(const XMLNode& node)
{
	return new QuestTriggerChangedDialogue(node);
}

QuestTrigger* QuestTriggerDefaultChangeDialogueFunc()
{
	return new QuestTriggerChangedDialogue();
}

//------------------------------------------------------------
//------------------------------------------------------------
//------------------------------------------------------------
//Static Variables
STATIC QuestTriggerRegistration QuestTriggerChangedDialogue::s_ChangeDialogueRegistration
	= QuestTriggerRegistration("ChangeDialogue", QuestTriggerChangeDialogueFunc, QuestTriggerDefaultChangeDialogueFunc);

//------------------------------------------------------------
//------------------------------------------------------------
//------------------------------------------------------------
//Constructors
QuestTriggerChangedDialogue::QuestTriggerChangedDialogue()
	: QuestTrigger()
{

}

QuestTriggerChangedDialogue::QuestTriggerChangedDialogue(const XMLNode& node, Quest* owningQuest)
	: QuestTrigger(node, owningQuest)
{
	//ChangeDialogue
	//	<ChangeDialogue VariableName = 'MageGuard' value = 'EnteringMagesGuildYouMayPass' / >
	for (int attrIdx = 0; attrIdx < node.nAttribute(); attrIdx++)
	{
		XMLAttribute attr = node.getAttribute(attrIdx);
		std::string AttrName = MakeLower(attr.lpszName);
		if (AttrName == "entity"
			|| AttrName == "entityname")
		{
			m_EntityVariableName = std::string(attr.lpszValue);
		}
		else if (AttrName == "dialoguesegment"
			|| AttrName == "dialogue")
		{
			m_DialogueToChangeTo = std::string(attr.lpszValue);
		}
	}

}

QuestTriggerChangedDialogue::QuestTriggerChangedDialogue(const QuestTriggerChangedDialogue& other,
	Quest* owningQuest)
	: QuestTrigger(other, owningQuest),
	m_EntityVariableName(other.m_EntityVariableName),
	m_DialogueToChangeTo(other.m_DialogueToChangeTo)
{

}

QuestTriggerChangedDialogue::~QuestTriggerChangedDialogue()
{
	QuestTrigger::~QuestTrigger();
}


//------------------------------------------------------------
//------------------------------------------------------------
//------------------------------------------------------------
//Setters
void QuestTriggerChangedDialogue::SetEntityVariableName(const std::string& name)
{
	m_EntityVariableName = name;
}

void QuestTriggerChangedDialogue::SetDialogueToChangeTo(const std::string& dialogue)
{
	m_DialogueToChangeTo = dialogue;
}


//------------------------------------------------------------
//------------------------------------------------------------
//------------------------------------------------------------
//Getters
const std::string& QuestTriggerChangedDialogue::GetEntityVariableName() const
{
	return m_EntityVariableName;
}

const std::string& QuestTriggerChangedDialogue::GetDialogueToChangeTo() const
{
	return m_DialogueToChangeTo;
}

//------------------------------------------------------------
//------------------------------------------------------------
//------------------------------------------------------------
//Operations
bool QuestTriggerChangedDialogue::PerformQuestTrigger()
{
	if (m_owningQuest == nullptr)
	{
		return false;
	}
	BaseGameEntity* entity = nullptr;
	m_owningQuest->GetVariable<BaseGameEntity*>(m_EntityVariableName, entity, nullptr);
	if (entity == nullptr)
	{
		return false;
	}
	Agent* agent = dynamic_cast<Agent*>(entity);
	if (agent == nullptr)
	{
		return false;
	}
	agent->SetDialogueToOpenIfSpokenTo(m_DialogueToChangeTo);

	return true;
}

QuestTrigger* QuestTriggerChangedDialogue::Clone(Quest* owningQuest) const
{
	QuestTriggerChangedDialogue* clone = new QuestTriggerChangedDialogue(*this, owningQuest);
	return clone;
}

void QuestTriggerChangedDialogue::WriteQuestTriggerToString(std::string& str, int indentationAmt) const
{
	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Quest Trigger Name: Change Dialogue\n";
	indentationAmt += 3;

	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Entity Name to change dialogue: " + m_EntityVariableName + "\n";

	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Dialogue to change to: " + m_DialogueToChangeTo + "\n";

}