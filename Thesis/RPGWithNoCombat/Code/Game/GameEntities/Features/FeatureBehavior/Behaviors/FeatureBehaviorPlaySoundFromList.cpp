#include "Game/GameEntities/Features/FeatureBehavior/Behaviors/FeatureBehaviorPlaySoundFromList.hpp"
#include "Engine/Core/StringUtils.hpp"

//-------------------------------------------------------------
//-------------------------------------------------------------
//-------------------------------------------------------------
//Spawner Functions
STATIC FeatureBehavior* DefaultCreateFeaturePlaySoundFromListBehavior()
{
	return new FeatureBehaviorPlaySoundFromList();
}

STATIC FeatureBehavior* CreateFeaturePlaySoundFromListBehavior(const XMLNode& node)
{
	return new FeatureBehaviorPlaySoundFromList(node);
}

//-------------------------------------------------------------
//-------------------------------------------------------------
//-------------------------------------------------------------
//Static Variables
STATIC const FeatureBehaviorRegistrationHelper 
	FeatureBehaviorPlaySoundFromList::s_FeatureAddToAgentStatBehaviorRegistration
		= FeatureBehaviorRegistrationHelper("PlaySoundFromList",
			CreateFeaturePlaySoundFromListBehavior,
			DefaultCreateFeaturePlaySoundFromListBehavior);

//-------------------------------------------------------------
//-------------------------------------------------------------
//-------------------------------------------------------------
//Constructors
FeatureBehaviorPlaySoundFromList::FeatureBehaviorPlaySoundFromList()
	: FeatureBehavior(INTERACTIVE_FEATURE_BEHAVIOR)
{

}

FeatureBehaviorPlaySoundFromList::FeatureBehaviorPlaySoundFromList(const XMLNode& node)
	: FeatureBehavior(node, INTERACTIVE_FEATURE_BEHAVIOR)
{

	for (int attrIdx = 0; attrIdx < node.nAttribute(); attrIdx++)
	{
		XMLAttribute attr = node.getAttribute(attrIdx);
		std::string attrName = MakeLower(attr.lpszName);
		if (attrName == "action")
		{
			std::string val = MakeLower(attr.lpszValue);
			if (val == "random"
				|| val == "procedural")
			{
				m_action = PLAY_SOUND_RANDOM;
			}
			else if (val == "sequential"
				|| val == "sequence")
			{
				m_action = PLAY_SOUND_SEQUENTIAL;
			}
		}
	}

	for (int childIdx = 0; childIdx < node.nChildNode(); childIdx++)
	{
		XMLNode child = node.getChildNode(childIdx);
		std::string soundEffectName = "";
		float volume = 1.f;

		for (int childAttrIdx = 0; childAttrIdx < child.nAttribute(); childAttrIdx++)
		{
			XMLAttribute attr = child.getAttribute(childAttrIdx);
			std::string AttrName = MakeLower(attr.lpszName);
			if (AttrName == "soundeffect"
				|| AttrName == "sound"
				|| AttrName == "name")
			{
				soundEffectName = std::string(attr.lpszValue);
			}
			else if (AttrName == "volume"
				|| AttrName == "vol")
			{
				volume = (float)atof(attr.lpszValue);
			}
		}

		AddSoundEffectName(soundEffectName, volume);
	}
}

FeatureBehaviorPlaySoundFromList::FeatureBehaviorPlaySoundFromList(const FeatureBehaviorPlaySoundFromList& other)
	: FeatureBehavior(other),
	m_soundEffectNames(other.m_soundEffectNames),
	m_indexLastPlayed(other.m_indexLastPlayed),
	m_action(other.m_action)
{

}

FeatureBehaviorPlaySoundFromList::~FeatureBehaviorPlaySoundFromList()
{
	FeatureBehavior::~FeatureBehavior();
}

//-------------------------------------------------------------
//-------------------------------------------------------------
//-------------------------------------------------------------
//Setters
void FeatureBehaviorPlaySoundFromList::AddSoundEffectName(const std::string& name, float volume)
{
	if (name == "")
	{
		return;
	}

	m_soundEffectNames.push_back(SoundEffectData(name, volume));
}

//-------------------------------------------------------------
//-------------------------------------------------------------
//-------------------------------------------------------------
//Operations
bool FeatureBehaviorPlaySoundFromList::EntityInteractWithFeature(BaseGameEntity* entity)
{
	if (entity == nullptr)
	{
		return false;
	}

	return PlaySoundAction::PlaySoundFromList(m_soundEffectNames, m_action, m_indexLastPlayed);
}

FeatureBehavior* FeatureBehaviorPlaySoundFromList::Clone()
{
	return new FeatureBehaviorPlaySoundFromList(*this);
}