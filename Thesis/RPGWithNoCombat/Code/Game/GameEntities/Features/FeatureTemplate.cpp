#include "Game/GameEntities/Features/FeatureTemplate.hpp"
#include "Game/GameEntities/Features/Feature.hpp"
#include "Engine/Core/EngineXMLParser.hpp"
#include "Game/Common/GameDebuggingCommons.hpp"
#include "Engine/Math/MathToStringUtils.hpp"

//-------------------------------------------------------
//-------------------------------------------------------
//-------------------------------------------------------
//Constructors
FeatureTemplate::FeatureTemplate()
{
	m_FeatureBehaviors.resize(NUMBER_OF_DEFIND_FEATURE_BEHAVIOR_TYPES);

	m_sequences.resize(NUMBER_OF_DIRECTIONS4);
	m_sequenceNames.resize(NUMBER_OF_DIRECTIONS4);

	for (size_t dirIdx = 0; dirIdx < NUMBER_OF_DIRECTIONS4; dirIdx++)
	{
		m_sequences[dirIdx].resize(NUMBER_OF_ANIMATIONIDXES, nullptr);
		m_sequenceNames[dirIdx].resize(NUMBER_OF_ANIMATIONIDXES, "");
	}
}

FeatureTemplate::FeatureTemplate(const FeatureTemplate& other)
	: m_defaultImage(other.m_defaultImage),
	m_sequences(other.m_sequences),
	m_renderOffset(other.m_renderOffset),
	m_solid(other.m_solid),
	m_overwriteTileOnSolid(other.m_overwriteTileOnSolid),
	m_defaultImageName(other.m_defaultImageName),
	m_sequenceNames(other.m_sequenceNames)
{
	m_FeatureBehaviors.resize(other.m_FeatureBehaviors.size());
	for (size_t behaviorTypeIdx = 0; behaviorTypeIdx < other.m_FeatureBehaviors.size(); behaviorTypeIdx++)
	{
		const std::vector<FeatureBehavior*>& bevType = other.m_FeatureBehaviors[behaviorTypeIdx];
		m_FeatureBehaviors[behaviorTypeIdx].reserve(bevType.size());
		for (size_t behaviorIdx = 0; behaviorIdx < bevType.size(); behaviorIdx++)
		{
			FeatureBehavior* bev = bevType[behaviorIdx];
			if (bev != nullptr)
			{
				m_FeatureBehaviors[behaviorTypeIdx].push_back(bev->Clone());
			}
		}
	}
}

FeatureTemplate::~FeatureTemplate()
{
	for (size_t bevTypeIdx = 0; bevTypeIdx < m_FeatureBehaviors.size(); bevTypeIdx++)
	{
		for (size_t bevIdx = 0; bevIdx < m_FeatureBehaviors[bevTypeIdx].size(); bevIdx++)
		{
			FeatureBehavior* bev = m_FeatureBehaviors[bevTypeIdx][bevIdx];
			if (bev != nullptr)
			{
				delete bev;
			}
		}
	}
	m_FeatureBehaviors.clear();
}

//-------------------------------------------------------
//-------------------------------------------------------
//-------------------------------------------------------
//Operations
void FeatureTemplate::CopyDataOntoFeature(Feature* feature)
{
	if (feature == nullptr)
	{
		return;
	}
	feature->ChangeSpriteResource(m_defaultImage);

	for (Directions4 dir = NORTH; dir < NUMBER_OF_DIRECTIONS4; dir = (Directions4)(dir + 1))
	{
		for (int animIdx = 0; (size_t)animIdx < m_sequences[dir].size(); animIdx++)
		{
			feature->AddAnimationSequence(m_sequences[dir][animIdx], dir, animIdx);
		}
	}

	for (size_t bevType = 0; bevType < m_FeatureBehaviors.size(); bevType++)
	{
		for (size_t bevIdx = 0; bevIdx < m_FeatureBehaviors[bevType].size(); bevIdx++)
		{
			FeatureBehavior* bev = m_FeatureBehaviors[bevType][bevIdx];
			if (bev != nullptr)
			{
				feature->AddFeatureBehavior(bev->Clone(), bevType);
			}
		}
	}

	feature->SetSolid(m_solid, m_overwriteTileOnSolid);

}


