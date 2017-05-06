#include "Game/GameEntities/Features/FeatureBehavior/FeatureBehavior.hpp"
#include "Game/Enums/GameEntityType.hpp"
#include "Engine/Core/EngineXMLParser.hpp"
#include "Game/GameEntities/BaseGameEntity.hpp"
#include "Game/Common/GameDebuggingCommons.hpp"
#include "Game/Map/Map.hpp"

//-------------------------------------------------------
//-------------------------------------------------------
//-------------------------------------------------------
//Constructors
FeatureBehavior::FeatureBehavior(DefinedFeatureBehaviorIdxes featureDefault)
	: m_desiredBehaviorType(featureDefault)
{
	if (m_desiredBehaviorType != -1)
	{
		m_desiredBehaviorTypeNotSet = false;
	}
	ResetEntitiesCanInteractWith();
}

FeatureBehavior::FeatureBehavior(const XMLNode& node, DefinedFeatureBehaviorIdxes featureDefault)
	: m_desiredBehaviorType(featureDefault)
{
	if (m_desiredBehaviorType != -1)
	{
		m_desiredBehaviorTypeNotSet = false;
	}
	//<SwitchSides type = 'Interactive' / >
	ResetEntitiesCanInteractWith();
	m_featureName = node.getName();

	for (int attrIdx = 0; attrIdx < node.nAttribute(); attrIdx++)
	{
		XMLAttribute attr = node.getAttribute(attrIdx);
		std::string attrName = MakeLower(attr.lpszName);
		if (attrName == "type")
		{
			std::string val = MakeLower(attr.lpszValue);
			if (val == "interactive")
			{
				m_desiredBehaviorType = (size_t)INTERACTIVE_FEATURE_BEHAVIOR;
				m_desiredBehaviorTypeNotSet = false;
			}
			else if (val == "update"
				|| val == "onupdate")
			{
				m_desiredBehaviorType = (size_t)ON_UPDATE_FEATURE_BEHAVIOR;
				m_desiredBehaviorTypeNotSet = false;
			}
			else if (val == "onplacement"
				|| val == "placement")
			{
				m_desiredBehaviorType = (size_t)ON_PLACEMENT_FEATURE_BEHAVIOR;
				m_desiredBehaviorTypeNotSet = false;
			}

		}
		else if (attrName == "referencename"
			|| attrName == "nickname"
			|| attrName == "nick")
		{
			m_chosenReferenceName = std::string(attr.lpszValue);
		}
	}
}

FeatureBehavior::FeatureBehavior(const FeatureBehavior& other)
	: m_entitiesCanInteractWith(other.m_entitiesCanInteractWith),
	m_featureName(other.m_featureName),
	m_desiredBehaviorType(other.m_desiredBehaviorType),
	m_desiredBehaviorTypeNotSet(other.m_desiredBehaviorTypeNotSet)
{
}

FeatureBehavior::~FeatureBehavior()
{
}

void FeatureBehavior::ResetEntitiesCanInteractWith()
{
	m_entitiesCanInteractWith.resize((int)NUM_OF_GAME_ENTITY_TYPES, true);
	m_entitiesCanInteractWith[FEATURE_ENTITY] = false;
}

//-------------------------------------------------------
//-------------------------------------------------------
//-------------------------------------------------------
//Setters
void FeatureBehavior::SetOwningFeature(Feature* feature)
{
	m_owningFeature = feature;
}

//-------------------------------------------------------
//-------------------------------------------------------
//-------------------------------------------------------
//Getters
const std::string& FeatureBehavior::GetFeatureName() const
{
	return m_featureName;
}

const std::string& FeatureBehavior::GetReferenceName() const
{
	return m_chosenReferenceName;
}

const Feature* FeatureBehavior::GetOwningFeature() const
{
	return m_owningFeature;
}

Feature* FeatureBehavior::GetEditableOwningFeature()
{
	return m_owningFeature;
}

size_t FeatureBehavior::GetDesiredBehaviorType() const
{
	return m_desiredBehaviorType;
}

bool FeatureBehavior::GetDesiredBehaviorTypeNotSet() const
{
	return m_desiredBehaviorTypeNotSet;
}

//-------------------------------------------------------
//-------------------------------------------------------
//-------------------------------------------------------
//Operations
int FeatureBehavior::WriteFeatureBehaviorToString(std::string& str, int indentationLevel) const
{
	GameDebuggingCommons::AddIndentation(str, indentationLevel);
	str += "Feature Behavior Name: " + m_featureName + "\n";
	indentationLevel += 3;

	GameDebuggingCommons::AddIndentation(str, indentationLevel);
	str += "Feature Behavior Nickname Name: " + m_chosenReferenceName + "\n";

	return indentationLevel;
}

void FeatureBehavior::CheckMapNodeForAttributesOfInterest(const XMLNode& node)
{
	node;
}

void FeatureBehavior::TriggerOnPlacement(const Map* map)
{
	map;
}

bool FeatureBehavior::EntityInteractWithFeature(BaseGameEntity* entity)
{
	if (entity == nullptr
		|| m_owningFeature == nullptr)
	{
		return false;
	}
	GameEntityType type = entity->GetEntityType();
	return m_entitiesCanInteractWith[type];
}

void FeatureBehavior::UpdateFeature(float dt)
{
	dt;
}