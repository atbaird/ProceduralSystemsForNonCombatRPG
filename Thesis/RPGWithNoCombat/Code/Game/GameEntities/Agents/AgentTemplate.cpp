#include "Game/GameEntities/Agents/AgentTemplate.hpp"
#include "Game/GameEntities/Agents/Player/Player.hpp"
#include "Game/GameEntities/Agents/NPCs/NPCBehavior.hpp"
#include "Game/GameEntities/Agents/NPCs/NPC.hpp"
#include "Game/GameEntities/Agents/Names/NameGenerator.hpp"
#include "Engine/Math/ExtendedMath.hpp"
#include "Engine/Core/EngineXMLParser.hpp"
#include "Game/Map/MapAgentSpecifications.hpp"
#include "Game/GameEntities/Agents/NPCs/NPCJobs/NPCJob.hpp"
#include "Game/GameEntities/Hair/HairGenerator.hpp"
#include "Engine/Math/ExtendedMath.hpp"
#include "Game/GameEntities/Hair/Hair.hpp"
#include "Game/GameEntities/Agents/NPCs/NPCAttributes/NPCAttributeContainer.hpp"
#include "Game/GameEntities/Agents/NPCs/NPCAttribute.hpp"
#include "Game/Common/GameDebuggingCommons.hpp"
#include "Game/GameEntities/Items/ItemGenerator.hpp"

//----------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------
//Static Variables
STATIC std::vector<std::string>* AgentTemplate::s_DefaultHairs = nullptr;

//----------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------
//Constructors
AgentTemplate::AgentTemplate()
	: m_defaultImage(nullptr),
	m_AgentName()
{
	ClearAgentName();

	m_sequences.resize(NUMBER_OF_DIRECTIONS4);
	m_SequenceNames.resize(NUMBER_OF_DIRECTIONS4);

	for (size_t dirIdx = 0; dirIdx < NUMBER_OF_DIRECTIONS4; dirIdx++)
	{
		m_sequences[dirIdx].resize(NUMBER_OF_ANIMATIONIDXES, nullptr);
		m_SequenceNames[dirIdx].resize(NUMBER_OF_ANIMATIONIDXES, "");
	}
}

AgentTemplate::AgentTemplate(const AgentTemplate& other)
	: m_templateNPCAttributes(other.m_templateNPCAttributes),
	m_defaultDialogueSegmentChoices(other.m_defaultDialogueSegmentChoices),
	m_stats(other.m_stats),
	m_AttributeNamesToLoad(other.m_AttributeNamesToLoad),
	m_maleHair(other.m_maleHair),
	m_femaleHair(other.m_femaleHair),
	m_NameGeneratorNamesToUse(other.m_NameGeneratorNamesToUse),
	m_JobTitlesToUse(other.m_JobTitlesToUse),
	m_HairColorChoiceLimits(other.m_HairColorChoiceLimits),
	m_ClothingNames(other.m_ClothingNames),
	m_sequences(other.m_sequences),
	m_SequenceNames(other.m_SequenceNames),
	m_DefaultSpriteName(other.m_DefaultSpriteName),
	m_skinColors(other.m_skinColors),
	m_AgentNameSetToUse(other.m_AgentNameSetToUse),
	m_percentageMale(other.m_percentageMale),
	m_movementSpeed(other.m_movementSpeed),
	m_sprintingMovementSpeed(other.m_sprintingMovementSpeed),
	m_npcMinimumMovementSpeed(other.m_npcMinimumMovementSpeed),
	m_npcMaximumMovementSpeed(other.m_npcMaximumMovementSpeed),
	m_defaultImage(other.m_defaultImage),
	m_AgentName(""),
	m_percentageMaleSetOnce(other.m_percentageMaleSetOnce)
{
	const char* otherName = other.GetAgentName();
	SetAgentTemplateName(otherName);
	m_NPCBehaviorsIfNPC.reserve(other.m_NPCBehaviorsIfNPC.size());
	for (size_t behaviorIdx = 0; behaviorIdx < other.m_NPCBehaviorsIfNPC.size(); behaviorIdx++)
	{
		NPCBehavior* bev = other.m_NPCBehaviorsIfNPC[behaviorIdx];
		if (bev != nullptr)
		{
			m_NPCBehaviorsIfNPC.push_back(bev->Clone());
		}
	}
}

AgentTemplate::~AgentTemplate()
{
	for (size_t bevIdx = 0; bevIdx < m_NPCBehaviorsIfNPC.size(); bevIdx++)
	{
		NPCBehavior* bev = m_NPCBehaviorsIfNPC[bevIdx];
		if (bev != nullptr)
		{
			delete bev;
		}
	}
	m_NPCBehaviorsIfNPC.clear();
}

//----------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------
//Setters
void AgentTemplate::SetDefaultImage(const SpriteResource* res, const std::string& spriteName)
{
	m_defaultImage = res;
	m_DefaultSpriteName = spriteName;
}

void AgentTemplate::SetPercentageMale(float percentage)
{
	m_percentageMale = percentage;
	m_percentageMaleSetOnce = true;
}

void AgentTemplate::AddTemplateNPCAttributesContainer(AgentTemplateNPCAttributesContainer& templa)
{
	for (size_t idx = 0; idx < m_templateNPCAttributes.size(); idx++)
	{
		if (m_templateNPCAttributes[idx].GetNPCAttributeToCall() == templa.GetNPCAttributeToCall())
		{
			m_templateNPCAttributes[idx].AddAllToBlackList(templa.GetBlackListCopy());
			m_templateNPCAttributes[idx].AddAllToWhiteList(templa.GetWhiteListCopy());
			if (templa.GetUseBlackList())
			{
				m_templateNPCAttributes[idx].SetUseBlackList(true);
			}
			if (templa.GetUseWhiteList())
			{
				m_templateNPCAttributes[idx].SetUseWhiteList(true);
			}
			return;
		}
	}

	m_templateNPCAttributes.push_back(templa);
}

