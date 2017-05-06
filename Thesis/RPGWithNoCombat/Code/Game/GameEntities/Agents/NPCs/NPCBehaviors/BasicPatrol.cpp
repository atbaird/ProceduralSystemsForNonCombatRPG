#include "Game/GameEntities/Agents/NPCs/NPCBehaviors/BasicPatrol.hpp"
#include "Engine/Math/Vector2.hpp"
#include "Game/Common/GameDebuggingCommons.hpp"
#include "Engine/Math/MathToStringUtils.hpp"

//------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------
//Spawner Functions
STATIC NPCBehavior* DefaultCreateBasicPatrolBehavior()
{
	BasicPatrol* patrol = new BasicPatrol();
	return patrol;
}

STATIC NPCBehavior* CreateBasicPatrolBehavior(const XMLNode& node)
{
	BasicPatrol* patrol = new BasicPatrol(node);
	return patrol;
}

//------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------
//Static Variables
STATIC const NPCBehaviorRegistrationHelper BasicPatrol::s_BasicPatrolBehaviorRegistration 
	= NPCBehaviorRegistrationHelper("patrol", CreateBasicPatrolBehavior, DefaultCreateBasicPatrolBehavior);

//------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------
//Constructors
BasicPatrol::BasicPatrol()
	: NPCBehavior()
{

}

BasicPatrol::BasicPatrol(const XMLNode& node)
	: NPCBehavior(node)
{

}

BasicPatrol::BasicPatrol(const BasicPatrol& other)
	: NPCBehavior(other)
{

}

BasicPatrol::~BasicPatrol()
{
	NPCBehavior::~NPCBehavior();
}


//------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------
//Operations
NPCBehavior* BasicPatrol::Clone()
{
	BasicPatrol* patrol = new BasicPatrol(*this);
	return patrol;
}

const float BasicPatrol::CalcUtility()
{
	return m_lastRanUtilityAmtToRun;
}

void BasicPatrol::Run(float dt, bool ranLastFrame)
{
	ranLastFrame;
	dt;
}


int BasicPatrol::WriteNPCBehaviorToString(std::string& str, int indentationLvl) const
{
	indentationLvl = NPCBehavior::WriteNPCBehaviorToString(str, indentationLvl);
	GameDebuggingCommons::AddIndentation(str, indentationLvl);
	str += "Patrol points: ";
	int nextTier = indentationLvl + 3;
	for (size_t i = 0; i < m_patrolPoints.size(); i++)
	{
		std::string pointVal = MathToStringUtils::ToString(m_patrolPoints[i]);
		GameDebuggingCommons::AddIndentation(str, nextTier);
		str += pointVal + "\n";
	}

	return indentationLvl;
}