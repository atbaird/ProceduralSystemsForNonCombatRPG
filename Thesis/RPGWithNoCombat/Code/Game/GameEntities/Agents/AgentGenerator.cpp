#include "Game/GameEntities/Agents/AgentGenerator.hpp"
#include "Engine/2D/SpriteResourceHandler.hpp"
#include "Engine/Math/ExtendedMath.hpp"
#include "Engine/Core/FileUtils.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Engine/Core/Performance/Performance.hpp"
#include "Game/GameEntities/Agents/Player/Player.hpp"
#include "Game/GameEntities/Agents/NPCs/NPC.hpp"
#include "Game/GameEntities/Agents/NPCs/NPCBehavior.hpp"
#include "Game/GameEntities/Agents/Names/NameGenerator.hpp"
#include "Game/GameEntities/Agents/NPCs/NPCJobs/NPCJob.hpp"
#include "Game/GameEntities/Features/FeatureGenerator.hpp"
#include "Game/Map/MapAgentSpecifications.hpp"
#include "Game/GameEntities/Hair/HairGenerator.hpp"
#include "Game/Map/Map.hpp"
#include "Game/GameEntities/Agents/NPCs/NPCAttributes/NPCAttributeContainer.hpp"
#include "Game/GameEntities/Agents/AgentTemplateNPCAttributesContainer.hpp"
#include "Game/Common/GameDebuggingCommons.hpp"
#include "Game/TheGame.hpp"

STATIC std::vector<AgentGenerator>* AgentGenerator::s_AgentGenerators = nullptr;
STATIC AgentGeneratorLoadingData* AgentGenerator::s_loadingData = nullptr;
//-----------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------
//Constructors
AgentGenerator::AgentGenerator()
{

}

AgentGenerator::AgentGenerator(const XMLNode& node)
{   
	ReadXMLNodeAttributes(node);
	ReadXMLNodeChildren(node);
}

AgentGenerator::AgentGenerator(const AgentGenerator& other)
	: m_template(other.m_template),
	m_generatorsToInheritFrom(other.m_generatorsToInheritFrom),
	m_checkForInheritance(other.m_checkForInheritance)
{

}

AgentGenerator::~AgentGenerator()
{
}


void AgentGenerator::ReadXMLNodeAttributes(const XMLNode& node)
{
	for (int attrIdx = 0; attrIdx < node.nAttribute(); attrIdx++)
	{
		XMLAttribute attr = node.getAttribute(attrIdx);
		std::string attrName = MakeLower(attr.lpszName);
		if (attrName == "name")
		{
			m_template.SetAgentTemplateName(attr.lpszValue);
		}
		else if (attrName ==  "defaultimage"
			|| attrName == "image")
		{
			std::string str = std::string(attr.lpszValue);
			const SpriteResource* res = SpriteResourceHandler::GetSpriteResourceByName(str);
			m_template.SetDefaultImage(res, str);
		}
		else if (attrName == "movementspeed"
			|| attrName == "speed")
		{
			float speed = (float)atof(attr.lpszValue);
			m_template.SetMovementSpeed(speed);
		}
		else if (attrName == "sprintingmovementspeed"
			|| attrName == "sprintingspeed")
		{
			float speed = (float)atof(attr.lpszValue);
			m_template.SetSprintingMovementSpeed(speed);
		}
		else if (attrName ==  "inherit"
			|| attrName == "inheritfromgenerator"
			|| attrName == "inheritgenerator")
		{
			m_generatorsToInheritFrom = std::string(attr.lpszValue);
			m_checkForInheritance = true;
		}
		else if (attrName == "nameset"
			|| attrName == "names")
		{
			m_template.SetAgentNameSetToUse(attr.lpszValue);
		}
		else if (attrName == "percentagemale")
		{
			m_template.SetPercentageMale((float)atof(attr.lpszValue));
		}
	}
}

