#include "Game/Quest/QuestTemplate.hpp"
#include "Engine/Core/EngineXMLParser.hpp"
#include "Game/GameEntities/Agents/NPCs/NPC.hpp"
#include "Game/GameEntities/Agents/Player/Player.hpp"
#include "Game/Map/MapFiles/MapFiles.hpp"
#include "Game/Quest/Quest.hpp"
#include "Game/Map/Map.hpp"
#include "Game/Common/GameDebuggingCommons.hpp"
#include "Game/Debugging/NamedPropertiesToString.hpp"

//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//Constructors
QuestTemplate::QuestTemplate()
{

}

QuestTemplate::QuestTemplate(const XMLNode& node)
{
	m_questTitle = node.getName();
	ReadQuestXMLNodeAttributes(node);
	ReadQuestXMLNodeChildren(node);
}

QuestTemplate::QuestTemplate(const QuestTemplate& other)
	: m_QuestVariables(other.m_QuestVariables),
	m_questTitle(other.m_questTitle),
	m_DialogueVersionOfQuestTitle(other.m_DialogueVersionOfQuestTitle),
	m_AgentsToGenerate(other.m_AgentsToGenerate),
	m_FeaturesToGenerate(other.m_FeaturesToGenerate),
	m_SpecialVariablesToKeepTrackOf(other.m_SpecialVariablesToKeepTrackOf)
{
	for (size_t idx = 0; idx < other.m_QuestEvents.size(); idx++)
	{
		QuestEvent* event = other.m_QuestEvents[idx];
		if (event != nullptr)
		{
			m_QuestEvents.push_back(new QuestEvent(*event, nullptr));
		}
	}
}


QuestTemplate::~QuestTemplate()
{
	for (size_t i = 0; i < m_QuestEvents.size(); i++)
	{
		QuestEvent* questEvent = m_QuestEvents[i];
		if (questEvent != nullptr)
		{
			delete questEvent;
		}
	}
	m_QuestEvents.clear();
}


void QuestTemplate::ReadQuestXMLNodeAttributes(const XMLNode& node)
{
	for (int attrIdx = 0; attrIdx < node.nAttribute(); attrIdx++)
	{
		XMLAttribute attr = node.getAttribute(attrIdx);
		std::string attrName = MakeLower(attr.lpszName);
		if (attrName == "title")
		{
			m_DialogueVersionOfQuestTitle = std::string(attr.lpszValue);
		}
	}
}

void QuestTemplate::ReadQuestXMLNodeChildren(const XMLNode& node)
{
	for (int childIdx = 0; childIdx < node.nChildNode(); childIdx++)
	{
		XMLNode child = node.getChildNode(childIdx);
		std::string childName = MakeLower(child.getName());
		if (childName == "variables")
		{
			ReadVariablesXMLNodeChildren(child);
		}
		else if (childName == "agentstogenerate")
		{
			ReadAgentsToGenerateXMLNodeChildren(child);
		}
		else if (childName == "featurestogenerate")
		{
			ReadFeaturesToGenerateXMLNodeChildren(child);
		}
		else if (childName == "events")
		{
			ReadEventsXMLNodeChildren(child);
		}
	}
}

