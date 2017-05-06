#include "Game/GameEntities/Agents/NPCs/NPCBehavior.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Game/Common/GameDebuggingCommons.hpp"

//-----------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------
//Constructors
NPCBehavior::NPCBehavior()
{

}

NPCBehavior::NPCBehavior(const XMLNode& node)
{
	std::string name = node.getName();
	m_name = MakeLower(name);
}

NPCBehavior::NPCBehavior(const NPCBehavior& other)
	: m_name(other.m_name),
	m_lastRanUtilityAmtToRun(other.m_lastRanUtilityAmtToRun),
	m_canRun(other.m_canRun),
	m_owner(other.m_owner)
{
}

NPCBehavior::~NPCBehavior()
{

}

//-----------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------
//Setters
void NPCBehavior::SetName(const std::string& name, bool ensureIsLowerCase)
{
	std::string nam = name;
	if (ensureIsLowerCase)
	{
		nam = MakeLower(nam);
	}
	m_name = nam;
}

void NPCBehavior::SetLastRanUtilityAmountToRun(float amt)
{
	m_lastRanUtilityAmtToRun = amt;
}

void NPCBehavior::SetOwner(NPC* owner)
{
	m_owner = owner;
}

void NPCBehavior::SetCanRun(bool run)
{
	m_canRun = run;
}


//-----------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------
//Getters
const std::string& NPCBehavior::GetName() const
{
	return m_name;
}

float NPCBehavior::GetRanUtilityAmountToRun() const
{
	return m_lastRanUtilityAmtToRun;
}

const NPC* NPCBehavior::GetOwner() const
{
	return m_owner;
}

NPC* NPCBehavior::GetEditableOwner() const
{
	return m_owner;
}

const bool NPCBehavior::GetIfHaveThenMakeEntityInteractable() const
{
	return m_IfHaveThenMakeEntityInteractable;
}

//-----------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------
//Operations
const bool NPCBehavior::DoesPassChanceToRun() const
{
	return m_canRun;
}

void NPCBehavior::CleanUpOnUpdate()
{

}


int NPCBehavior::WriteNPCBehaviorToString(std::string& str, int indentationLvl) const
{
	GameDebuggingCommons::AddIndentation(str, indentationLvl);
	str += "NPC Behavior Name: " + m_name + "\n";

	indentationLvl += 3;
	GameDebuggingCommons::AddIndentation(str, indentationLvl);
	str += "Last utility score: " + std::to_string(m_lastRanUtilityAmtToRun) + "\n";
	return indentationLvl;
}