void AgentGenerator::ReadXMLNodeChildren(const XMLNode& node)
{
	for (int childIdx = 0; childIdx < node.nChildNode(); childIdx++)
	{
		XMLNode child = node.getChildNode(childIdx);
		std::string childName = MakeLower(child.getName());
		if (childName == "animations")
		{
			GetAnimationsOutOfXML(child);
		}
		else if (childName == "npcbehaviors")
		{
			GetNPCBehaviorsOutOfXML(child);
		}
		else if (childName == "namegenerators")
		{
			for (int nameGenIdx = 0; nameGenIdx < child.nChildNode(); nameGenIdx++)
			{
				XMLNode nameGen = child.getChildNode(nameGenIdx);
				m_template.AddNameGeneratorName(nameGen.getName());
			}
		}
		else if (childName == "statranges"
			|| childName == "agentstats")
		{
			m_template.ReadXMLNodeChildrenForStatRangeSettings(child);
		}
		else if (childName == "jobs")
		{
			for (int jobTitleIdx = 0; jobTitleIdx < child.nChildNode(); jobTitleIdx++)
			{
				XMLNode jobNode = child.getChildNode(jobTitleIdx);
				std::string jobTitle = jobNode.getName();
				m_template.AddJobTitle(jobTitle);
			}
		}
		else if (childName == "dialogue")
		{
			GetDialogueOutOfXML(child);
		}
		else if (childName == "hair"
			|| childName == "hairs")
		{
			GetHairsOutOfXML(child);
		}
		else if (childName == "haircolors"
			|| childName == "haircolorchoices"
			|| childName == "haircolorchoicelimits")
		{
			GetHairColorLimitsOutOfXML(child);
		}
		else if (childName == "skincolors")
		{
			GetSkinColorsOutOfXML(child);
		}
		else if (childName == "clothing")
		{
			GetClothingOutOfXML(child);
		}
		else if (childName == "npcattributes"
			|| childName == "attributes")
		{
			GetAgentAttributesOutOfXML(child);
		}
	}
}

void AgentGenerator::GetHairColorLimitsOutOfXML(const XMLNode& node)
{
	for (int childIdx = 0; childIdx < node.nChildNode(); childIdx++)
	{
		XMLNode child = node.getChildNode(childIdx);
		std::string colorChoice = std::string(child.getName());
		m_template.AddHairColorChoiceLimit(colorChoice);
	}
}

void AgentGenerator::GetSkinColorsOutOfXML(const XMLNode& node)
{
	for (int childIdx = 0; childIdx < node.nChildNode(); childIdx++)
	{
		XMLNode child = node.getChildNode(childIdx);
		std::string childName = MakeLower(child.getName());
		if (childName == "skincolor"
			|| childName == "skin")
		{
			for (int attrIdx = 0; attrIdx < child.nAttribute(); attrIdx++)
			{
				XMLAttribute attr = child.getAttribute(attrIdx);
				std::string attrName = MakeLower(attr.lpszName);
				if (attrName == "value"
					|| attrName == "skincolor")
				{
					int colorChoice = atoi(attr.lpszValue);
					m_template.AddSkinColor(colorChoice);
				}
			}
		}
	}
}

void AgentGenerator::GetHairsOutOfXML(const XMLNode& node)
{
	for (int childIdx = 0; childIdx < node.nChildNode(); childIdx++)
	{
		XMLNode child = node.getChildNode(childIdx);
		std::string nodeName = MakeLower(child.getName());
		if (nodeName == "default"
			|| nodeName == "both")
		{
			for (int grandChildIdx = 0; grandChildIdx < child.nChildNode(); grandChildIdx++)
			{
				XMLNode grandChild = child.getChildNode(grandChildIdx);
				std::string grandChildName = std::string(grandChild.getName());
				m_template.AddMaleHairName(grandChildName);
				m_template.AddFemaleHairName(grandChildName);
			}
		}
		else if (nodeName == "male")
		{
			for (int grandChildIdx = 0; grandChildIdx < child.nChildNode(); grandChildIdx++)
			{
				XMLNode grandChild = child.getChildNode(grandChildIdx);
				std::string grandChildName = std::string(grandChild.getName());
				m_template.AddMaleHairName(grandChildName);
			}
		}
		else if (nodeName == "female")
		{
			for (int grandChildIdx = 0; grandChildIdx < child.nChildNode(); grandChildIdx++)
			{
				XMLNode grandChild = child.getChildNode(grandChildIdx);
				std::string grandChildName = std::string(grandChild.getName());
				m_template.AddFemaleHairName(grandChildName);
			}
		}
	}
}