void QuestTemplate::ReadVariablesXMLNodeChildren(const XMLNode& node)
{
	for (int childIdx = 0; childIdx < node.nChildNode(); childIdx++)
	{
		XMLNode variable = node.getChildNode(childIdx);
		std::string variableType = MakeLower(variable.getName());
		std::string varName = "";
		if (variableType == "player")
		{
			BaseGameEntity* playerEntity = nullptr;
			for (int attrIdx = 0; attrIdx < variable.nAttribute(); attrIdx++)
			{
				XMLAttribute attr = variable.getAttribute(attrIdx);
				std::string attrName = MakeLower(attr.lpszName);
				if (attrName == "variablename"
					|| attrName == "variable"
					|| attrName == "name")
				{
					varName = MakeLower(std::string(attr.lpszValue));
				}
			}
			if (varName != "")
			{
				SpecialVariableTypes playerRegistrySlot;
				playerRegistrySlot.SetSpecialVariableTypeEnum(IS_PLAYER);
				playerRegistrySlot.SetVariableName(varName);
				m_SpecialVariablesToKeepTrackOf.push_back(playerRegistrySlot);
				m_QuestVariables.Set<BaseGameEntity*>(varName, playerEntity);
			}
		}
		else if (variableType == "npc")
		{
			BaseGameEntity* npcEntity = nullptr;
			for (int attrIdx = 0; attrIdx < variable.nAttribute(); attrIdx++)
			{
				XMLAttribute attr = variable.getAttribute(attrIdx);
				std::string attrName = MakeLower(attr.lpszName);
				if (attrName == "variablename"
					|| attrName == "variable"
					|| attrName == "name")
				{
					varName = MakeLower(std::string(attr.lpszValue));
				}
			}
			if (varName != "")
			{
				SpecialVariableTypes npcRegistrySlot;
				npcRegistrySlot.SetSpecialVariableTypeEnum(IS_AN_NPC);
				npcRegistrySlot.SetVariableName(varName);
				m_SpecialVariablesToKeepTrackOf.push_back(npcRegistrySlot);
				m_QuestVariables.Set<BaseGameEntity*>(varName, npcEntity);
			}
		}
		else if (variableType == "feature")
		{
			BaseGameEntity* featureEntity = nullptr;
			for (int attrIdx = 0; attrIdx < variable.nAttribute(); attrIdx++)
			{
				XMLAttribute attr = variable.getAttribute(attrIdx);
				std::string attrName = MakeLower(attr.lpszName);
				if (attrName == "variablename"
					|| attrName == "variable"
					|| attrName == "name")
				{
					varName = MakeLower(std::string(attr.lpszValue));
				}
			}
			if (varName != "")
			{
				SpecialVariableTypes npcRegistrySlot;
				npcRegistrySlot.SetSpecialVariableTypeEnum(IS_A_FEATURE);
				npcRegistrySlot.SetVariableName(varName);
				m_SpecialVariablesToKeepTrackOf.push_back(npcRegistrySlot);
				m_QuestVariables.Set<BaseGameEntity*>(varName, featureEntity);
			}
		}
		else if (variableType == "text")
		{
			std::string var = "";
			for (int attrIdx = 0; attrIdx < variable.nAttribute(); attrIdx++)
			{
				XMLAttribute attr = variable.getAttribute(attrIdx);
				std::string attrName = MakeLower(attr.lpszName);
				if (attrName == "variablename"
					|| attrName == "variable"
					|| attrName == "name")
				{
					varName = MakeLower(std::string(attr.lpszValue));
				}
				else if (attrName == "defaultvalue"
					|| attrName == "value")
				{
					var = std::string(attr.lpszValue);
				}
			}
			if (varName != "")
			{
				m_QuestVariables.Set<std::string>(varName, var);
			}
		}
		else if(variableType == "float")
		{
			float var = 0.f;
			for (int attrIdx = 0; attrIdx < variable.nAttribute(); attrIdx++)
			{
				XMLAttribute attr = variable.getAttribute(attrIdx);
				std::string attrName = MakeLower(attr.lpszName);
				if (attrName == "variablename"
					|| attrName == "variable"
					|| attrName == "name")
				{
					varName = MakeLower(std::string(attr.lpszValue));
				}
				else if (attrName == "defaultvalue"
					|| attrName == "value")
				{
					var = (float)atof(attr.lpszValue);
				}
			}
			if (varName != "")
			{
				m_QuestVariables.Set<float>(varName, var);
			}
		}
		else if (variableType == "bool"
			|| variableType == "boolean")
		{
			bool var = false;
			for (int attrIdx = 0; attrIdx < variable.nAttribute(); attrIdx++)
			{
				XMLAttribute attr = variable.getAttribute(attrIdx);
				std::string attrName = MakeLower(attr.lpszName);
				if (attrName == "variablename"
					|| attrName == "variable"
					|| attrName == "name")
				{
					varName = MakeLower(std::string(attr.lpszValue));
				}
				else if (attrName == "defaultvalue"
					|| attrName == "value")
				{
					std::string attrVal = MakeLower(attr.lpszValue);
					if (attrVal == "false"
						|| attrVal == "f")
					{
						var = false;
					}
					else if (attrVal == "true"
						|| attrVal == "t")
					{
						var = true;
					}
				}
			}
			if (varName != "")
			{
				m_QuestVariables.Set<bool>(varName, var);
			}
		}
	}
}

