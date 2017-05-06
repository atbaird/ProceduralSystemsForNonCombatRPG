#include "Game/Dialogue/DialogueRequirements/Inheritors/DialogueRequirementNPCHasAttributeValue.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Game/Quest/QuestEvents/QuestRequirement.hpp"
#include "Game/GameEntities/Agents/Player/Player.hpp"
#include "Game/GameEntities/Agents/NPCs/NPC.hpp"
#include "Game/Common/GameDebuggingCommons.hpp"

//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//Spawning Functions
DialogueRequirement* DialogueReqNPCHasAttributeValueCreationFunc(const XMLNode& node)
{
	return new DialogueRequirementNPCHasAttributeValue(node);
}

DialogueRequirement* DialogueReqNPCHasAttributeValueDefaultCreationFunc()
{
	return new DialogueRequirementNPCHasAttributeValue();
}

//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//Static Variables
STATIC const DialogueRequirementRegistrationHelper 
	DialogueRequirementNPCHasAttributeValue::s_NPCHasAttributeValueRegHelper
		= DialogueRequirementRegistrationHelper("NPCHasAttributeValue", 
			DialogueReqNPCHasAttributeValueCreationFunc, 
			DialogueReqNPCHasAttributeValueDefaultCreationFunc);

//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//Constructors
DialogueRequirementNPCHasAttributeValue::DialogueRequirementNPCHasAttributeValue()
	: DialogueRequirement()
{

}

DialogueRequirementNPCHasAttributeValue::DialogueRequirementNPCHasAttributeValue(const XMLNode& node)
	: DialogueRequirement(node)
{
	for (int attrIdx = 0; attrIdx < node.nAttribute(); attrIdx++)
	{
		XMLAttribute attr = node.getAttribute(attrIdx);
		std::string attrName = MakeLower(attr.lpszName);
		if (attrName == "name"
			|| attrName == "attributename"
			|| attrName == "attrname")
		{
			SetNPCAttributeName(attr.lpszName);
		}
		else if (attrName == "value"
			|| attrName == "attributevalue"
			|| attrName == "attrvalue")
		{
			SetNPCAttributeValue(attr.lpszValue);
		}
	}
}

DialogueRequirementNPCHasAttributeValue::DialogueRequirementNPCHasAttributeValue(
	const DialogueRequirementNPCHasAttributeValue& other)
	: DialogueRequirement(other),
	m_NPCAttributeName(other.m_NPCAttributeName),
	m_NPCAttributeValue(other.m_NPCAttributeValue)
{

}

DialogueRequirementNPCHasAttributeValue::~DialogueRequirementNPCHasAttributeValue()
{
	DialogueRequirement::~DialogueRequirement();
}

//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//Setters
void DialogueRequirementNPCHasAttributeValue::SetNPCAttributeName(const std::string& name)
{
	m_NPCAttributeName = name;
}

void DialogueRequirementNPCHasAttributeValue::SetNPCAttributeValue(const std::string& value)
{
	m_NPCAttributeValue = value;
}

//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//Getters
const std::string& DialogueRequirementNPCHasAttributeValue::GetNPCAttributeName() const
{
	return m_NPCAttributeName;
}

const std::string& DialogueRequirementNPCHasAttributeValue::GetNPCAttributeValue() const
{
	return m_NPCAttributeValue;
}


//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//Operations
void DialogueRequirementNPCHasAttributeValue::WriteDialogueTriggerToString(std::string& str, int indentationAmt) const
{
	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Dialogue Requirement Name: NPC Has Attribute Value\n";
	indentationAmt += 3;

	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Attribute Name: " + m_NPCAttributeName + "\n";

	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Desired Attribute Value: " + m_NPCAttributeValue + "\n";
}

bool DialogueRequirementNPCHasAttributeValue::CheckIfRequirementIsMet() const
{
	const BaseGameEntity* entityPly = BaseGameEntity::GetPointerToPlayer();
	if (entityPly == nullptr)
	{
		return false;
	}
	const Player* player = dynamic_cast<const Player*>(entityPly);
	if (player == nullptr)
	{
		return false;
	}
	Agent* agent = player->GetTalkingTo();
	if (agent == nullptr)
	{
		return false;
	}
	NPC* npc = dynamic_cast<NPC*>(agent);
	if (npc == nullptr)
	{
		return false;
	}

	bool npcAttributeExists = false;
	if (!npc->DetermineNPCHasAttributeSetting(m_NPCAttributeName, m_NPCAttributeValue, npcAttributeExists))
	{
		return false;
	}

	return true;
}

DialogueRequirement* DialogueRequirementNPCHasAttributeValue::Clone() const
{
	return new DialogueRequirementNPCHasAttributeValue(*this);
}