void AgentGenerator::GetDialogueOutOfXML(const XMLNode& child)
{
	for (int dialogueChoiceIdx = 0; dialogueChoiceIdx < child.nChildNode(); dialogueChoiceIdx++)
	{
		/*
		<greeting addHashTagToName='true'/>
		*/
		XMLNode dialogueChoice = child.getChildNode(dialogueChoiceIdx);
		AgentGeneratorDialogueSegmentChoice dialogueChoiceVal;
		std::string dialogueSegmentName = "";
		for (int attrIdx = 0; attrIdx < dialogueChoice.nAttribute(); attrIdx++)
		{
			XMLAttribute attr = dialogueChoice.getAttribute(attrIdx);
			std::string lowerCaseAttrName = MakeLower(attr.lpszName);
			//<QuestionWhereIsBrother limitEntities = '1' limitEntitiesPerMap = '1' allowGeneratedNormally = 'false' / >
			if (lowerCaseAttrName == "dialoguesegment"
				|| lowerCaseAttrName == "segment")
			{
				dialogueSegmentName = std::string(attr.lpszValue);
			}
			else if (lowerCaseAttrName == "limitentities"
				|| lowerCaseAttrName == "limitentitiespergame")
			{
				int limit = atoi(attr.lpszValue);
				dialogueChoiceVal.SetNumberOfUsesAllowed(limit);
			}
		}
		if (dialogueSegmentName == "")
		{
			continue;
		}
		dialogueChoiceVal.AddDialogueSegment(dialogueSegmentName);
		m_template.AddDialogueSegmentChoice(dialogueChoiceVal);
	}
}

void AgentGenerator::GetAnimationsOutOfXML(const XMLNode& child)
{
    std::vector<SpriteAnimationSequence> sequences;
    for (int animIdx = 0; animIdx < child.nChildNode(); animIdx++)
    {
        XMLNode anim = child.getChildNode(animIdx);
        Directions4 dir = INVALID_DIR4;
        int animIdxVal = -1;
		std::string animName = "";
        SpriteAnimationSequence* sequence = nullptr;
        for (int attrIdx = 0; attrIdx < anim.nAttribute(); attrIdx++)
        {
            XMLAttribute attr = anim.getAttribute(attrIdx);
            if (SimpleStrCmpLower(attr.lpszName, "animationName"))
            {
				animName = std::string(attr.lpszValue);
                sequence = SpriteResourceHandler::GetSpriteAnimationSequenceByName(animName);
            }
            else if (SimpleStrCmpLower(attr.lpszName, "direction"))
            {
                int val = atoi(attr.lpszValue);
                if (val != 0 || (val == 0 && SimpleStrCmp(attr.lpszValue, "0")))
                {
                    dir = (Directions4)(val);
                }
                else
                {
                    if (SimpleStrCmpLower(attr.lpszValue, "north"))
                    {
                        dir = NORTH;
                    }
                    else if (SimpleStrCmpLower(attr.lpszValue, "west"))
                    {
                        dir = WEST;
                    }
                    else if (SimpleStrCmpLower(attr.lpszValue, "south"))
                    {
                        dir = SOUTH;
                    }
                    else if (SimpleStrCmpLower(attr.lpszValue, "east"))
                    {
                        dir = EAST;
                    }
                }
            }
            else if (SimpleStrCmpLower(attr.lpszName, "animIdx"))
            {
                int val = atoi(attr.lpszValue);
                if (val != 0 || (val == 0 && SimpleStrCmp(attr.lpszValue, "0")))
                {
                    animIdxVal = val;
                }
                else
                {
                    if (SimpleStrCmpLower(attr.lpszValue, "walk"))
                    {
                        animIdxVal = WALK;
                    }
                }
            }
        }
        m_template.AddSpriteAnimationSequence(sequence, dir, animIdxVal, animName);
    }
}

void AgentGenerator::GetNPCBehaviorsOutOfXML(const XMLNode& child)
{
    for (int bevIdx = 0; bevIdx < child.nChildNode(); bevIdx++)
    {
        XMLNode bevNode = child.getChildNode(bevIdx);
        std::string bevName = bevNode.getName();
        NPCBehavior* bev = NPCBehaviorRegistrationHelper::CreateBehaviorByName(bevName, bevNode);
        if (bev == nullptr)
        {
            continue;
        }
        m_template.RegisterNPCBehavior(bev);
    }
}