void QuestTemplate::ReadAgentsToGenerateXMLNodeChildren(const XMLNode& node)
{
	for (int childIdx = 0; childIdx < node.nChildNode(); childIdx++)
	{
		XMLNode agent = node.getChildNode(childIdx);
		m_AgentsToGenerate.push_back(agent);
	}
}

void QuestTemplate::ReadFeaturesToGenerateXMLNodeChildren(const XMLNode& node)
{
	for (int childIdx = 0; childIdx < node.nChildNode(); childIdx++)
	{
		XMLNode feature = node.getChildNode(childIdx);
		m_FeaturesToGenerate.push_back(feature);
	}
}

void QuestTemplate::ReadEventsXMLNodeChildren(const XMLNode& node)
{
	for (int childIdx = 0; childIdx < node.nChildNode(); childIdx++)
	{
		XMLNode event_ = node.getChildNode(childIdx);
		QuestEvent* questEvent = new QuestEvent(event_);
		m_QuestEvents.push_back(questEvent);
	}
}

//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//Setters
void QuestTemplate::SetQuestTitle(const std::string& questTitle)
{
	m_questTitle = questTitle;
}

void QuestTemplate::AddQuestEvent(QuestEvent* questEvent)
{
	if (questEvent == nullptr)
	{
		return;
	}
	m_QuestEvents.push_back(questEvent);
}

void QuestTemplate::SetNamedProperties(const NamedProperties& questVariables)
{
	m_QuestVariables = questVariables;
}

//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//Getters
const NamedProperties& QuestTemplate::GetNamedProperties() const
{
	return m_QuestVariables;
}

const std::string& QuestTemplate::GetQuestTitle() const
{
	return m_questTitle;
}

const std::vector<QuestEvent*>& QuestTemplate::GetQuestEvents() const
{
	return m_QuestEvents;
}

const std::vector<QuestEvent*>* QuestTemplate::GetQuestEventsPtr() const
{
	return &m_QuestEvents;
}

std::vector<QuestEvent*>* QuestTemplate::GetQuestEventsEditablePtr()
{
	return &m_QuestEvents;
}

//------------------------------------------------------------------
//------------------------------------------------------------------
//------------------------------------------------------------------
//Operations
void QuestTemplate::CopyVariablesOntoQuest(Quest* quest, const XMLNode& mapQuestNode,
	const std::string& currentMapFile, MapQuestImportantData& data,
	MapFiles* files) const
{
	if (quest == nullptr)
	{
		return;
	}

	for (size_t questEventIdx = 0; questEventIdx < m_QuestEvents.size(); questEventIdx++)
	{
		QuestEvent* orgEvent = m_QuestEvents[questEventIdx];
		if (orgEvent == nullptr)
		{
			continue;
		}
		QuestEvent* copy = new QuestEvent(*orgEvent, quest);
		quest->AddQuestEvent(copy);
		
	}
	for (size_t i = 0; i < m_SpecialVariablesToKeepTrackOf.size(); i++)
	{
		const SpecialVariableTypes& varType = m_SpecialVariablesToKeepTrackOf[i];
		if (varType.GetSpecialVariableType() == IS_PLAYER)
		{
			quest->SetVariableNameForPlayer(varType.GetVariableName());
		}
	}
	quest->SetQuestTitle(m_questTitle);

	//Add in Agent and feature generation stuff here, as well as setting of pointers.
	quest->CopyQuestVariables(&m_QuestVariables);

	{
		for (size_t i = 0; i < m_AgentsToGenerate.size(); i++)
		{
			XMLNode agentNode = m_AgentsToGenerate[i];
			MapFiles::LoadAgentNodeFromQuest(agentNode, mapQuestNode, data, m_questTitle, 
				currentMapFile, files);
		}

		for (size_t i = 0; i < m_FeaturesToGenerate.size(); i++)
		{
			XMLNode featruesNode = m_FeaturesToGenerate[i];
			std::string entityName = "";

			MapFiles::LoadFeatureNodeFromQuest(featruesNode, mapQuestNode, data, m_questTitle,
				currentMapFile, files);
		}
	}
}


