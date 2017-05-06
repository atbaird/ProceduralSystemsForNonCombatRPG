#include "Game/GameEntities/Agents/NPCs/NPCBehaviors/Wander.hpp"
#include "Game/GameEntities/Agents/NPCs/NPC.hpp"
#include "Engine/Math/ExtendedMath.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Engine/Math/Vector2.hpp"
#include "Engine/Core/EngineXMLParser.hpp"
#include "Game/Common/GameDebuggingCommons.hpp"

//Spawner Functions
STATIC NPCBehavior* DefaultCreateWanderBehavior()
{
	return new Wander();
}

STATIC NPCBehavior* CreateWanderBehavior(const XMLNode& node)
{
	return new Wander(node);
}

//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//Static Variables
STATIC const NPCBehaviorRegistrationHelper Wander::s_WanderBehaviorRegistration = NPCBehaviorRegistrationHelper("Wander", CreateWanderBehavior, DefaultCreateWanderBehavior);

//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//Constructors
Wander::Wander()
	: NPCBehavior()
{
	m_lastRanUtilityAmtToRun = s_DefaultUtilityScore;
}

Wander::Wander(const XMLNode& node)
	: NPCBehavior(node)
{
	m_lastRanUtilityAmtToRun = s_DefaultUtilityScore;
	float maxSeconds = m_minSecondsBeforeChangeMovementDirection + m_additionalSecondsBeforeChangeDir;

	for (int attrIdx = 0; attrIdx < node.nAttribute(); attrIdx++)
	{
		XMLAttribute attr = node.getAttribute(attrIdx);
		std::string lowerCaseName = MakeLower(attr.lpszName);
		if (lowerCaseName == "secondsbeforechangemovementdirection")
		{
			bool foundOne = false;
			bool foundTwo = false;
			Vector2 vals = EngineXMLParser::ParseXMLAttributeForVector2(attr, foundOne, foundTwo);
			if (foundOne)
			{
				m_minSecondsBeforeChangeMovementDirection = vals.x;
			}
			if (foundTwo)
			{
				maxSeconds = vals.y;
			}
		}
		else if (lowerCaseName == "minimumseconds" || lowerCaseName == "minimumsecondsbeforechangemovementdirection")
		{
			m_minSecondsBeforeChangeMovementDirection = (float)atof(attr.lpszValue);
		}
		else if (lowerCaseName == "maximumseconds" || lowerCaseName == "maximumsecondsbeforechangemovementdirection")
		{
			maxSeconds = (float)atof(attr.lpszValue);
		}
	}

	m_additionalSecondsBeforeChangeDir = maxSeconds - m_minSecondsBeforeChangeMovementDirection;
	if (m_additionalSecondsBeforeChangeDir < 0.f)
	{
		m_additionalSecondsBeforeChangeDir = 0.f;
	}
}

Wander::Wander(const Wander& other)
	: NPCBehavior(other),
	m_minSecondsBeforeChangeMovementDirection(other.m_minSecondsBeforeChangeMovementDirection),
	m_additionalSecondsBeforeChangeDir(other.m_additionalSecondsBeforeChangeDir),
	m_secondsLeftBeforeChangeDirections(0.f)
{
	m_lastRanUtilityAmtToRun = other.m_lastRanUtilityAmtToRun;
}

Wander::~Wander()
{
	NPCBehavior::~NPCBehavior();
}

//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//Setters
void Wander::SetMinimumSecondsBeforeChangeMovementDirection(float seconds)
{
	m_minSecondsBeforeChangeMovementDirection = seconds;
}

void Wander::SetMaximumSecondsBeforeChangeMovementDirection(float seconds)
{
	SetAdditionalSecondsBeforeChangeMovementDirection(seconds - m_minSecondsBeforeChangeMovementDirection);
}

void Wander::SetAdditionalSecondsBeforeChangeMovementDirection(float seconds)
{
	m_additionalSecondsBeforeChangeDir = seconds;
	if (m_additionalSecondsBeforeChangeDir < 0.f)
	{
		m_additionalSecondsBeforeChangeDir = 0.f;
	}
}

void Wander::SetSecondsBeforeChangeMovementDirection(const Vector2& timeRange)
{
	m_minSecondsBeforeChangeMovementDirection = timeRange.x;
	SetAdditionalSecondsBeforeChangeMovementDirection(timeRange.y - timeRange.x);
}

void Wander::ChangeDirections()
{
	float randomPercentage = GetRandomFloatZeroToOne();
	m_secondsLeftBeforeChangeDirections = (randomPercentage * m_additionalSecondsBeforeChangeDir)
		+ m_minSecondsBeforeChangeMovementDirection;

	if (m_owner != nullptr)
	{
		int enumVal = GetRandomIntLessThan(NUMBER_OF_DIRECTIONS8);
		Vector2 direction = Directions::GetDirectionForDirectionEnum((Directions8)enumVal);
		m_owner->SetMovementDirection(direction);
	}
}

//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//Getters
float Wander::GetMinimumSecondsBeforeChangeMovementDirection() const
{
	return m_minSecondsBeforeChangeMovementDirection;
}

float Wander::GetAdditionalSecondsBeforeChangeDirection() const
{
	return m_additionalSecondsBeforeChangeDir;
}

float Wander::GetMaximumSecondsBeforeChangeMovementDirection() const
{
	return m_minSecondsBeforeChangeMovementDirection + m_additionalSecondsBeforeChangeDir;
}

float Wander::GetSecondsLeftBeforeChangeDirections() const
{
	return m_secondsLeftBeforeChangeDirections;
}

//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//Operations
NPCBehavior* Wander::Clone()
{
	Wander* wander = new Wander(*this);
	return wander;
}

const float Wander::CalcUtility()
{
	return m_lastRanUtilityAmtToRun;
}

void Wander::Run(float dt, bool ranLastFrame)
{
	if (m_owner != nullptr)
	{
		m_owner->ResetCurrentMovementSpeedToTrueMovementSpeed();
	}
	m_secondsLeftBeforeChangeDirections -= (dt);
	if (m_secondsLeftBeforeChangeDirections <= 0.f)
	{
		ChangeDirections();
		if (ranLastFrame == false)
		{
			m_finishedRunningOnce = true;
		}
	}
	else
	{
		m_finishedRunningOnce = false;
	}

}


int Wander::WriteNPCBehaviorToString(std::string& str, int indentationLvl) const
{
	indentationLvl = NPCBehavior::WriteNPCBehaviorToString(str, indentationLvl);
	GameDebuggingCommons::AddIndentation(str, indentationLvl);
	str += "Minimum seconds before change direction: " + std::to_string(m_minSecondsBeforeChangeMovementDirection) + "\n";
	GameDebuggingCommons::AddIndentation(str, indentationLvl);
	str += "Additional seconds range before change direction: " + std::to_string(m_additionalSecondsBeforeChangeDir) + "\n";

	return indentationLvl;
}