void AgentGenerator::GetClothingOutOfXML(const XMLNode& child)
{
	for (int clothingIdx = 0; clothingIdx < child.nChildNode(); clothingIdx++)
	{
		XMLNode clothingNode = child.getChildNode(clothingIdx);
		for (int attrIdx = 0; attrIdx < clothingNode.nAttribute(); attrIdx++)
		{
			XMLAttribute attr = clothingNode.getAttribute(attrIdx);
			std::string attrName = MakeLower(attr.lpszName);
			if (attrName == "name"
				|| attrName == "clothing"
				|| attrName == "clothingname")
			{
				m_template.AddClothingName(attr.lpszValue);
			}
		}
	}
}

void AgentGenerator::GetAgentAttributesOutOfXML(const XMLNode& child)
{
	for (int attributeIdx = 0; attributeIdx < child.nChildNode(); attributeIdx++)
	{
		XMLNode attribute = child.getChildNode(attributeIdx);
		std::string attributeName = MakeLower(attribute.getName());
		bool useWhitelist = false;
		bool useBlacklist = false;
		std::vector<std::string> whitelistValues;
		std::vector<std::string> blacklistValues;

		for (int attributeChildIdx = 0; attributeChildIdx < attribute.nChildNode(); attributeChildIdx++)
		{
			XMLNode attributeChild = attribute.getChildNode(attributeChildIdx);
			std::string attributeChildName = MakeLower(attributeChild.getName());
			if (attributeChildName == "whitelist")
			{
				useWhitelist = true;
				for (int whitelistIdx = 0; whitelistIdx < attributeChild.nChildNode(); whitelistIdx++)
				{
					XMLNode whitelistThis = attributeChild.getChildNode(whitelistIdx);
					std::string whiteListVal = MakeLower(whitelistThis.getName());
					for (int whiteListAttrIdx = 0; whiteListAttrIdx < whitelistThis.nAttribute(); whiteListAttrIdx++)
					{
						XMLAttribute attr = whitelistThis.getAttribute(whiteListAttrIdx);
						std::string attrName = MakeLower(attr.lpszName);
						if (attrName == "name"
							|| attrName == "value")
						{
							whiteListVal = MakeLower(attr.lpszValue);
						}
					}

					whitelistValues.push_back(whiteListVal);
				}
			}
			else if (attributeChildName == "blacklist")
			{
				useBlacklist = true;
				for (int blacklistIdx = 0; blacklistIdx < attributeChild.nChildNode(); blacklistIdx++)
				{
					XMLNode blacklistThis = attributeChild.getChildNode(blacklistIdx);
					std::string blackListVal = MakeLower(blacklistThis.getName());
					for (int blackListAttrIdx = 0; blackListAttrIdx < blacklistThis.nAttribute(); blackListAttrIdx++)
					{
						XMLAttribute attr = blacklistThis.getAttribute(blackListAttrIdx);
						std::string attrName = MakeLower(attr.lpszName);
						if (attrName == "name"
							|| attrName == "value")
						{
							blackListVal = MakeLower(attr.lpszValue);
						}
					}
					blacklistValues.push_back(blackListVal);
				}
			}
		}
		AgentTemplateNPCAttributesContainer npcAttributeContainer =
			AgentTemplateNPCAttributesContainer(attributeName,
				whitelistValues, blacklistValues, useWhitelist, useBlacklist);
		m_template.AddTemplateNPCAttributesContainer(npcAttributeContainer);

	}
}


void AgentGenerator::InheritPropertiesFromGenerator(AgentGenerator* generator)
{
	if (generator == nullptr)
	{
		return;
	}
	m_template.CopyVariablesOffOtherTemplate(generator->m_template);
}

void AgentGenerator::ParseStringOfGeneratorsToInheritFrom()
{
	std::string val = ReplaceCharInString(m_generatorsToInheritFrom, ',', ' ');
	std::vector<std::string> parsed = ParseString(val, ' ');
	for (size_t idx = 0; idx < parsed.size(); idx++)
	{
		std::string generatorToInheritFrom = parsed[idx];
		AgentGenerator* generator = GetEditableAgentGeneratorByAgentName(generatorToInheritFrom.c_str());
		if (generator == this)
		{
			continue;
		}
		InheritPropertiesFromGenerator(generator);
	}
}

