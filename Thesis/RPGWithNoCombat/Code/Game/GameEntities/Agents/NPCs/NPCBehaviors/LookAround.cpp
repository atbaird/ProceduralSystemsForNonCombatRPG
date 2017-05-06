#include "Game/GameEntities/Agents/NPCs/NPCBehaviors/LookAround.hpp"
#include "Game/GameEntities/Agents/NPCs/NPC.hpp"
#include "Engine/Math/ExtendedMath.hpp"
#include "Game/Common/GameDebuggingCommons.hpp"

//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------
//Spawner Functions
STATIC NPCBehavior* DefaultCreateLookAroundBehavior()
{
	LookAround* lookaround = new LookAround();
	return lookaround;
}

STATIC NPCBehavior* CreateLookAroundBehavior(const XMLNode& node)
{
	LookAround* lookaround = new LookAround(node);
	return lookaround;
}

//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------
//Static Variables
STATIC const NPCBehaviorRegistrationHelper LookAround::s_LookAroundBehaviorRegistration 
	= NPCBehaviorRegistrationHelper("LookAround", CreateLookAroundBehavior, DefaultCreateLookAroundBehavior);
STATIC const float LookAround::s_DefaultUtilityScore = 0.f;

//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------
//Constructors
LookAround::LookAround() 
	: NPCBehavior()
{
	m_lastRanUtilityAmtToRun = s_DefaultUtilityScore;
}

LookAround::LookAround(const XMLNode& node)
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
			bool sizeOf2Found = false;
			bool sizeOf1Found = false;
			Vector2 vals = EngineXMLParser::ParseXMLAttributeForVector2(attr, sizeOf1Found, sizeOf2Found);
			if (sizeOf1Found)
			{
				m_minSecondsBeforeChangeMovementDirection = vals.x;
			}
			if (sizeOf2Found)
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

LookAround::LookAround(const LookAround& other) 
	: NPCBehavior(other),
	m_minSecondsBeforeChangeMovementDirection(other.m_minSecondsBeforeChangeMovementDirection),
	m_additionalSecondsBeforeChangeDir(other.m_additionalSecondsBeforeChangeDir),
	m_secondsLeftBeforeChangeDirections(0.f)
{
	m_lastRanUtilityAmtToRun = other.m_lastRanUtilityAmtToRun;
}

LookAround::~LookAround()
{
	NPCBehavior::~NPCBehavior();
}


//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------
//Setters
void LookAround::SetMinimumSecondsBeforeChangeMovementDirection(float seconds)
{
	m_minSecondsBeforeChangeMovementDirection = seconds;
}

void LookAround::SetMaximumSecondsBeforeChangeMovementDirection(float seconds)
{
	SetAdditionalSecondsBeforeChangeMovementDirection(seconds - m_minSecondsBeforeChangeMovementDirection);
}

void LookAround::SetAdditionalSecondsBeforeChangeMovementDirection(float seconds)
{
	m_additionalSecondsBeforeChangeDir = seconds;
	if (m_additionalSecondsBeforeChangeDir < 0.f)
	{
		m_additionalSecondsBeforeChangeDir = 0.f;
	}
}

void LookAround::SetSecondsBeforeChangeMovementDirection(const Vector2& timeRange)
{
	m_minSecondsBeforeChangeMovementDirection = timeRange.x;
	SetAdditionalSecondsBeforeChangeMovementDirection(timeRange.y - timeRange.x);
}

void LookAround::ChangeDirections()
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

//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------
//Getters
float LookAround::GetMinimumSecondsBeforeChangeMovementDirection() const
{
	return m_minSecondsBeforeChangeMovementDirection;
}

float LookAround::GetAdditionalSecondsBeforeChangeDirection() const
{
	return m_additionalSecondsBeforeChangeDir;
}

float LookAround::GetMaximumSecondsBeforeChangeMovementDirection() const
{
	return m_minSecondsBeforeChangeMovementDirection + m_additionalSecondsBeforeChangeDir;
}

float LookAround::GetSecondsLeftBeforeChangeDirections() const
{
	return m_secondsLeftBeforeChangeDirections;
}

//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------
//Operations
NPCBehavior* LookAround::Clone()
{
	LookAround* lookaround = new LookAround(*this);
	return lookaround;
}

const float LookAround::CalcUtility()
{
	return m_lastRanUtilityAmtToRun;
}

void LookAround::CleanUpOnUpdate()
{
	if (m_owner != nullptr)
	{
		m_owner->ResetCurrentMovementSpeedToTrueMovementSpeed();
	}
}

void LookAround::Run(float dt, bool ranLastFrame)
{
	ranLastFrame;
	m_secondsLeftBeforeChangeDirections -= (dt);
	if (m_secondsLeftBeforeChangeDirections <= 0.f)
	{
		ChangeDirections();
		if (ranLastFrame == false)
		{
			m_finishedRunningOnce = true;
		}
	}
	if (m_owner != nullptr)
	{
		m_owner->SetCurrentMovementSpeed(0.f);
		m_owner->SetAnimationPlaySpeed(0.f);
	}
}

int LookAround::WriteNPCBehaviorToString(std::string& str, int indentationLvl) const
{
	indentationLvl = NPCBehavior::WriteNPCBehaviorToString(str, indentationLvl);
	GameDebuggingCommons::AddIndentation(str, indentationLvl);
	str += "Minimum seconds before change direction: " + std::to_string(m_minSecondsBeforeChangeMovementDirection) + "\n";
	GameDebuggingCommons::AddIndentation(str, indentationLvl);
	str += "Additional seconds range before change direction" + std::to_string(m_additionalSecondsBeforeChangeDir) + "\n";

	return indentationLvl;
}