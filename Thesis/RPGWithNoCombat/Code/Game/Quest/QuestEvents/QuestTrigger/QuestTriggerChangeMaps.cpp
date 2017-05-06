#include "Game/Quest/QuestEvents/QuestTrigger/QuestTriggerChangeMaps.hpp"
#include "Game/Quest/Quest.hpp"
#include "Game/Map/MapFiles/MapFiles.hpp"
#include "Game/Common/GameDebuggingCommons.hpp"

//-------------------------------------------------------------
//-------------------------------------------------------------
//-------------------------------------------------------------
//Registration Funcs
QuestTrigger* QuestTriggerChangeMapsFunc(const XMLNode& node)
{
	return new QuestTriggerChangeMaps(node);
}

QuestTrigger* QuestTriggerDefaultChangeMapsFunc()
{
	return new QuestTriggerChangeMaps();
}

//-------------------------------------------------------------
//-------------------------------------------------------------
//-------------------------------------------------------------
//Static Variables
STATIC QuestTriggerRegistration 
	QuestTriggerChangeMaps::s_ChangeMapsRegistration 
		= QuestTriggerRegistration("", 
			QuestTriggerChangeMapsFunc, QuestTriggerDefaultChangeMapsFunc);

//-------------------------------------------------------------
//-------------------------------------------------------------
//-------------------------------------------------------------
//Constructors
QuestTriggerChangeMaps::QuestTriggerChangeMaps()
	: QuestTrigger()
{

}

QuestTriggerChangeMaps::QuestTriggerChangeMaps(const XMLNode& node, Quest* owningQuest)
	: QuestTrigger(node, owningQuest)
{
	//ChangeMaps
	//	<ChangeMaps MapFile="">
	//		<EntityPosition VariableName="player" position="0,0">
	//	</ChangeMaps>
}

QuestTriggerChangeMaps::QuestTriggerChangeMaps(const QuestTriggerChangeMaps& other, Quest* owningQuest)
	: QuestTrigger(other, owningQuest),
	m_MapFileToChangeTo(other.m_MapFileToChangeTo)
{

}

QuestTriggerChangeMaps::~QuestTriggerChangeMaps()
{
	QuestTrigger::~QuestTrigger();
}

//-------------------------------------------------------------
//-------------------------------------------------------------
//-------------------------------------------------------------
//Setters
void QuestTriggerChangeMaps::SetMapFileToChangeTo(const std::string& mapFile)
{
	m_MapFileToChangeTo = mapFile;
}

//-------------------------------------------------------------
//-------------------------------------------------------------
//-------------------------------------------------------------
//Getters
const std::string& QuestTriggerChangeMaps::GetMapFile() const
{
	return m_MapFileToChangeTo;
}

//-------------------------------------------------------------
//-------------------------------------------------------------
//-------------------------------------------------------------
//Operations
bool QuestTriggerChangeMaps::PerformQuestTrigger()
{
	if (m_owningQuest == nullptr)
	{
		return false;
	}

	MapFiles::LoadMapByNameAllAtOnce(m_MapFileToChangeTo);
	return true;
}

QuestTrigger* QuestTriggerChangeMaps::Clone(Quest* owningQuest) const
{
	QuestTriggerChangeMaps* clone = new QuestTriggerChangeMaps(*this, owningQuest);
	return clone;
}

void QuestTriggerChangeMaps::WriteQuestTriggerToString(std::string& str, int indentationAmt) const
{
	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Quest Trigger Name: Change Maps\n";
	indentationAmt += 3;

	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Map to change to: " + m_MapFileToChangeTo + "\n";
}