void AgentGenerator::WriteAgentGeneratorToString(std::string& str, int indentation) const
{
	GameDebuggingCommons::AddIndentation(str, indentation);
	str += "!---!\n";
	GameDebuggingCommons::AddIndentation(str, indentation);
	str += "Agent Generator: " + std::string(m_template.GetAgentName()) + "\n";
	GameDebuggingCommons::AddIndentation(str, indentation);
	str += "Inherits from: " + (m_generatorsToInheritFrom) + "\n";
	indentation += 3;
	m_template.WriteAgentGeneratorToString(str, indentation);
}

//-----------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------
//Getters
const char* AgentGenerator::GetAgentName() const
{
    return m_template.GetAgentName();
}

std::string AgentGenerator::GetGeneratorsToInheritFrom() const
{
	return m_generatorsToInheritFrom;
}

bool AgentGenerator::GetCheckForInheritance() const
{
	return m_checkForInheritance;
}

//-----------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------
//Spawn Objects
BaseGameEntity* AgentGenerator::SpawnPlayerType(const Vector2& position, const Map* map) const
{
    const SpriteResource* res = nullptr;
    {
        res = m_template.GetDefaultImage();
    }

	Player* player = new Player(res);
	player->SetAgentGeneratorUsed(GetAgentName());
	if (map == nullptr)
	{
		map = Map::GetEditableMap();
	}
    player->SetPosition(position, map);
    {
        m_template.CopyVariablesToAgent(player);
        m_template.CopyVariablesUniqueToAgentOfType(player, PLAYER_AGENTTYPE);
	}
	player->SetGeneratorUsed(MakeLower(m_template.GetAgentName()));
	player->SetAgentName("Player");
    return player;
}

BaseGameEntity* AgentGenerator::SpawnNPCType(const Vector2& position, const Map* map) const
{
    const SpriteResource* res = nullptr;
    {
        res = m_template.GetDefaultImage();
    }

	NPC* npc = new NPC(res);
	npc->SetAgentGeneratorUsed(GetAgentName());
	if (map == nullptr)
	{
		npc->SetPosition(position);
	}
	else
	{
		npc->SetPosition(position, map);
	}
    {
        m_template.CopyVariablesToAgent(npc);
        m_template.CopyVariablesUniqueToAgentOfType(npc, NPC_AGENTTYPE);
	}
	npc->SetGeneratorUsed(MakeLower(m_template.GetAgentName()));
    return npc;
}


BaseGameEntity* AgentGenerator::SpawnNPCType(const MapAgentSpecifications& specifications, const Map* map) const
{
	Vector2 position = Vector2::vec2_zeros;

	if (specifications.m_tileCoordsSet && specifications.m_validSpawnPointTilesToUse.size() > 0)
	{
		Map* currentMap = Map::GetEditableMap();
		int index = GetRandomIntLessThan(specifications.m_validSpawnPointTilesToUse.size());
		TILE_COORDS coordsForPosition = specifications.m_validSpawnPointTilesToUse[index];
		position = currentMap->GetCenterOfTileAtTileCoords(coordsForPosition);
	}
	else
	{
		Map* currentMap = Map::GetEditableMap();
		position = currentMap->GetRandomPositionOnMap();
	}

	const SpriteResource* res = nullptr;
	{
		res = m_template.GetDefaultImage();
	}
	NPC* npc = new NPC(res);
	npc->SetAgentGeneratorUsed(GetAgentName());
	m_template.CopyVariablesToAgent(npc, specifications);
	m_template.CopyVariablesUniqueToAgentOfType(npc, NPC_AGENTTYPE, specifications);
	npc->SetGeneratorUsed(MakeLower(m_template.GetAgentName()));

	if (map == nullptr)
	{
		npc->SetPosition(position);
	}
	else
	{
		npc->SetPosition(position, map);
	}
	return npc;
}

//-----------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------
//Static Methods
STATIC void AgentGenerator::WriteAllAgentGeneratorsToString(std::string& str, int indentation)
{
	if (s_AgentGenerators == nullptr)
	{
		str += "\nNone Found.";
		return;
	}
	str += "\nNumber of Agent Generators: " + std::to_string(s_AgentGenerators->size());
	str += "\n";
	for (size_t i = 0; i < s_AgentGenerators->size(); i++)
	{
		const AgentGenerator& generator = s_AgentGenerators->at(i);
		generator.WriteAgentGeneratorToString(str, indentation);
	}
}

