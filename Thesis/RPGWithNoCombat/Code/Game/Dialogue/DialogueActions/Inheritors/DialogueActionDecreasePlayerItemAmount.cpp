#include "Game/Dialogue/DialogueActions/Inheritors/DialogueActionDecreasePlayerItemAmount.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Game/GameEntities/Agents/Player/Player.hpp"
#include "Game/Common/GameDebuggingCommons.hpp"

//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
//Creation Funcs
DialogueTrigger* CreateDialogueActionDecreasePlayerItemAmount(const XMLNode& node)
{
	return new DialogueActionDecreasePlayerItemAmount(node);
}

DialogueTrigger* CreateDefaultDialogueActionDecreasePlayerItemAmount()
{
	return new DialogueActionDecreasePlayerItemAmount();
}

//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
//Static Variables
STATIC const DialogueActionRegistrationHelper 
	DialogueActionDecreasePlayerItemAmount::s_DialogueActionDecreasePlayerItemAmountRegistrationHelper
		= DialogueActionRegistrationHelper("DecreasePlayerItemAmount", 
			CreateDialogueActionDecreasePlayerItemAmount,
			CreateDefaultDialogueActionDecreasePlayerItemAmount);

//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
//Constructors
DialogueActionDecreasePlayerItemAmount::DialogueActionDecreasePlayerItemAmount()
	: DialogueTrigger()
{

}

DialogueActionDecreasePlayerItemAmount::DialogueActionDecreasePlayerItemAmount(const XMLNode& node)
	: DialogueTrigger(node)
{
	for (int attrIdx = 0; attrIdx < node.nAttribute(); attrIdx++)
	{
		XMLAttribute attr = node.getAttribute(attrIdx);
		std::string attrName = MakeLower(attr.lpszName);
		if (attrName == "variable"
			|| attrName == "variableName")
		{
			SetVariableName(std::string(attr.lpszValue));
		}
		else if (attrName == "value")
		{
			std::string attrVal = MakeLower(attr.lpszValue);
			{
				m_value = (float)atof(attrVal.c_str());
			}
		}
	}
}

DialogueActionDecreasePlayerItemAmount::DialogueActionDecreasePlayerItemAmount(
	const DialogueActionDecreasePlayerItemAmount& other)
	: DialogueTrigger(other),
	m_itemName(other.m_itemName),
	m_lowerCaseItemName(other.m_lowerCaseItemName),
	m_value(other.m_value)
{

}

DialogueActionDecreasePlayerItemAmount::~DialogueActionDecreasePlayerItemAmount()
{
	DialogueTrigger::~DialogueTrigger();
}

//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
//Setters
void DialogueActionDecreasePlayerItemAmount::SetVariableName(const std::string& name)
{
	m_itemName = name;
	m_lowerCaseItemName = MakeLower(name);
}

void DialogueActionDecreasePlayerItemAmount::SetValue(float value)
{
	m_value = value;
}

//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
//Getters
const std::string& DialogueActionDecreasePlayerItemAmount::GetVariableName() const
{
	return m_itemName;
}

const std::string& DialogueActionDecreasePlayerItemAmount::GetLowerCaseVariableName() const
{
	return m_lowerCaseItemName;
}

float DialogueActionDecreasePlayerItemAmount::GetValue() const
{
	return m_value;
}

//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
//Operations
void DialogueActionDecreasePlayerItemAmount::WriteDialogueTriggerToString(std::string& str, int indentationAmt) const
{
	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Dialogue Trigger Name: Decrease Player Item Amount\n";
	indentationAmt += 3;

	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Item Name to decrease: " + m_itemName + "\n";

	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Amount to decrease by: " + std::to_string(m_value) + "\n";
}

void DialogueActionDecreasePlayerItemAmount::TriggerAction()
{
	BaseGameEntity* playerEntity = BaseGameEntity::GetEditablePointerToPlayer();
	Player* player = dynamic_cast<Player*>(playerEntity);
	if (player == nullptr 
		|| m_lowerCaseItemName == "")
	{
		return;
	}
	float originalVal = 0.f;
	player->GetItemValue(m_lowerCaseItemName, originalVal, 0.f);

	originalVal -= m_value;
	player->SetItemValue(m_lowerCaseItemName, originalVal);
}

DialogueTrigger* DialogueActionDecreasePlayerItemAmount::Clone() const
{
	return new DialogueActionDecreasePlayerItemAmount(*this);
}