#include "Game/Dialogue/DialogueActions/Inheritors/DialogueActionIncreasePlayerItemAmount.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Game/GameEntities/Agents/Player/Player.hpp"
#include "Game/Common/GameDebuggingCommons.hpp"

//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
//Creation Funcs
DialogueTrigger* CreateDialogueActionIncreasePlayerItemAmount(const XMLNode& node)
{
	return new DialogueActionIncreasePlayerItemAmount(node);
}

DialogueTrigger* CreateDefaultDialogueActionIncreasePlayerItemAmount()
{
	return new DialogueActionIncreasePlayerItemAmount();
}

//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
//Static Variables
STATIC const DialogueActionRegistrationHelper 
	DialogueActionIncreasePlayerItemAmount::s_DialogueActionIncreasePlayerItemAmountRegistrationHelper
		= DialogueActionRegistrationHelper("IncreasePlayerItemAmount",
			CreateDialogueActionIncreasePlayerItemAmount,
			CreateDefaultDialogueActionIncreasePlayerItemAmount);


//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
//Constructors
DialogueActionIncreasePlayerItemAmount::DialogueActionIncreasePlayerItemAmount()
	: DialogueTrigger()
{

}

DialogueActionIncreasePlayerItemAmount::DialogueActionIncreasePlayerItemAmount(const XMLNode& node)
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

DialogueActionIncreasePlayerItemAmount::DialogueActionIncreasePlayerItemAmount(
	const DialogueActionIncreasePlayerItemAmount& other)
	: DialogueTrigger(other),
	m_itemName(other.m_itemName),
	m_lowerCaseItemName(other.m_lowerCaseItemName),
	m_value(other.m_value)
{

}

DialogueActionIncreasePlayerItemAmount::~DialogueActionIncreasePlayerItemAmount()
{
	DialogueTrigger::~DialogueTrigger();
}

//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
//Setters
void DialogueActionIncreasePlayerItemAmount::SetVariableName(const std::string& name)
{
	m_itemName = name;
	m_lowerCaseItemName = MakeLower(name);
}

void DialogueActionIncreasePlayerItemAmount::SetValue(float value)
{
	m_value = value;
}

//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
//Getters
const std::string& DialogueActionIncreasePlayerItemAmount::GetVariableName() const
{
	return m_itemName;
}

const std::string& DialogueActionIncreasePlayerItemAmount::GetLowerCaseVariableName() const
{
	return m_lowerCaseItemName;
}

float DialogueActionIncreasePlayerItemAmount::GetValue() const
{
	return m_value;
}

//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
//Operations
void DialogueActionIncreasePlayerItemAmount::WriteDialogueTriggerToString(std::string& str, int indentationAmt) const
{
	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Dialogue Trigger Name: Increase Player Item Amount\n";
	indentationAmt += 3;

	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Item Name: " + m_itemName + "\n";

	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Value to increase by: " + std::to_string(m_value) + "\n";
}

void DialogueActionIncreasePlayerItemAmount::TriggerAction()
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

	originalVal += m_value;
	player->SetItemValue(m_lowerCaseItemName, originalVal);
}

DialogueTrigger* DialogueActionIncreasePlayerItemAmount::Clone() const
{
	return new DialogueActionIncreasePlayerItemAmount(*this);
}