STATIC void AgentGenerator::RegisterDefaultHair(std::string hairName)
{
	AgentTemplate::AddDefaultHair(hairName);
}

STATIC void AgentGenerator::ClearAllGenerators()
{
	AgentTemplate::ClearDefaultHairs();
	HairGenerator::ClearAllGenerators();
	NPCJob::DestroyAllJobs();
	NameGenerator::ClearAllGenerators();
	FeatureGenerator::ClearAllGenerators();
	AgentStat::ClearAllAgentStats();
	NPCAttributeContainer::ClearNPCAttributes();
	if (s_loadingData != nullptr)
	{
		delete s_loadingData;
		s_loadingData = nullptr;
	}

    if (s_AgentGenerators == nullptr)
    {
        return;
    }
    delete s_AgentGenerators;
    s_AgentGenerators = nullptr;
}

STATIC void AgentGenerator::ReadInGeneratorsFromCommonAgentGeneratorsFolderAllAtOnce()
{
	NPCAttributeContainer::ReadInNPCAttributesFromCommonFolderAllAtOnce();
	NameGenerator::ReadInGeneratorsFromCommonNameGeneratorsFolderAllAtOnce();
	HairGenerator::ReadInGeneratorsFromCommonHairGeneratorsFolderAllAtOnce();
	AgentStat::ReadInAllAgentStatsFromCommonFolderAllAtOnce();
	NPCJob::RegisterAllJobsFromXMLFileAllAtOnce();
	FeatureGenerator::LoadAllFeaturesFromXMLAllAtOnce();

	std::string commonAgentGeneratorFileDirection = "Data/XML/AgentGenerators/";
	std::string agentGeneratorsRootNodeName = "AgentGenerators";
	std::string agentGeneratorsNodeName = "AgentGenerator";
	GameSettings* settings = TheGame::GetGameSettings();
	if (settings != nullptr)
	{
		commonAgentGeneratorFileDirection = settings->m_commonAgentGeneratorFileDirectory;
		agentGeneratorsRootNodeName = settings->m_agentGeneratorsRootNodeName;
		agentGeneratorsNodeName = settings->m_agentGeneratorsNodeName;
	}

    std::vector<std::string> files = FileUtils::EnumerateFilesInDirectory(commonAgentGeneratorFileDirection,
		"*", true);

    for (size_t fileIdx = 0; fileIdx < files.size(); fileIdx++)
    {
        std::string fileStr = files.at(fileIdx);
		ReadAgentGeneratorFromSingleXMLFileLocation(settings, fileStr, 
			agentGeneratorsRootNodeName, agentGeneratorsNodeName);
	}

	for (size_t idx = 0; idx < s_AgentGenerators->size(); idx++)
	{
		AgentGenerator* generator = &s_AgentGenerators->at(idx);
		if (generator != nullptr
			&& generator->m_checkForInheritance)
		{
			generator->ParseStringOfGeneratorsToInheritFrom();
		}
	}

}