void AgentTemplate::SetAgentNameSetToUse(const std::string& nameSet)
{
	m_AgentNameSetToUse = nameSet;
}

void AgentTemplate::AddNPCAttributeName(const std::string& attributeName)
{
	m_AttributeNamesToLoad.push_back(attributeName);
}

void AgentTemplate::AddSkinColor(int skinColor)
{
	m_skinColors.push_back(skinColor);
}

void AgentTemplate::AddHairColorChoiceLimit(const std::string& limit)
{
	m_HairColorChoiceLimits.push_back(limit);
}

void AgentTemplate::AddNameGeneratorName(const std::string& name)
{
	m_NameGeneratorNamesToUse.push_back(name);
}

void AgentTemplate::AddMaleHairName(const std::string& name)
{
	if (name == "")
	{
		return;
	}
	m_maleHair.push_back(name);
}

void AgentTemplate::AddFemaleHairName(const std::string& name)
{
	if (name == "")
	{
		return;
	}
	m_femaleHair.push_back(name);
}

void AgentTemplate::AddClothingName(const std::string& name)
{
	if (name == "")
	{
		return;
	}
	m_ClothingNames.push_back(name);
}

void AgentTemplate::SetAgentTemplateName(const char* name)
{
	m_AgentName = std::string(name);
}


void AgentTemplate::SetMovementSpeed(float movementSpeed)
{
	m_movementSpeed = movementSpeed;
}

void AgentTemplate::SetSprintingMovementSpeed(float movementSpeed)
{
	m_sprintingMovementSpeed = movementSpeed;
}

void AgentTemplate::SetNPCMinimumMovementSpeed(float movementSpeed)
{
	m_npcMinimumMovementSpeed = movementSpeed;
}

void AgentTemplate::SetNPCMaximumMovementSpeed(float movementSpeed)
{
	m_npcMaximumMovementSpeed = movementSpeed;
}

void AgentTemplate::AddSpriteAnimationSequence(SpriteAnimationSequence* sequence, Directions4 dir, int idx,
	const std::string& str)
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
		m_SequenceNames[dir].resize(idx + 1);
	}
	m_sequences[dir][idx] = sequence;
	m_SequenceNames[dir][idx] = str;
}

void AgentTemplate::RegisterNPCBehavior(NPCBehavior* bev)
{
	if (bev == nullptr)
	{
		return;
	}
	m_NPCBehaviorsIfNPC.push_back(bev);
}

void AgentTemplate::ReadXMLNodeChildrenForStatRangeSettings(const XMLNode& node)
{
	for (int childIdx = 0; childIdx < node.nChildNode(); childIdx++)
	{
		XMLNode child = node.getChildNode(childIdx);
		std::string childName = child.getName();
		std::string childNameLowerCase = MakeLower(childName);
		std::string childAbbreviation = childName;
		int min = 0;
		int max = 100;
		for (int childAttrIdx = 0; childAttrIdx < child.nAttribute(); childAttrIdx++)
		{
			XMLAttribute attr = child.getAttribute(childAttrIdx);
			std::string attrName = MakeLower(attr.lpszName);
			std::string attrVal = std::string(attr.lpszValue);
			if (attrName == "min"
				|| attrName == "minimum")
			{
				min = atoi(attrVal.c_str());
			}
			else if (attrName == "max"
				|| attrName == "maximum")
			{
				max = atoi(attrVal.c_str());
			}
			else if (attrName == "range")
			{
				std::string val = ReplaceCharInString(attrVal, ',', ' ');
				std::vector<std::string> parsed = ParseString(val, ' ');
				if (parsed.size() > 0)
				{
					min = atoi(parsed[0].c_str());
				}
				if (parsed.size() > 1)
				{
					max = atoi(parsed[1].c_str());
				}
			}
		}

		AgentStat stat;
		stat.SetStatName(childName);
		stat.SetStatAbreviation(childAbbreviation);
		stat.SetStatAbsoluteMin(min);
		stat.SetStatAbsoluteMax(max);

		bool found = false;
		for (size_t statIdx = 0; statIdx < m_stats.size(); statIdx++)
		{
			if (m_stats[statIdx].GetLowerCaseStatName() == stat.GetLowerCaseStatName()
				|| m_stats[statIdx].GetLowerCaseStatAbreviation() == stat.GetLowerCaseStatAbreviation())
			{
				found = true;
				m_stats[statIdx].SetStatAbsoluteMin(min);
				m_stats[statIdx].SetStatAbsoluteMax(max);
				break;
			}
		}
		if (found == false)
		{
			m_stats.push_back(stat);
		}
	}
}

void AgentTemplate::AddJobTitle(const std::string& jobTitle)
{
	m_JobTitlesToUse.push_back(jobTitle);
}

void AgentTemplate::AddDialogueSegmentChoice(const AgentGeneratorDialogueSegmentChoice& dialogueChoice)
{
	m_defaultDialogueSegmentChoices.push_back(dialogueChoice);
}

void AgentTemplate::ClearAgentName()
{
	m_AgentName = "";
}

//----------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------
//Getters
void AgentTemplate::WriteOutStringsToString(const std::vector<std::string>& strings, std::string& str,
	int indentation) const
{
	for (size_t i = 0; i < strings.size(); i++)
	{
		const std::string& string = strings[i];
		GameDebuggingCommons::AddIndentation(str, indentation);
		str += string + "\n";

	}
}