void QuestTemplate::WriteQuestTemplateToString(std::string& str, int indentationAmt) const
{
	int nextLevel = indentationAmt + 3;
	int levelAfterThat = nextLevel + 3;
	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Quest Variables:\n";
	NamedPropertiesToString::ConvertAllKnownNamedPropertyValuesToString(&m_QuestVariables, 
		str, nextLevel);

	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Agents to Generate:\n";
	for (size_t agentIdx = 0; agentIdx < m_AgentsToGenerate.size(); agentIdx++)
	{
		const XMLNode& node = m_AgentsToGenerate[agentIdx];
		std::string AgentGeneratorName = node.getName();
		std::string variableName = "";
		std::string globalVariableName = "";
		bool forceSpawnNew = false;

		for (int attrIdx = 0; attrIdx < node.nAttribute(); attrIdx++)
		{
			XMLAttribute attr = node.getAttribute(attrIdx);
			std::string attrName = MakeLower(attr.lpszName);
			if (attrName == "variablename"
				|| attrName == "variable")
			{
				variableName = MakeLower(attr.lpszValue);
			}
			else if (attrName == "alwaysspawnnew"
				|| attrName == "spawnnew"
				|| attrName == "forcespawnnew")
			{
				std::string val = MakeLower(attr.lpszValue);
				if (val == "true"
					|| val == "t")
				{
					forceSpawnNew = true;
				}
				else if (val == "false"
					|| val == "f")
				{
					forceSpawnNew = false;
				}
			}
			else if (attrName == "globalvariablename"
				|| attrName == "globalvariable")
			{
				globalVariableName = MakeLower(attr.lpszValue);
			}
		}
		GameDebuggingCommons::AddIndentation(str, nextLevel);
		str += "Agent Generator to Use: " + AgentGeneratorName + "\n";

		GameDebuggingCommons::AddIndentation(str, levelAfterThat);
		str += "Variable name: " + variableName + "\n";
		GameDebuggingCommons::AddIndentation(str, levelAfterThat);
		str += "Global variable name: " + globalVariableName + "\n";
		GameDebuggingCommons::AddIndentation(str, levelAfterThat);
		str += "Force spawn as new: ";
		GameDebuggingCommons::WriteBoolOntoString(str, forceSpawnNew);
	}

	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Features to Generate:\n";
	for (size_t agentIdx = 0; agentIdx < m_FeaturesToGenerate.size(); agentIdx++)
	{
		const XMLNode& node = m_FeaturesToGenerate[agentIdx];
		std::string AgentGeneratorName = node.getName();
		std::string variableName = "";
		std::string globalVariableName = "";
		bool forceSpawnNew = true;

		for (int attrIdx = 0; attrIdx < node.nAttribute(); attrIdx++)
		{
			XMLAttribute attr = node.getAttribute(attrIdx);
			std::string attrName = MakeLower(attr.lpszName);
			if (attrName == "variablename"
				|| attrName == "variable")
			{
				variableName = MakeLower(attr.lpszValue);
			}
			else if (attrName == "alwaysspawnnew"
				|| attrName == "spawnnew"
				|| attrName == "forcespawnnew")
			{
				std::string val = MakeLower(attr.lpszValue);
				if (val == "true"
					|| val == "t")
				{
					forceSpawnNew = true;
				}
				else if (val == "false"
					|| val == "f")
				{
					forceSpawnNew = false;
				}
			}
			else if (attrName == "globalvariablename"
				|| attrName == "globalvariable")
			{
				globalVariableName = MakeLower(attr.lpszValue);
			}
		}
		GameDebuggingCommons::AddIndentation(str, nextLevel);
		str += "Feature Generator to Use: " + AgentGeneratorName + "\n";

		GameDebuggingCommons::AddIndentation(str, levelAfterThat);
		str += "Variable name: " + variableName + "\n";
		GameDebuggingCommons::AddIndentation(str, levelAfterThat);
		str += "Global variable name: " + globalVariableName + "\n";
		GameDebuggingCommons::AddIndentation(str, levelAfterThat);
		str += "Force spawn as new: ";
		GameDebuggingCommons::WriteBoolOntoString(str, forceSpawnNew);
	}

	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Quest Events:\n";
	for (size_t i = 0; i < m_QuestEvents.size(); i++)
	{
		const QuestEvent* event = m_QuestEvents[i];
		if (event != nullptr)
		{
			event->WriteQuestEventToString(str, nextLevel);
		}
	}
}