void FeatureTemplate::CopyDataOntoFeature(Feature* feature, const XMLNode& node)
{
	if (feature == nullptr)
	{
		return;
	}
	feature->ChangeSpriteResource(m_defaultImage);

	for (Directions4 dir = NORTH; dir < NUMBER_OF_DIRECTIONS4; dir = (Directions4)(dir + 1))
	{
		for (int animIdx = 0; (size_t)animIdx < m_sequences[dir].size(); animIdx++)
		{
			feature->AddAnimationSequence(m_sequences[dir][animIdx], dir, animIdx);
		}
	}

	for (size_t bevType = 0; bevType < m_FeatureBehaviors.size(); bevType++)
	{
		for (size_t bevIdx = 0; bevIdx < m_FeatureBehaviors[bevType].size(); bevIdx++)
		{
			FeatureBehavior* bev = m_FeatureBehaviors[bevType][bevIdx];
			if (bev != nullptr)
			{
				FeatureBehavior* bevCopy = bev->Clone();
				bevCopy->CheckMapNodeForAttributesOfInterest(node);
				feature->AddFeatureBehavior(bevCopy, bevType);
			}
		}
	}

	feature->SetSolid(m_solid, m_overwriteTileOnSolid);
	feature->SetRenderOffset(m_renderOffset);
}

//-------------------------------------------------------
//-------------------------------------------------------
//-------------------------------------------------------
//Setters
void FeatureTemplate::SetRenderOffset(const Vector2& renderOffset)
{
	m_renderOffset = renderOffset;
}

void FeatureTemplate::SetDefaultImage(const SpriteResource* image, const std::string& imageName)
{
	m_defaultImage = image;
	m_defaultImageName = imageName;
}

void FeatureTemplate::AddSpriteAnimationSequence(SpriteAnimationSequence* sequence, Directions4 dir, int idx,
	const std::string& animName)
{
	if (sequence == nullptr
		|| dir < 0
		|| idx < 0
		|| (size_t)dir >= m_sequences.size())
	{
		return;
	}
	if ((size_t)idx >= m_sequences[dir].size())
	{
		m_sequences[dir].resize(idx + 1);
		m_sequenceNames[dir].resize(idx + 1);
	}
	m_sequences[dir][idx] = sequence;
	m_sequenceNames[dir][idx] = animName;
}

void FeatureTemplate::AddFeatureBehavior(size_t bevType, FeatureBehavior* bev)
{
	if (bevType >= m_FeatureBehaviors.size())
	{
		m_FeatureBehaviors.resize(bevType + 1);
	}
	m_FeatureBehaviors[bevType].push_back(bev);
}

void FeatureTemplate::SetIsSolid(bool solid, bool overwrite)
{
	m_solid = solid;
	m_overwriteTileOnSolid = overwrite;
}

//-------------------------------------------------------
//-------------------------------------------------------
//-------------------------------------------------------
//Getters
const Vector2& FeatureTemplate::GetRenderOffset() const
{
	return m_renderOffset;
}

bool FeatureTemplate::GetOverwriteIsTileOnSolid() const
{
	return m_overwriteTileOnSolid;
}

bool FeatureTemplate::GetIsSolid() const
{
	return m_solid;
}

size_t FeatureTemplate::GetNumberOfFeatureBehaviors() const
{
	return m_FeatureBehaviors.size();
}

const SpriteResource* FeatureTemplate::GetDefaultImage() const
{
	return m_defaultImage;
}

const std::vector<std::vector<SpriteAnimationSequence*>>& FeatureTemplate::GetAnimationSequences() const
{
	return m_sequences;
}

const std::vector<std::vector<SpriteAnimationSequence*>>* FeatureTemplate::GetAnimationSequencesPtr() const
{
	return &m_sequences;
}

std::vector<std::vector<SpriteAnimationSequence*>>* FeatureTemplate::GetEditableAnimationSequencesPtr() 
{
	return &m_sequences;
}