void AgentTemplate::WriteAgentGeneratorToString(std::string& str, int indentation) const
{
	int nextTierIndentation = indentation + 3;

	//Main Name Generator
	GameDebuggingCommons::AddIndentation(str, indentation);
	str += "Main Name Generator: " + m_AgentNameSetToUse + "\n";

	//Male or Female
	GameDebuggingCommons::AddIndentation(str, indentation);
	str += "Percentage chance for Agent to be Male: " + std::to_string(m_percentageMale) + "\n";
	
	//Movmenet Speed
	GameDebuggingCommons::AddIndentation(str, indentation);
	str += "Normal movement speed: " + std::to_string(m_movementSpeed) + "\n";

	GameDebuggingCommons::AddIndentation(str, indentation);
	str += "Sprinting movement speed: " + std::to_string(m_sprintingMovementSpeed) + "\n";

	//Default sprite image
	GameDebuggingCommons::AddIndentation(str, indentation);
	str += "Default Sprite: ";
	if (m_defaultImage == nullptr)
	{
		str += "NULL\n";
	}
	else
	{
		str += m_DefaultSpriteName + "\n";
	}


	//Attributes
	GameDebuggingCommons::AddIndentation(str, indentation);
	str += "NPC Attribute Names\n";
	WriteOutStringsToString(m_AttributeNamesToLoad, str, nextTierIndentation);

	//Male Hairs
	GameDebuggingCommons::AddIndentation(str, indentation);
	str += "Male Hairs\n";
	WriteOutStringsToString(m_maleHair, str, nextTierIndentation);

	//Female Hairs
	GameDebuggingCommons::AddIndentation(str, indentation);
	str += "Female Hairs\n";
	WriteOutStringsToString(m_femaleHair, str, nextTierIndentation);

	//Hair Color Limits
	GameDebuggingCommons::AddIndentation(str, indentation);
	str += "Hair Color limits\n";
	WriteOutStringsToString(m_HairColorChoiceLimits, str, nextTierIndentation);

	//Clothing to use
	GameDebuggingCommons::AddIndentation(str, indentation);
	str += "Clothing\n";
	WriteOutStringsToString(m_ClothingNames, str, nextTierIndentation);
	
	//NameGenerators
	GameDebuggingCommons::AddIndentation(str, indentation);
	str += "Additional Name Generators\n";
	WriteOutStringsToString(m_NameGeneratorNamesToUse, str, nextTierIndentation);

	//NPC Jobs
	GameDebuggingCommons::AddIndentation(str, indentation);
	str += "NPC Jobs\n"; 
	WriteOutStringsToString(m_JobTitlesToUse, str, nextTierIndentation);

	//NPC Attribute Containers
	GameDebuggingCommons::AddIndentation(str, indentation);
	str += "NPC Attribute Containers\n";
	for (size_t i = 0; i < m_templateNPCAttributes.size(); i++)
	{
		m_templateNPCAttributes[i].WriteNPCAttributeContainerOutToString(str, nextTierIndentation);
	}

	//Dialogue Segment Choices
	GameDebuggingCommons::AddIndentation(str, indentation);
	str += "Dialogue Segment Choice\n";
	for (size_t i = 0; i < m_defaultDialogueSegmentChoices.size(); i++)
	{
		m_defaultDialogueSegmentChoices[i].WriteDialogueSegmentChoiceToString(str, nextTierIndentation);
	}

	//Agent Stats
	GameDebuggingCommons::AddIndentation(str, indentation);
	str += "Agent Stats\n";
	for (size_t i = 0; i < m_stats.size(); i++)
	{
		m_stats[i].WriteAgentStatToString(str, nextTierIndentation);
	}

	//NPC Behaviors
	GameDebuggingCommons::AddIndentation(str, indentation);
	str += "NPC Behaviors\n";
	for (size_t i = 0; i <  m_NPCBehaviorsIfNPC.size(); i++)
	{
		NPCBehavior* bev = m_NPCBehaviorsIfNPC[i];
		if (bev != nullptr)
		{
			bev->WriteNPCBehaviorToString(str, nextTierIndentation);
		}
	}
	
	//Animations
	GameDebuggingCommons::AddIndentation(str, indentation);
	str += "Animations per Direction\n";
	int animationTierIndentation = nextTierIndentation + 3;
	for (int dir = 0; (size_t)dir < m_SequenceNames.size(); dir++)
	{
		switch (dir)
		{
		case NORTH:
			GameDebuggingCommons::AddIndentation(str, nextTierIndentation);
			str += "NORTH\n";
			break;
		case EAST:
			GameDebuggingCommons::AddIndentation(str, nextTierIndentation);
			str += "EAST\n";
			break;
		case WEST:
			GameDebuggingCommons::AddIndentation(str, nextTierIndentation);
			str += "WEST\n";
			break;
		case SOUTH:
			GameDebuggingCommons::AddIndentation(str, nextTierIndentation);
			str += "SOUTH\n";
			break;
		default:
			break;
		}
		for (int animIdx = 0; (size_t)animIdx < m_SequenceNames[dir].size(); animIdx++)
		{
			GameDebuggingCommons::AddIndentation(str, animationTierIndentation);
			str += "Animation: " + m_SequenceNames[dir][animIdx] + "\n";
		}
	}

}