STATIC bool AgentGenerator::OnUpdateReadInGeneratorsFromCommonAgentGeneratorsFolder()
{
	if (s_loadingData == nullptr)
	{
		s_loadingData = new STATIC AgentGeneratorLoadingData();
	}
	GameSettings* settings = TheGame::GetGameSettings();

	if (!s_loadingData->m_readData)
	{
		s_loadingData->m_readData = true;
		if (settings != nullptr)
		{
			s_loadingData->m_fileDirectory = settings->m_commonAgentGeneratorFileDirectory;
			s_loadingData->m_rootNodeName = settings->m_agentGeneratorsRootNodeName;
			s_loadingData->m_NodeName = settings->m_agentGeneratorsNodeName;
			s_loadingData->m_amountOfTimeToTakePerFrame = settings->m_amountOfTimeLoadingAgentGeneratorsPerFrame;
		}
		s_loadingData->m_files = FileUtils::EnumerateFilesInDirectory(s_loadingData->m_fileDirectory,
			"*", true);
		s_loadingData->m_index = 0;
		s_loadingData->m_loadingPhase = true;
		return s_loadingData->m_files.size() == 0;
	}

	if (s_loadingData->m_amountOfTimeToTakePerFrame <= 0)
	{
		return true;
	}
	int numberToReadThisFrame = 0;

	if (s_loadingData->m_loadingPhase)
	{
		while ((size_t)s_loadingData->m_index < s_loadingData->m_files.size())
		{
			std::string fileStr = s_loadingData->m_files[s_loadingData->m_index];
			ReadAgentGeneratorFromSingleXMLFileLocation(settings, fileStr,
				s_loadingData->m_rootNodeName, s_loadingData->m_NodeName);
			numberToReadThisFrame++;
			s_loadingData->m_index++;
			uint64_t timeDiff = Performance::GetCurrentPerformanceCount() - g_TheGame->GetFrameStartTime();
			double timeDiffDouble = Performance::PerformanceCountToSeconds(timeDiff);
			if (timeDiffDouble >= s_loadingData->m_amountOfTimeToTakePerFrame)
			{
				return false;
			}
		}
		if ((size_t)s_loadingData->m_index >= s_loadingData->m_files.size())
		{
			s_loadingData->m_loadingPhase = false;
			s_loadingData->m_index = 0;
		}
		return false;
	}
	else
	{
		if (s_AgentGenerators == nullptr)
		{
			return true;
		}
		while ((size_t)s_loadingData->m_index < s_AgentGenerators->size())
		{
			AgentGenerator* generator = &s_AgentGenerators->at(s_loadingData->m_index);
			if (generator != nullptr
				&& generator->m_checkForInheritance)
			{
				generator->ParseStringOfGeneratorsToInheritFrom();
			}
			numberToReadThisFrame++;
			s_loadingData->m_index++;
			uint64_t timeDiff = Performance::GetCurrentPerformanceCount() - g_TheGame->GetFrameStartTime();
			double timeDiffDouble = Performance::PerformanceCountToSeconds(timeDiff);
			if (timeDiffDouble >= s_loadingData->m_amountOfTimeToTakePerFrame)
			{
				return false;
			}
		}

		return (size_t)s_loadingData->m_index >= s_AgentGenerators->size();
	}
}

STATIC void AgentGenerator::ReadAgentGeneratorFromSingleXMLFileLocation(GameSettings* settings,
	const std::string& fileStr,
	const std::string& agentGeneratorsRootNodeName,
	const std::string& agentGeneratorsNodeName)
{
	std::string last3Letters = fileStr.substr(fileStr.size() - 3, 3);
	if (SimpleStrCmpLower(last3Letters, "xml") == false)
	{
		return;
	}

	bool errorOccurred = false;
	XMLNode root = EngineXMLParser::ParseXMLFile(fileStr, agentGeneratorsRootNodeName.c_str(), errorOccurred);
	if (errorOccurred == true)
	{
		return;
	}

	for (int childIdx = 0; childIdx < root.nChildNode(); childIdx++)
	{
		XMLNode child = root.getChildNode(childIdx);
		if (SimpleStrCmpLower(child.getName(), agentGeneratorsNodeName))
		{
			AgentGenerator newGenerator = AgentGenerator(child);
			if (s_AgentGenerators == nullptr)
			{
				s_AgentGenerators = new std::vector<AgentGenerator>();
				unsigned int preallocationAmt = 50;
				if (settings != nullptr)
				{
					preallocationAmt = settings->m_preallocationAmountAgentGenerators;
				}
				s_AgentGenerators->reserve(preallocationAmt);
			}
			s_AgentGenerators->push_back(newGenerator);
		}
	}
}

STATIC const AgentGenerator* AgentGenerator::GetAgentGeneratorByAgentName(const char* name)
{
    if (s_AgentGenerators == nullptr
		|| name == nullptr)
    {
        return nullptr;
	}
	std::string lowerCase = MakeLower(name);
    for (size_t i = 0; i < s_AgentGenerators->size(); i++)
    {
        std::string nam = MakeLower(s_AgentGenerators->at(i).GetAgentName());
        if (nam == lowerCase)
        {
            return &s_AgentGenerators->at(i);
        }
    }
    return nullptr;
}

STATIC const AgentGenerator* AgentGenerator::GetAgentGeneratorByIndex(size_t index)
{
    if (s_AgentGenerators == nullptr || index >= s_AgentGenerators->size())
    {
        return nullptr;
    }
    return &s_AgentGenerators->at(index);
}

