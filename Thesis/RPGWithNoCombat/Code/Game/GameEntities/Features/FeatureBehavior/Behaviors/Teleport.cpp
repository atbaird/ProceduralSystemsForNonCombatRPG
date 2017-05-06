#include "Game/GameEntities/Features/FeatureBehavior/Behaviors/Teleport.hpp"
#include "Game/Map/Map.hpp"

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------
//Spawner Functions

STATIC FeatureBehavior* DefaultCreateFeatureTeleportBehavior()
{
	return new Teleport();
}

STATIC FeatureBehavior* CreateFeatureTeleportBehavior(const XMLNode& node)
{
	return new Teleport(node);
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------
//Static Variables
STATIC const FeatureBehaviorRegistrationHelper 
Teleport::s_FeatureTeleportBehaviorRegistration = FeatureBehaviorRegistrationHelper("Teleport", 
	CreateFeatureTeleportBehavior, DefaultCreateFeatureTeleportBehavior);

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------
//Constructors
Teleport::Teleport()
	: FeatureBehavior(INTERACTIVE_FEATURE_BEHAVIOR)
{

}

Teleport::Teleport(const XMLNode& node) 
	: FeatureBehavior(node, INTERACTIVE_FEATURE_BEHAVIOR)
{
	for (int attrIdx = 0; attrIdx < node.nAttribute(); attrIdx++)
	{
		XMLAttribute attr = node.getAttribute(attrIdx);
		std::string attrName = MakeLower(attr.lpszName);
		if (attrName == "tilecoords"
			|| attrName == "position"
			|| attrName == "teleportto")
		{
			bool sizeOf1Found = false;
			bool sizeOf2Found = false;
			IntVector2 vals = EngineXMLParser::ParseXMLAttributeForIntVector2(attr, sizeOf1Found, sizeOf2Found);
			if (sizeOf1Found)
			{
				m_positionToTeleportToSet = true;
				m_tilePositionToTeleportTo.x = vals.x;
			}
			if (sizeOf2Found)
			{
				m_tilePositionToTeleportTo.y = vals.y;
			}
		}
	}
}

Teleport::Teleport(const Teleport& other)
	: FeatureBehavior(other),
	m_tilePositionToTeleportTo(other.m_tilePositionToTeleportTo),
	m_positionToTeleportToSet(other.m_positionToTeleportToSet)
{

}

Teleport::~Teleport()
{
	FeatureBehavior::~FeatureBehavior();
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------
//Operations
void Teleport::CheckMapNodeForAttributesOfInterest(const XMLNode& node)
{
	std::string lowerCaseReferenceName = MakeLower(m_chosenReferenceName);
	for (int attrIdx = 0; attrIdx < node.nAttribute(); attrIdx++)
	{
		XMLAttribute attr = node.getAttribute(attrIdx);
		std::string lowerCaseAttrName = MakeLower(attr.lpszName);
		if (lowerCaseAttrName == "teleportto"
			|| lowerCaseAttrName == "teleport.tilecoords"
			|| lowerCaseAttrName == "teleport.position"
			|| lowerCaseAttrName == "teleport.teleportto"
			|| (lowerCaseReferenceName != ""
				&& (lowerCaseAttrName == (lowerCaseReferenceName + ".tilecoords")
					|| lowerCaseAttrName == (lowerCaseReferenceName + ".position")
					|| lowerCaseAttrName == (lowerCaseReferenceName + ".teleportto"))))
		{
			bool sizeOf1Found = false;
			bool sizeOf2Found = false;
			IntVector2 vals = EngineXMLParser::ParseXMLAttributeForIntVector2(attr, sizeOf1Found, sizeOf2Found);
			if (sizeOf1Found)
			{
				m_positionToTeleportToSet = true;
				m_tilePositionToTeleportTo.x = vals.x;
			}
			if (sizeOf2Found)
			{
				m_tilePositionToTeleportTo.y = vals.y;
			}
		}
	}
}

bool Teleport::EntityInteractWithFeature(BaseGameEntity* entity)
{
	Map* map = Map::GetEditableMap();
	if (FeatureBehavior::EntityInteractWithFeature(entity) == false
		|| m_positionToTeleportToSet == false
		|| map == nullptr)
	{
		return false;
	}
	const Tile* tile = map->GetTileAtCoords(m_tilePositionToTeleportTo);
	if (tile == nullptr)
	{
		return false;
	}
	Vector2 position = tile->GetPosition();
	entity->SetPosition(position);

	return true;
}

FeatureBehavior* Teleport::Clone()
{
	return new Teleport(*this);
}