const std::vector<std::vector<FeatureBehavior*>>& FeatureTemplate::GetFeatureBehaviors() const
{
	return m_FeatureBehaviors;
}

const std::vector<std::vector<FeatureBehavior*>>* FeatureTemplate::GetFeatureBehaviorsPtr() const
{
	return &m_FeatureBehaviors;
}

std::vector<std::vector<FeatureBehavior*>>* FeatureTemplate::GetEditableFeatureBehaviorsPtr()
{
	return &m_FeatureBehaviors;
}


void FeatureTemplate::WriteFeatureTemplateToString(std::string& str, int indentationLvl) const
{
	/*

	std::string m_defaultImageName = "default";
	const SpriteResource* m_defaultImage = nullptr;
	std::vector<std::vector<SpriteAnimationSequence*>> m_sequences;
	std::vector<std::vector<FeatureBehavior*>> m_FeatureBehaviors;
	std::vector<std::vector<std::string>> m_sequenceNames;
	Vector2 m_renderOffset = Vector2::vec2_zeros;
	bool m_solid = false;
	bool m_overwriteTileOnSolid = false;
	
	*/

	GameDebuggingCommons::AddIndentation(str, indentationLvl);
	str += "Overwrites Tile as Solid: ";
	GameDebuggingCommons::WriteBoolOntoString(str, m_overwriteTileOnSolid);

	GameDebuggingCommons::AddIndentation(str, indentationLvl);
	str += "If overwrite, make Tile solid: "; 
	GameDebuggingCommons::WriteBoolOntoString(str, m_solid);

	GameDebuggingCommons::AddIndentation(str, indentationLvl);
	str += "Render Offset: ";
	std::string offset = MathToStringUtils::ToString(m_renderOffset);
	str += offset + "\n";


	GameDebuggingCommons::AddIndentation(str, indentationLvl);
	str += "Default Sprite Image: " + m_defaultImageName + "\n";

	GameDebuggingCommons::AddIndentation(str, indentationLvl);
	str += "Sprite Animations: \n";
	int nextLevel = indentationLvl + 3;
	int animLevel = nextLevel + 3;
	for (size_t dir = 0; dir < m_sequenceNames.size(); dir++)
	{
		GameDebuggingCommons::AddIndentation(str, nextLevel);
		switch (dir)
		{
		case NORTH:
			str += "NORTH\n";
			break;
		case EAST:
			str += "EAST\n";
			break;
		case SOUTH:
			str += "SOUTH\n";
			break;
		case WEST:
			str += "WEST\n";
			break;
		default:
			break;
		}

		for (size_t animIdx = 0; animIdx < m_sequenceNames[dir].size(); animIdx++)
		{
			GameDebuggingCommons::AddIndentation(str, animLevel);
			if (m_sequences[dir][animIdx] == nullptr)
			{
				str += "NULL\n";
			}
			else
			{
				str += m_sequenceNames[dir][animIdx] + "\n";
			}
		}
	}

	GameDebuggingCommons::AddIndentation(str, indentationLvl);
	str += "Feature Behaviors: \n";
	for (size_t featureBevType = 0; featureBevType < m_FeatureBehaviors.size(); featureBevType++)
	{
		GameDebuggingCommons::AddIndentation(str, nextLevel);
		switch (featureBevType)
		{
		case INTERACTIVE_FEATURE_BEHAVIOR:
			str += "INTERACTIVE\n";
			break;
		case ON_UPDATE_FEATURE_BEHAVIOR:
			str += "ON UPDATE\n";
			break;
		case ON_PLACEMENT_FEATURE_BEHAVIOR:
			str += "ON PLACEMENT\n";
			break;
		default:
			str += "UNKNOWN\n";
			break;
		}

		for (size_t featureIdx = 0; featureIdx < m_FeatureBehaviors[featureBevType].size(); featureIdx++)
		{
			FeatureBehavior* bev = m_FeatureBehaviors[featureBevType][featureIdx];
			if (bev == nullptr)
			{
				GameDebuggingCommons::AddIndentation(str, animLevel);
				str += "NULL\n";
			}
			else
			{
				bev->WriteFeatureBehaviorToString(str, animLevel);
			}
		}
	}
}