//----------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------
//Copy Methods
void AgentTemplate::CopyVariablesOffOtherTemplate(const AgentTemplate& other)
{
	//This method will copy everything EXCEPT Dialogue Segments, Animation Sequences, and movement speed.

	if (m_defaultImage == nullptr)
	{
		m_defaultImage = other.m_defaultImage;
		m_DefaultSpriteName = other.m_DefaultSpriteName;
	}
	if (m_AgentNameSetToUse == "")
	{
		m_AgentNameSetToUse = other.m_AgentNameSetToUse;
	}
	if (!m_percentageMaleSetOnce
		&& other.m_percentageMaleSetOnce)
	{
		m_percentageMale = other.m_percentageMale;
		m_percentageMaleSetOnce = true;
	}

	if (m_defaultDialogueSegmentChoices.size() == 0)
	{
		m_defaultDialogueSegmentChoices.insert(m_defaultDialogueSegmentChoices.begin(),
			other.m_defaultDialogueSegmentChoices.begin(), other.m_defaultDialogueSegmentChoices.end());
	}

	CopyStringsOffOtherTemplateStrings(m_maleHair, other.m_maleHair);
	CopyStringsOffOtherTemplateStrings(m_femaleHair, other.m_femaleHair);
	CopyStringsOffOtherTemplateStrings(m_NameGeneratorNamesToUse, other.m_NameGeneratorNamesToUse);
	CopyStringsOffOtherTemplateStrings(m_JobTitlesToUse, other.m_JobTitlesToUse);
	CopyStringsOffOtherTemplateStrings(m_HairColorChoiceLimits, other.m_HairColorChoiceLimits);
	CopyIntsOffOtherTemplateInts(m_skinColors, other.m_skinColors);
	CopyNPCBehaviorsOffOtherTemplateBehaviors(m_NPCBehaviorsIfNPC, other.m_NPCBehaviorsIfNPC);

	//Better not to let this one actually copy anything.
	CopyAnimationsOffOtherAnimations(m_sequences, other.m_sequences,
		m_SequenceNames, other.m_SequenceNames);

	for (size_t otherNpcAttributeIdx = 0; 
		otherNpcAttributeIdx < other.m_templateNPCAttributes.size(); 
		otherNpcAttributeIdx++)
	{
		bool found = false;
		for (size_t NpcAttributeIdx = 0;
			NpcAttributeIdx < m_templateNPCAttributes.size();
			NpcAttributeIdx++)
		{
			if (m_templateNPCAttributes[NpcAttributeIdx].GetNPCAttributeToCall()
				== other.m_templateNPCAttributes[otherNpcAttributeIdx].GetNPCAttributeToCall())
			{
				m_templateNPCAttributes[NpcAttributeIdx].AddAllToBlackList(
					other.m_templateNPCAttributes[otherNpcAttributeIdx].GetBlackListCopy());
				m_templateNPCAttributes[NpcAttributeIdx].AddAllToWhiteList(
					other.m_templateNPCAttributes[otherNpcAttributeIdx].GetWhiteListCopy());
				found = true;
				break;
			}
		}
		if (found)
		{
			continue;
		}
		m_templateNPCAttributes.push_back(other.m_templateNPCAttributes[otherNpcAttributeIdx]);
	}
}


STATIC void AgentTemplate::CopyStringsOffOtherTemplateStrings(std::vector<std::string>& myHairNames,
	const std::vector<std::string>& otherHairNames)
{
	if (myHairNames.size() != 0)
	{
		return;
	}
	for (size_t otherHairNameIdx = 0; otherHairNameIdx < otherHairNames.size(); otherHairNameIdx++)
	{
		bool alreadyExists = false;
		std::string name = otherHairNames[otherHairNameIdx];
		std::string lowerCaseName = MakeLower(name);
		for (size_t myHairNameIdx = 0; myHairNameIdx < myHairNames.size(); myHairNameIdx++)
		{
			if (MakeLower(myHairNames[myHairNameIdx]) == lowerCaseName)
			{
				alreadyExists = true;
				break;
			}
		}

		if (!alreadyExists)
		{
			myHairNames.push_back(name);
		}
	}
}

STATIC void AgentTemplate::CopyIntsOffOtherTemplateInts(std::vector<int>& myInts, const std::vector<int>& otherInts)
{
	if (myInts.size() != 0)
	{
		return;
	}
	for (size_t intIdx = 0; intIdx < otherInts.size(); intIdx++)
	{
		myInts.push_back(otherInts[intIdx]);
	}
}

STATIC void AgentTemplate::CopyNPCBehaviorsOffOtherTemplateBehaviors(std::vector<NPCBehavior*>& NPCBehaviorsIfNPC,
	const std::vector<NPCBehavior*>& other_NPCBehaviorsIfNPC)
{
	if (NPCBehaviorsIfNPC.size() != 0)
	{
		return;
	}
	for (size_t otherBehaviorIdx = 0; otherBehaviorIdx < other_NPCBehaviorsIfNPC.size(); otherBehaviorIdx++)
	{
		NPCBehavior* other_behavior = other_NPCBehaviorsIfNPC[otherBehaviorIdx];
		if (other_behavior == nullptr)
		{
			continue;
		}

		bool behaviorRedefinedAlready = false;
		std::string other_behaviorNameLowerCase = MakeLower(other_behavior->GetName());
		for (size_t myBehaviorIdx = 0; myBehaviorIdx < NPCBehaviorsIfNPC.size(); myBehaviorIdx++)
		{
			NPCBehavior* my_behavior = NPCBehaviorsIfNPC[myBehaviorIdx];
			if (my_behavior == nullptr)
			{
				continue;
			}
			std::string my_behaviorNameLowerCase = MakeLower(my_behavior->GetName());
			if (my_behaviorNameLowerCase == other_behaviorNameLowerCase)
			{
				behaviorRedefinedAlready = true;
				break;
			}
		}
		if (!behaviorRedefinedAlready)
		{
			NPCBehavior* bev = other_behavior->Clone();
			NPCBehaviorsIfNPC.push_back(bev);
		}
	}
}


