#include "Game/GameEntities/Agents/NPCs/NPCBehaviors/Talking.hpp"
#include "Game/GameEntities/Agents/NPCs/NPC.hpp"
#include "Game/Common/GameDebuggingCommons.hpp"

//---------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------
//Creation Functions
STATIC NPCBehavior* DefaultCreateTalkingBehavior()
{
	return new Talking();
}

STATIC NPCBehavior* CreateTalkingBehavior(const XMLNode& node)
{
	return new Talking(node);
}

//---------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------
//Static Variables
STATIC const NPCBehaviorRegistrationHelper 
	Talking::s_TalkingBehaviorRegistration("talking", CreateTalkingBehavior, DefaultCreateTalkingBehavior);
STATIC float Talking::s_UtilityWhenNotTalking = 0.f;
STATIC float Talking::s_UtilityWhenTalking = 9999999999999.f;

//---------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------
//Constructors
Talking::Talking()
	: NPCBehavior()
{
	m_IfHaveThenMakeEntityInteractable = true;
}

Talking::Talking(const XMLNode& node)
	: NPCBehavior(node)
{
	m_IfHaveThenMakeEntityInteractable = true;
}

Talking::Talking(const Talking& other)
	: NPCBehavior(other)
{
	m_IfHaveThenMakeEntityInteractable = true;
}

Talking::~Talking()
{
	NPCBehavior::~NPCBehavior();
}

//---------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------
//Setters
void Talking::ChangeDirections()
{
	Agent* agent = m_owner->GetTalkingTo();
	if (agent == nullptr)
	{
		return;
	}
	Vector2 positionalDifference = agent->GetPosition() - m_owner->GetPosition();
	positionalDifference.normalize();
	m_owner->SetMovementDirection(positionalDifference);
}

//---------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------
//Getters
Agent* Talking::GetAgentLastTargeted() const
{
	return m_agentLastTargeted;
}

bool Talking::GetTalkingModeEnabled() const
{
	return m_talkingModeEnabled;
}

//---------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------
//Operations
void Talking::CleanUpOnUpdate()
{
	if (m_owner != nullptr)
	{
		m_owner->ResetCurrentMovementSpeedToTrueMovementSpeed();
	}
}

NPCBehavior* Talking::Clone()
{
	Talking* lookaround = new Talking(*this);
	return lookaround;
}

const float Talking::CalcUtility()
{
	if (m_owner != nullptr)
	{
		m_talkingModeEnabled = m_owner->GetTalkingToSomeone();
	}
	else
	{
		m_talkingModeEnabled = false;
	}

	if (m_talkingModeEnabled)
	{
		return s_UtilityWhenTalking;
	}
	else
	{
		return s_UtilityWhenNotTalking;
	}
}

void Talking::Run(float dt, bool ranLastFrame)
{
	dt;
	ranLastFrame;
	if (m_owner != nullptr)
	{
		m_agentLastTargeted = m_owner->GetTalkingTo();
		ChangeDirections();
	}

	if (m_owner != nullptr)
	{
		m_owner->SetCurrentMovementSpeed(0.f);
		m_owner->SetAnimationPlaySpeed(0.f);
	}
}

int Talking::WriteNPCBehaviorToString(std::string& str, int indentationLvl) const
{
	indentationLvl = NPCBehavior::WriteNPCBehaviorToString(str, indentationLvl);

	return indentationLvl;
}