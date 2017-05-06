#include "Game/GameEntities/Hair/Hair.hpp"
#include "Engine/Core/EngineXMLParser.hpp"
#include "Engine/2D/SpriteResourceHandler.hpp"
#include "Engine/Math/MathToStringUtils.hpp"
#include "Game/Common/GameDebuggingCommons.hpp"

//------------------------------------------------------------------
//------------------------------------------------------------------
//------------------------------------------------------------------
//Constructor
Hair::Hair(std::string hairName, const SpriteResource* resource, Directions4 defaultDirection)
	: BaseGameEntity(HAIR_ENTITY, resource, defaultDirection)
{
	SetAgentName(hairName);
}

Hair::Hair(std::string hairName, const XMLNode& node)
	: BaseGameEntity(HAIR_ENTITY)
{
	SetAgentName(hairName);
	for (int attrIdx = 0; attrIdx < node.nAttribute(); attrIdx++)
	{
		XMLAttribute attr = node.getAttribute(attrIdx);
		std::string lowerCaseName = MakeLower(attr.lpszName);
		if (lowerCaseName == "name")
		{
			m_name = attr.lpszValue;
		}
		else if (lowerCaseName == "defaultimage")
		{
			m_currentSprite = SpriteResourceHandler::GetSpriteResourceByName(attr.lpszValue);
		}
	}
}

Hair::~Hair()
{
	BaseGameEntity::~BaseGameEntity();
}


//------------------------------------------------------------------
//------------------------------------------------------------------
//------------------------------------------------------------------
//Setters
void Hair::SetHairColorName(const std::string& hairColorName)
{
	m_hairColorName = hairColorName;
}

void Hair::SetDialogueHairName(const std::string& hairName)
{
	m_dialogueHairName = hairName;
}

//------------------------------------------------------------------
//------------------------------------------------------------------
//------------------------------------------------------------------
//Getters

const std::string& Hair::GetHairColorName() const
{
	return m_hairColorName;
}

const std::string& Hair::GetDeialogueHairName() const
{
	return m_dialogueHairName;
}

//------------------------------------------------------------------
//------------------------------------------------------------------
//------------------------------------------------------------------
//Operations
void Hair::WriteOutEntityImportantDataToStr(std::string& str, int currentIndentationLevel) const
{
	GameDebuggingCommons::AddIndentation(str, currentIndentationLevel);
	str += "Hair: " + GetName() + "\n";
	currentIndentationLevel += 3;
	GameDebuggingCommons::AddIndentation(str, currentIndentationLevel);
	str += "Hair Color: " + m_hairColorName;
	BaseGameEntity::WriteOutEntityImportantDataToStr(str, currentIndentationLevel);
}

//------------------------------------------------------------------
//------------------------------------------------------------------
//------------------------------------------------------------------
//Updates and Render
void Hair::Update(const float& dt)
{
	BaseGameEntity::Update(dt);
}