STATIC void AgentTemplate::CopyAgentStatsOffOtherAgentStats(std::vector<AgentStat>& agentStats,
	const std::vector<AgentStat>& other_agentStats)
{
	for (size_t otherIdx = 0; otherIdx < other_agentStats.size(); otherIdx++)
	{
		bool alreadyDefined = false;
		std::string statName = other_agentStats[otherIdx].GetLowerCaseStatName();
		std::string statAbbreviation = other_agentStats[otherIdx].GetLowerCaseStatAbreviation();
		for (size_t myIdx = 0; myIdx < agentStats.size(); myIdx++)
		{
			std::string myStatName = agentStats[myIdx].GetLowerCaseStatName();
			std::string myStatAbbreviation = agentStats[myIdx].GetLowerCaseStatAbreviation();
			bool myStatNameIsSame = myStatName == statName;
			bool myStatAbbreviationIsSame = myStatAbbreviation == statAbbreviation;
			if (myStatNameIsSame || myStatAbbreviationIsSame)
			{
				alreadyDefined = true;
				break;
			}
		}
		if (!alreadyDefined)
		{
			agentStats.push_back(other_agentStats[otherIdx]);
		}
	}
}

void AgentTemplate::CopyAnimationsOffOtherAnimations(std::vector<std::vector<SpriteAnimationSequence*>>& myAnimations,
	const std::vector<std::vector<SpriteAnimationSequence*>>& otherAnimations,
	std::vector<std::vector<std::string>>& myAnimationsNames,
	const std::vector<std::vector<std::string>>& otherAnimationsNames)
{

	for (size_t directionIdx = 0; directionIdx < otherAnimations.size(); directionIdx++)
	{
		for (size_t animIdx = 0; animIdx < otherAnimations[directionIdx].size(); animIdx++)
		{
			if (myAnimations[directionIdx][animIdx] != nullptr)
			{
				continue;
			}
			else if (otherAnimations[directionIdx].size() >= myAnimations[directionIdx].size())
			{
				myAnimations[directionIdx].resize(otherAnimations[directionIdx].size());
				myAnimationsNames[directionIdx].resize(otherAnimationsNames[directionIdx].size());
			}
			myAnimations[directionIdx][animIdx] = otherAnimations[directionIdx][animIdx];
			myAnimationsNames[directionIdx][animIdx] = otherAnimationsNames[directionIdx][animIdx];
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------
//Copy onto Agent Methods
void AgentTemplate::CopyVariablesToAgentThatDontRequireSpecifications(BaseGameEntity* entity) const
{
	for (Directions4 dir = NORTH; dir < NUMBER_OF_DIRECTIONS4; dir = (Directions4)(dir + 1))
	{
		for (int animIdx = 0; (size_t)animIdx < m_sequences[dir].size(); animIdx++)
		{
			entity->AddAnimationSequence(m_sequences[dir][animIdx], dir, animIdx);
		}
	}
	Agent* agent = dynamic_cast<Agent*>(entity);
	if (agent == nullptr)
	{
		return;
	}
	if (m_ClothingNames.size() != 0)
	{
		int randomClothing = GetRandomIntLessThan(m_ClothingNames.size());
		std::string clothingName = m_ClothingNames[randomClothing];
		BaseGameEntity* clothing = ItemGenerator::SpawnItemByItemName(clothingName.c_str(), Vector2::vec2_zeros);
		Item* itemClothing = dynamic_cast<Item*>(clothing);
		agent->SetEquippedClothing(itemClothing);
	}

	agent->SetCurrentMovementSpeed(m_movementSpeed);
	agent->SetTrueMovementSpeed(m_movementSpeed);
	agent->SetSprintingMovementSpeed(m_sprintingMovementSpeed);

	bool isMale = false;
	float chance = GetRandomFloatZeroToOne();
	if (chance <= m_percentageMale)
	{
		isMale = true;
	}
	else
	{
		isMale = false;
	}

	agent->SetIsMale(isMale);

	if (m_skinColors.size() == 0)
	{
		agent->SetColorSchemeChoice(0);
	}
	else
	{
		int randomSkinColor = GetRandomIntLessThan(m_skinColors.size());
		agent->SetColorSchemeChoice(m_skinColors[randomSkinColor]);
	}

	bool hairsNotDefinedForGender = isMale ? m_maleHair.size() == 0 : m_femaleHair.size() == 0;
		//(isMale && m_maleHair.size() == 0)
		//|| (isMale == false && m_femaleHair.size() == 0);

	if (hairsNotDefinedForGender)
	{
		if (s_DefaultHairs != nullptr)
		{
			int ranHairSelection = GetRandomIntLessThan(s_DefaultHairs->size());
			BaseGameEntity* hair = nullptr;

			if (m_HairColorChoiceLimits.size() == 0)
			{
				hair = HairGenerator::SpawnHairByHairName(s_DefaultHairs->at(ranHairSelection).c_str());
			}
			else
			{
				hair = HairGenerator::SpawnHairByHairName(s_DefaultHairs->at(ranHairSelection).c_str(), 
					m_HairColorChoiceLimits);
			}

			Hair* hairAsHair = dynamic_cast<Hair*>(hair);
			agent->SetHair(hairAsHair);
		}
	}
	else
	{
		BaseGameEntity* hair = nullptr;
		if (isMale)
		{
			int ranHairSelection = GetRandomIntLessThan(m_maleHair.size());
			if (m_HairColorChoiceLimits.size() == 0)
			{
				hair = HairGenerator::SpawnHairByHairName(m_maleHair[ranHairSelection].c_str());
			}
			else
			{
				hair = HairGenerator::SpawnHairByHairName(m_maleHair[ranHairSelection].c_str(), m_HairColorChoiceLimits);
			}
		}
		else
		{
			int ranHairSelection = GetRandomIntLessThan(m_femaleHair.size());
			if (m_HairColorChoiceLimits.size() == 0)
			{
				hair = HairGenerator::SpawnHairByHairName(m_femaleHair[ranHairSelection].c_str());
			}
			else
			{
				hair = HairGenerator::SpawnHairByHairName(m_femaleHair[ranHairSelection].c_str(), m_HairColorChoiceLimits);
			}
		}
		Hair* hairAsHair = dynamic_cast<Hair*>(hair);
		agent->SetHair(hairAsHair);
	}

	if (m_AgentNameSetToUse != "")
	{
		std::string name =
			NameGenerator::GlobalGenerateARandomNameForGeneratorNameByGender(m_AgentNameSetToUse, isMale);
		agent->SetAgentName(name);
	}

}

void AgentTemplate::CopyVariablesToAgentStats(Agent* agent) const
{
	for (size_t i = 0; i < m_stats.size(); i++)
	{
		AgentStat stat = m_stats[i];
		agent->SetOrAddStatByStat(stat);
	}
	agent->RandomizeAllStats();
	agent->RestoreMaxAndMinStatValuesToGlobalDefaults();
}

void AgentTemplate::CopyVariablesToAgentStats(Agent* agent, 
	const MapAgentSpecifications& specifications) const
{
	std::vector<AgentStat> stats;
	for (size_t statIdx = 0; statIdx < specifications.m_overwritingStats.size(); statIdx++)
	{
		bool overwriteStat = specifications.m_overwriteStats[statIdx];
		const AgentStat& overwritingStat = specifications.m_overwritingStats[statIdx];
		const AgentStat* stat = nullptr;
		for (size_t OtherStatIdx = 0; OtherStatIdx < m_stats.size(); OtherStatIdx++)
		{
			if (m_stats[OtherStatIdx].GetLowerCaseStatAbreviation() == overwritingStat.GetLowerCaseStatAbreviation()
				|| m_stats[OtherStatIdx].GetLowerCaseStatName() == overwritingStat.GetLowerCaseStatName())
			{
				stat = &m_stats[OtherStatIdx];
				break;
			}
		}

		if (stat == nullptr)
		{
			AgentStat copy = overwritingStat;
			copy.RandomizeValue();
			stats.push_back(copy);
		}
		else
		{
			AgentStat copy = *stat;
			if (overwriteStat)
			{
				copy.SetStatAbsoluteMax(overwritingStat.GetValueMax());
				copy.SetStatAbsoluteMin(overwritingStat.GetValueMin());
			}
			stats.push_back(copy);
		}
	}

	CheckAllListedJobsAgainstStats(specifications, stats);

	for (size_t i = 0; i < stats.size(); i++)
	{
		AgentStat stat = stats[i];
		stat.RandomizeValue();
		agent->SetOrAddStatByStat(stat);
	}
}

void AgentTemplate::CopyVariablesToAgent(BaseGameEntity* entity) const
{
	CopyVariablesToAgentThatDontRequireSpecifications(entity);

	Agent* agent = dynamic_cast<Agent*>(entity);
	if (agent == nullptr)
	{
		return;
	}
	CopyVariablesToAgentStats(agent);
}


void AgentTemplate::CopyVariablesToAgent(BaseGameEntity* entity, const MapAgentSpecifications& specifications) const
{
	CopyVariablesToAgentThatDontRequireSpecifications(entity);

	Agent* agent = dynamic_cast<Agent*>(entity);
	if (agent == nullptr)
	{
		return;
	}
	CopyVariablesToAgentStats(agent, specifications);
	
}

void AgentTemplate::OverwriteStatIfNotNegOne(int& statToSet, int previouslyDefinedStat, int overwrittingStat) const
{
	if (overwrittingStat != -1)
	{
		statToSet = overwrittingStat;
	}
	else
	{
		statToSet = previouslyDefinedStat;
	}
}


void AgentTemplate::CheckAllListedJobsAgainstStats(const MapAgentSpecifications& specifications, 
	std::vector<AgentStat>& stats) const
{
	//The point of this is to guarantee that the procedurally generated
	//stats meet the bare minimum for at least one of the jobs,
	//Thus guaranteeing the NPC doesn't end up jobless.
	//This does mean that it will sacrifice inputs from the user if they do not keep in mind the
	//minimum stat requirements for the required jobs.
	std::vector<int> lowestVals;
	std::vector<int> highestVals;
	std::vector<NPCJob*> lowestJobs;
	std::vector<NPCJob*> highestJobs;

	std::vector<std::string> jobsDesired = specifications.m_jobsDesired;
	if (jobsDesired.size() == 0)
	{
		return;
	}

	for (size_t currentStatIdx = 0; currentStatIdx < stats.size(); currentStatIdx++)
	{
		const AgentStat& stat = stats[currentStatIdx];
		int lowestStat = -1;
		int highestStat = -1;
		NPCJob* jobWithLowest = nullptr;
		NPCJob* jobWithHighest = nullptr;
		for (size_t jobIdx = 0; jobIdx < jobsDesired.size(); jobIdx++)
		{
			std::string jobTitle = jobsDesired[jobIdx];
			NPCJob* job = NPCJob::GetJobByTitle(jobTitle);
			if (job == nullptr)
			{
				jobsDesired.erase(jobsDesired.begin() + jobIdx);
				jobIdx--;
				continue;
			}
			int actual = -1;
			int nameTest = job->GetStatRequirementByName(stat.GetStatName());
			int abbreviationTest = job->GetStatRequirementByAbbreviation(stat.GetStatAbreviation());
			if (nameTest != -1)
			{
				actual = nameTest;
			}
			else if (abbreviationTest != -1)
			{
				actual = abbreviationTest;
			}

			CheckRequiredJobStatAgainstLowestHighestStat(lowestStat, highestStat, actual, 
				jobWithLowest, jobWithHighest, job);
		}

		for (size_t jobIdx = 0; jobIdx < specifications.m_jobsDesired.size(); jobIdx++)
		{
			std::string jobTitle = specifications.m_jobsDesired[jobIdx];
			NPCJob* job = NPCJob::GetJobByTitle(jobTitle);
			if (job == nullptr)
			{
				jobsDesired.erase(specifications.m_jobsDesired.begin() + jobIdx);
				jobIdx--;
				continue;
			}
			int actual = -1;
			int nameTest = job->GetStatRequirementByName(stat.GetStatName());
			int abbreviationTest = job->GetStatRequirementByAbbreviation(stat.GetStatAbreviation());
			if (nameTest != -1)
			{
				actual = nameTest;
			}
			else if (abbreviationTest != -1)
			{
				actual = abbreviationTest;
			}

			CheckRequiredJobStatAgainstLowestHighestStat(lowestStat, highestStat, actual, 
				jobWithLowest, jobWithHighest, job);
		}
		SetMinMaxStatBasedOffLowestHighestJobStats(stats, jobWithLowest, jobWithHighest);
	}
}

void AgentTemplate::CheckRequiredJobStatAgainstLowestHighestStat(int& lowestStat, int& highestStat, int currentStat, 
	NPCJob*& lowestJob,	NPCJob*& highestJob, NPCJob* job) const
{
	if (job == nullptr)
	{
		return;
	}

	if (lowestStat == -1
		|| lowestStat > currentStat)
	{
		lowestStat = currentStat;
		lowestJob = job;
	}
	if (highestStat == -1
		|| highestStat < currentStat)
	{
		highestStat = currentStat;
		highestJob = job;
	}
}

void AgentTemplate::SetMinMaxStatBasedOffLowestHighestJobStats(std::vector<AgentStat>& stats,
	NPCJob* lowestJob, NPCJob* highestJob) const
{
	if (lowestJob == nullptr
		|| highestJob == nullptr)
	{
		return;
	}

	std::vector<AgentStat> lowestStats = lowestJob->GetStats();
	std::vector<AgentStat> highestStats = highestJob->GetStats();

	for (size_t j = 0; j < stats.size(); j++)
	{
		AgentStat& stat = stats[j];
		int currentMin = stat.GetValueMin();
		int currentMax = stat.GetValueMax();
		int jobLowest = -1;
		int jobHighest = -1;
		for (size_t i = 0; i < lowestStats.size(); i++)
		{
			const AgentStat& lowStat = lowestStats[i];
			if (stat.GetLowerCaseStatAbreviation() == lowStat.GetLowerCaseStatAbreviation()
				|| stat.GetLowerCaseStatName() == lowStat.GetLowerCaseStatName())
			{
				int jobMin = lowStat.GetValueMin();
				int jobMax = lowStat.GetValueMax();
				SetMinMaxStatBasedOnLowestAndHighestStat(jobLowest, jobHighest, jobMin, jobMax);
				break;
			}
		}

		for (size_t i = 0; i < highestStats.size(); i++)
		{
			const AgentStat& highStat = highestStats[i];
			if (stat.GetLowerCaseStatAbreviation() == highStat.GetLowerCaseStatAbreviation()
				|| stat.GetLowerCaseStatName() == highStat.GetLowerCaseStatName())
			{
				int jobMin = highStat.GetValueMin();
				int jobMax = highStat.GetValueMax();
				SetMinMaxStatBasedOnLowestAndHighestStat(jobLowest, jobHighest, jobMin, jobMax);

				break;
			}
		}
		SetMinMaxStatBasedOnLowestAndHighestStat(currentMin, currentMax, jobLowest, jobHighest);
		stat.SetStatAbsoluteMin(currentMin);
		stat.SetStatAbsoluteMax(currentMax);
	}
}

void AgentTemplate::SetMinMaxStatBasedOnLowestAndHighestStat(int& minStat, int& maxStat, 
	int& lowestStat, int& highestStat) const
{
	if (lowestStat != -1 && minStat < lowestStat)
	{
		minStat = lowestStat;
		if (maxStat < minStat)
		{
			maxStat = minStat;
		}
	}
	if (highestStat != -1 && maxStat < highestStat)
	{
		maxStat = highestStat;
	}
}


void AgentTemplate::CopyVariablesUniqueToAgentOfType(BaseGameEntity* entity, AgentTypes type) const
{
	if (entity == nullptr)
	{
		return;
	}

	if (type == PLAYER_AGENTTYPE)
	{
		Player* player = dynamic_cast<Player*>(entity);
		CopyUniqueVariablesToPlayer(player);
	}
	else if (type == NPC_AGENTTYPE)
	{
		NPC* npc = dynamic_cast<NPC*>(entity);
		CopyUniqueVariablesToNPC(npc);
	}
}


void AgentTemplate::CopyVariablesUniqueToAgentOfType(BaseGameEntity* entity, AgentTypes type, 
	const MapAgentSpecifications& specifications) const
{
	if (entity == nullptr)
	{
		return;
	}

	if (type == PLAYER_AGENTTYPE)
	{
		Player* player = dynamic_cast<Player*>(entity);
		CopyUniqueVariablesToPlayer(player, specifications);
	}
	else if (type == NPC_AGENTTYPE)
	{
		NPC* npc = dynamic_cast<NPC*>(entity);
		CopyUniqueVariablesToNPC(npc, specifications);
	}
}

void AgentTemplate::CopyUniqueVariablesToPlayer(Player* player) const
{
	if (player == nullptr)
	{
		return;
	}

}

void AgentTemplate::CopyUniqueVariablesToPlayer(Player* player, const MapAgentSpecifications& specifications) const
{
	if (player == nullptr)
	{
		return;
	}
	specifications;
}


void AgentTemplate::CopyUniqueVariablesToNPCCommon(NPC* npc) const
{
	bool isMale = npc->GetIsMale();
	if (m_NameGeneratorNamesToUse.size() == 0)
	{
		//npc->SetAgentName(NameGenerator::GlobalGenerateARandomNameFromDefaultByGender(isMale));
	}
	else
	{
		int nameIdx = GetRandomIntLessThan(m_NameGeneratorNamesToUse.size());
		npc->SetAgentName(
			NameGenerator::GlobalGenerateARandomNameForGeneratorNameByGender(
				m_NameGeneratorNamesToUse[nameIdx], isMale));
	}

	float addMovement = (m_npcMaximumMovementSpeed - m_npcMinimumMovementSpeed) * GetRandomFloatZeroToOne();
	npc->SetCurrentMovementSpeed(m_npcMinimumMovementSpeed + addMovement);
	npc->SetTrueMovementSpeed(m_npcMinimumMovementSpeed + addMovement);

	for (size_t idx = 0; idx < m_NPCBehaviorsIfNPC.size(); idx++)
	{
		NPCBehavior* bev = m_NPCBehaviorsIfNPC[idx];
		if (bev == nullptr)
		{
			continue;
		}
		NPCBehavior* clone = bev->Clone();
		clone->SetOwner(npc);
		npc->AddNPCBehavior(clone);
	}
	if (m_defaultDialogueSegmentChoices.size() > 0)
	{
		std::vector<int> indexsAvailable;
		for (size_t i = 0; i < m_defaultDialogueSegmentChoices.size(); i++)
		{
			if (m_defaultDialogueSegmentChoices[i].GetShouldBeAllowedToUseThisChoice())
			{
				indexsAvailable.push_back(i);
			}
		}
		if (indexsAvailable.size() > 0)
		{
			int randDialogue = GetRandomIntLessThan(indexsAvailable.size());
			randDialogue = indexsAvailable[randDialogue];
			npc->SetDialogueToOpenIfSpokenTo(m_defaultDialogueSegmentChoices[randDialogue].GetDialogueChoice());
			m_defaultDialogueSegmentChoices[randDialogue].IncrementNumberOfUsesSoFar();
		}
	}

	if (m_AttributeNamesToLoad.size() > 0)
	{
		for (size_t attrIdx = 0; attrIdx < m_AttributeNamesToLoad.size(); attrIdx++)
		{
			std::string attributeName = m_AttributeNamesToLoad[attrIdx];
			NPCAttribute attr = NPCAttributeContainer::GetRandomNPCAttribute(attributeName);
			npc->AddNPCAttribute(attr);
		}
	}
}

void AgentTemplate::CopyUniqueVariablesToNPC(NPC* npc) const
{
	if (npc == nullptr)
	{
		return;
	}

	CopyUniqueVariablesToNPCCommon(npc);
	for (size_t attributeIdx = 0; attributeIdx < m_templateNPCAttributes.size(); attributeIdx++)
	{
		NPCAttribute attribute = 
			m_templateNPCAttributes[attributeIdx]
			.GetRandomNPCAttributeBasedOffRequirements();
		npc->AddNPCAttribute(attribute);
	}

	NPCJob::DetermineMeetsAJobsRequirements(npc, m_JobTitlesToUse);
}


void AgentTemplate::CopyUniqueVariablesToNPC(NPC* npc, const MapAgentSpecifications& specifications) const
{
	if (npc == nullptr)
	{
		return;
	}
	CopyUniqueVariablesToNPCCommon(npc);

	for (size_t attributeIdx = 0; attributeIdx < m_templateNPCAttributes.size(); attributeIdx++)
	{
		bool found = false;
		int index = -1;
		for (size_t idx = 0; idx < specifications.m_agentTemplateNPCAttributes.size(); idx++)
		{
			if (m_templateNPCAttributes[attributeIdx].GetNPCAttributeToCall()
				== specifications.m_agentTemplateNPCAttributes[idx].GetNPCAttributeToCall())
			{
				found = true;
				index = (int)idx;
				break;
			}
		}
		NPCAttribute attribute = NPCAttribute::s_EmptyAttribute;
		if (found)
		{
			attribute =
				m_templateNPCAttributes[attributeIdx]
					.GetRandomNPCAttributeBasedOffRequirementsWithRespectToOther(
					specifications.m_agentTemplateNPCAttributes[index]);
		}
		else
		{
			attribute = m_templateNPCAttributes[attributeIdx]
				.GetRandomNPCAttributeBasedOffRequirements();
		}
		npc->AddNPCAttribute(attribute);
	}

	if (specifications.m_jobsDesired.size() == 0)
	{
		NPCJob::DetermineMeetsAJobsRequirements(npc, m_JobTitlesToUse);
	}
	else
	{
		NPCJob::DetermineMeetsAJobsRequirements(npc, specifications.m_jobsDesired);
		if (npc->GetJob() == nullptr)
		{
			//This should not happen. If it does, then someone's done something horrible.
			NPCJob::DetermineMeetsAJobsRequirements(npc, m_JobTitlesToUse);
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------
//Static Functions
void AgentTemplate::AddDefaultHair(const std::string& defaultHairName)
{
	if(s_DefaultHairs == nullptr)
	{
		s_DefaultHairs = new std::vector<std::string>();
	}
	s_DefaultHairs->push_back(defaultHairName);
}

void AgentTemplate::ClearDefaultHairs()
{
	if (s_DefaultHairs == nullptr)
	{
		return;
	}
	delete s_DefaultHairs;
	s_DefaultHairs = nullptr;
}