STATIC AgentGenerator* AgentGenerator::GetEditableAgentGeneratorByAgentName(const char* name)
{
	if (s_AgentGenerators == nullptr)
	{
		return nullptr;
	}
	for (size_t i = 0; i < s_AgentGenerators->size(); i++)
	{
		const char* nam = s_AgentGenerators->at(i).GetAgentName();
		if (nam != nullptr && SimpleStrCmp(name, nam))
		{
			return &s_AgentGenerators->at(i);
		}
	}
	return nullptr;
}

STATIC AgentGenerator* AgentGenerator::GetEditableAgentGeneratorByIndex(size_t index)
{
	if (s_AgentGenerators == nullptr || index >= s_AgentGenerators->size())
	{
		return nullptr;
	}
	return &s_AgentGenerators->at(index);
}

STATIC BaseGameEntity* AgentGenerator::SpawnPlayerInAgentGeneratorByAgentName(const char* name, const Vector2& position,
	const Map* map)
{
    const AgentGenerator* gen = GetAgentGeneratorByAgentName(name);
    if (gen == nullptr)
    {
        return nullptr;
    }
    return gen->SpawnPlayerType(position, map);
}

STATIC BaseGameEntity* AgentGenerator::SpawnPlayerInAgentGeneratorByIndex(size_t index, const Vector2& position,
	const Map* map)
{
    const AgentGenerator* gen = GetAgentGeneratorByIndex(index);
    if (gen == nullptr)
    {
        return nullptr;
    }
    return gen->SpawnPlayerType(position, map);
}

STATIC BaseGameEntity* AgentGenerator::SpawnNPCInAgentGeneratorByAgentName(const char* name, const Vector2& position,
	const Map* map)
{
    const AgentGenerator* gen = GetAgentGeneratorByAgentName(name);
    if (gen == nullptr)
    {
        return nullptr;
    }
    return gen->SpawnNPCType(position, map);
}

STATIC BaseGameEntity* AgentGenerator::SpawnNPCInAgentGeneratorByIndex(size_t index, const Vector2& position,
	const Map* map)
{
    const AgentGenerator* gen = GetAgentGeneratorByIndex(index);
    if (gen == nullptr)
    {
        return nullptr;
    }
    return gen->SpawnNPCType(position, map);
}

STATIC BaseGameEntity* AgentGenerator::SpawnNPCInAgentGeneratorRandom(const Vector2& position,
	const Map* map)
{
    if (s_AgentGenerators == nullptr)
    {
        return nullptr;
    }
    size_t idx = (size_t)floor(((float)rand() / (float)RAND_MAX) * (float)s_AgentGenerators->size());
    return SpawnNPCInAgentGeneratorByIndex(idx, position, map);
}


STATIC void AgentGenerator::SpawnNPCsInAgentGeneratorsBasedOffMapAgentSpecifications(
	const MapAgentSpecifications& specifications, std::vector<BaseGameEntity*>& agentsOut,
	const Map* map)
{
	int numberToSpawn = GetRandomIntInRange(specifications.m_minNumberToSpawn, specifications.m_maxNumberToSpawn);
	if (numberToSpawn <= 0
		|| specifications.m_agentGeneratorToUse == "")
	{
		return;
	}

	const AgentGenerator* gen = GetAgentGeneratorByAgentName(specifications.m_agentGeneratorToUse.c_str());
	if (gen == nullptr)
	{
		return;
	}

	for (size_t agentIdx = 0; agentIdx < (size_t)numberToSpawn; agentIdx++)
	{
		BaseGameEntity* agentEntity = gen->SpawnNPCType(specifications, map);
		agentsOut.push_back(agentEntity);
	}
}

STATIC void AgentGenerator::SpawnNPCsInAgentGeneratorsBasedOffMapAgentSpecifications(
	const std::vector<MapAgentSpecifications>& specifications, std::vector<BaseGameEntity*>& agentsOut,
	const Map* map)
{
	for (size_t specificationsIdx = 0; specificationsIdx < specifications.size(); specificationsIdx++)
	{
		MapAgentSpecifications specification = specifications[specificationsIdx];
		SpawnNPCsInAgentGeneratorsBasedOffMapAgentSpecifications(specification, agentsOut, map);
	}
}