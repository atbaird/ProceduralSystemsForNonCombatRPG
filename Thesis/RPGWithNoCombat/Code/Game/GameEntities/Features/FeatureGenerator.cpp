#include "Game/GameEntities/Features/FeatureGenerator.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Engine/Core/FileUtils.hpp"
#include "Engine/2D/SpriteResourceHandler.hpp"
#include "Engine/Core/Performance/Performance.hpp"
#include "Game/GameEntities/Features/Feature.hpp"
#include "Game/GameEntities/Features/FeatureBehavior/FeatureBehaviorRegistrationHelper.hpp"
#include "Game/Common/GameDebuggingCommons.hpp"
#include "Game/TheGame.hpp"

//--------------------------------------------------------
//--------------------------------------------------------
//--------------------------------------------------------
//Static Variables
STATIC std::vector<FeatureGenerator>* FeatureGenerator::s_FeatureGenerators = nullptr;
STATIC FeatureGeneratorLoadingData* FeatureGenerator::s_loadingData = nullptr;

//--------------------------------------------------------
//--------------------------------------------------------
//--------------------------------------------------------
//Constructors
FeatureGenerator::FeatureGenerator()
{

}

FeatureGenerator::FeatureGenerator(const XMLNode& node)
{
	for (int attrIdx = 0; attrIdx < node.nAttribute(); attrIdx++)
	{
		XMLAttribute attr = node.getAttribute(attrIdx);
		std::string attrName = MakeLower(attr.lpszName);
		if (attrName == "name")
		{
			m_FeatureGenName = std::string(attr.lpszValue);
		}
		else if (attrName ==  "defaultimage")
		{
			std::string defaultSprite = std::string(attr.lpszValue);
			const SpriteResource* res = SpriteResourceHandler::GetSpriteResourceByName(defaultSprite);
			m_template.SetDefaultImage(res, defaultSprite);
		}
		else if (attrName == "solid"
			|| attrName == "issolid")
		{
			std::string val = MakeLower(attr.lpszValue);
			if (val == "true" || val == "t")
			{
				m_template.SetIsSolid(true, true);
			}
			else
			{
				m_template.SetIsSolid(false, true);
			}
		}
		else if (attrName == "renderoffset"
			|| attrName == "offset"
			|| attrName == "spriteoffset")
		{
			bool sizeOf1 = false;
			Vector2 renderOffset = EngineXMLParser::ParseXMLAttributeForVector2(attr, sizeOf1);
			if (sizeOf1)
			{
				m_template.SetRenderOffset(renderOffset);
			}
		}
	}

	for (int childIdx = 0; childIdx < node.nChildNode(); childIdx++)
	{
		XMLNode child = node.getChildNode(childIdx);
		if (SimpleStrCmpLower(child.getName(), "animations"))
		{
			GetAnimationsOutOfXML(child);
		}
		else if (SimpleStrCmpLower(child.getName(), "featurebehaviors"))
		{
			GetFeatureBehaviorsOutOfXML(child);
		}
	}
}

FeatureGenerator::FeatureGenerator(const FeatureGenerator& other)
	: m_template(other.m_template),
	m_FeatureGenName(other.m_FeatureGenName)
{

}

FeatureGenerator::~FeatureGenerator()
{

}


void FeatureGenerator::GetAnimationsOutOfXML(const XMLNode& child)
{
	std::vector<SpriteAnimationSequence> sequences;
	for (int animIdx = 0; animIdx < child.nChildNode(); animIdx++)
	{
		XMLNode anim = child.getChildNode(animIdx);
		Directions4 dir = INVALID_DIR4;
		int animIdxVal = -1;
		SpriteAnimationSequence* sequence = nullptr;
		std::string animName = "";
		for (int attrIdx = 0; attrIdx < anim.nAttribute(); attrIdx++)
		{
			XMLAttribute attr = anim.getAttribute(attrIdx);
			std::string attrName = MakeLower(attr.lpszName);
			if (attrName == "animationname")
			{
				animName = std::string(attr.lpszValue);
				sequence = SpriteResourceHandler::GetSpriteAnimationSequenceByName(animName);
			}
			else if (attrName ==  "direction")
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
			else if (attrName ==  "animidx")
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

void FeatureGenerator::GetFeatureBehaviorsOutOfXML(const XMLNode& child)
{
	for (int bevIdx = 0; bevIdx < child.nChildNode(); bevIdx++)
	{
		XMLNode bevNode = child.getChildNode(bevIdx);
		//<SwitchSides type = 'Interactive' / >
		std::string bevName = bevNode.getName();
		FeatureBehavior* bev = FeatureBehaviorRegistrationHelper::CreateFeatureBehaviorByName(bevName, bevNode);
		if (bev == nullptr)
		{
			continue;
		}
		if (bev->GetDesiredBehaviorTypeNotSet())
		{
			m_template.AddFeatureBehavior(m_template.GetNumberOfFeatureBehaviors(), bev);
		}
		else
		{
			m_template.AddFeatureBehavior(bev->GetDesiredBehaviorType(), bev);
		}
	}
}

//--------------------------------------------------------
//--------------------------------------------------------
//--------------------------------------------------------
//Setters

//--------------------------------------------------------
//--------------------------------------------------------
//--------------------------------------------------------
//Getters

//--------------------------------------------------------
//--------------------------------------------------------
//--------------------------------------------------------
//Operations
BaseGameEntity* FeatureGenerator::SpawnFeature()
{
	Feature* feature = new Feature();
	m_template.CopyDataOntoFeature(feature);
	feature->SetGeneratorUsed(MakeLower(m_FeatureGenName));
	feature->SetAgentName(m_FeatureGenName);
	return feature;
}

BaseGameEntity* FeatureGenerator::SpawnFeature(const XMLNode& node)
{
	Feature* feature = new Feature();
	m_template.CopyDataOntoFeature(feature, node);
	feature->SetGeneratorUsed(MakeLower(m_FeatureGenName));
	feature->SetAgentName(m_FeatureGenName);
	return feature;
}

//--------------------------------------------------------
//--------------------------------------------------------
//--------------------------------------------------------
//Getters
void FeatureGenerator::WriteGeneratorToString(std::string& str, int indentationLvl) const
{
	GameDebuggingCommons::AddIndentation(str, indentationLvl);
	str += "Feature Generator Name: " + m_FeatureGenName + "\n";
	indentationLvl += 3;
	m_template.WriteFeatureTemplateToString(str, indentationLvl);
}

//--------------------------------------------------------
//--------------------------------------------------------
//--------------------------------------------------------
//Static Functions
STATIC void FeatureGenerator::WriteAllFeatureGeneratorsToString(std::string& str, int indentationLvl)
{
	if (s_FeatureGenerators == nullptr)
	{
		return;
	}

	for (size_t i = 0; i < s_FeatureGenerators->size(); i++)
	{
		const FeatureGenerator& gen = s_FeatureGenerators->at(i);
		gen.WriteGeneratorToString(str, indentationLvl);
	}
}

STATIC void FeatureGenerator::LoadAllFeaturesFromXMLAllAtOnce()
{
	std::string commonFeatureGeneratorFileDirection = "Data/XML/FeatureGenerators/";
	std::string featureGeneratorsRootNodeName = "FeatureGenerators";
	std::string featureGeneratorsNodeName = "FeatureGenerator";
	GameSettings* settings = TheGame::GetGameSettings();
	if (settings != nullptr)
	{
		commonFeatureGeneratorFileDirection = settings->m_commonFeatureGeneratorFileDirectory;
		featureGeneratorsRootNodeName = settings->m_featureGeneratorsRootNodeName;
		featureGeneratorsNodeName = settings->m_featureGeneratorsNodeName;
	}

	std::vector<std::string> files = 
		FileUtils::EnumerateFilesInDirectory(commonFeatureGeneratorFileDirection, "*", true);

	for (size_t fileIdx = 0; fileIdx < files.size(); fileIdx++)
	{
		std::string fileStr = files.at(fileIdx);
		LoadSingleFeatureGeneratorFromXMLFileLocation(settings, fileStr,
			featureGeneratorsRootNodeName, featureGeneratorsNodeName);
		
	}
}

STATIC bool FeatureGenerator::OnUpdateLoadAllFeaturesFromXML()
{
	if (s_loadingData == nullptr)
	{
		s_loadingData = new STATIC FeatureGeneratorLoadingData();
	}

	GameSettings* settings = TheGame::GetGameSettings();
	if (!s_loadingData->m_readData)
	{
		if (settings != nullptr)
		{
			s_loadingData->m_fileDirectory = settings->m_commonFeatureGeneratorFileDirectory;
			s_loadingData->m_rootNodeName = settings->m_featureGeneratorsRootNodeName;
			s_loadingData->m_NodeName = settings->m_featureGeneratorsNodeName;
			s_loadingData->m_amountOfTimeToTakeReadingPerFrame = settings->m_amountOfTimeLoadingFeatureGeneratorsPerFrame;
		}
		s_loadingData->m_readData = true;
		s_loadingData->m_files = FileUtils::EnumerateFilesInDirectory(s_loadingData->m_fileDirectory, "*", true);
		return s_loadingData->m_files.size() == 0;
	}
	if (s_loadingData->m_amountOfTimeToTakeReadingPerFrame <= 0)
	{
		return true;
	}
	int numberReadThisFrame = 0;
	while ((size_t)s_loadingData->m_index < s_loadingData->m_files.size())
	{
		std::string fileStr = s_loadingData->m_files[s_loadingData->m_index];
		LoadSingleFeatureGeneratorFromXMLFileLocation(settings, fileStr,
			s_loadingData->m_rootNodeName, s_loadingData->m_NodeName);
		s_loadingData->m_index++;
		numberReadThisFrame++;
		uint64_t timeDiff = Performance::GetCurrentPerformanceCount() - g_TheGame->GetFrameStartTime();
		double timeDiffDouble = Performance::PerformanceCountToSeconds(timeDiff);
		if (timeDiffDouble >= s_loadingData->m_amountOfTimeToTakeReadingPerFrame)
		{
			return false;
		}
	}
	return (size_t)s_loadingData->m_index >= s_loadingData->m_files.size();
}

STATIC void FeatureGenerator::LoadSingleFeatureGeneratorFromXMLFileLocation(GameSettings* settings,
	const std::string& fileStr,
	const std::string& featureGeneratorsRootNodeName,
	const std::string& featureGeneratorsNodeName)
{
	std::string last3Letters = fileStr.substr(fileStr.size() - 3, 3);
	if (SimpleStrCmpLower(last3Letters, "xml") == false)
	{
		return;
	}

	bool errorOccurred = false;
	XMLNode root = EngineXMLParser::ParseXMLFile(fileStr, featureGeneratorsRootNodeName.c_str(), errorOccurred);
	if (errorOccurred == true)
	{
		return;
	}

	for (int childIdx = 0; childIdx < root.nChildNode(); childIdx++)
	{
		XMLNode child = root.getChildNode(childIdx);
		if (SimpleStrCmpLower(child.getName(), featureGeneratorsNodeName))
		{
			FeatureGenerator newGenerator = FeatureGenerator(child);
			if (s_FeatureGenerators == nullptr)
			{
				s_FeatureGenerators = new std::vector<FeatureGenerator>();
				unsigned int preallocationAmt = 50;
				if (settings != nullptr)
				{
					preallocationAmt = settings->m_preallocationAmountFeatureGenerators;
				}
				s_FeatureGenerators->reserve(preallocationAmt);
			}
			s_FeatureGenerators->push_back(newGenerator);
		}
	}
}

STATIC void FeatureGenerator::ClearAllGenerators()
{
	if (s_loadingData != nullptr)
	{
		delete s_loadingData;
		s_loadingData = nullptr;
	}
	if (s_FeatureGenerators == nullptr)
	{
		return;
	}
	std::vector<FeatureGenerator>* generators = s_FeatureGenerators;
	s_FeatureGenerators = nullptr;

	delete generators;
}

STATIC BaseGameEntity* FeatureGenerator::SpawnFeatureByFeatureName(const char* name, const Vector2& position, 
	const Map* map)
{
	if (s_FeatureGenerators == nullptr)
	{
		return nullptr;
	}
	std::string lowerCaseName = MakeLower(name);

	for (size_t featureGenIdx = 0; featureGenIdx < s_FeatureGenerators->size(); featureGenIdx++)
	{
		FeatureGenerator* gen = &s_FeatureGenerators->at(featureGenIdx);
		if (gen == nullptr)
		{
			continue;
		}
		std::string featureName = MakeLower(gen->m_FeatureGenName);
		if (featureName == lowerCaseName)
		{
			BaseGameEntity* feature = gen->SpawnFeature();
			if (map == nullptr)
			{
				map = Map::GetMap();
			}
			feature->SetPosition(position, map);
			Feature* featureFeature = dynamic_cast<Feature*>(feature);
			featureFeature->TriggerPlacementActions(map);
			return feature;
		}
	}
	return nullptr;
}

STATIC BaseGameEntity* FeatureGenerator::SpawnFeatureByIndex(size_t index, const Vector2& position, const Map* map)
{
	if (s_FeatureGenerators == nullptr
		|| index >= s_FeatureGenerators->size())
	{
		return nullptr;
	}
	FeatureGenerator* gen = &s_FeatureGenerators->at(index);
	if (gen == nullptr)
	{
		return nullptr;
	}
	BaseGameEntity* feature = gen->SpawnFeature();
	if (map == nullptr)
	{
		map = Map::GetMap();
	}
	feature->SetPosition(position, map);
	Feature* featureFeature = dynamic_cast<Feature*>(feature);
	featureFeature->TriggerPlacementActions(map);

	return nullptr;
}


STATIC BaseGameEntity* FeatureGenerator::SpawnFeatureByFeatureName(const char* name,
	const Vector2& position, const XMLNode& node, const Map* map)
{
	if (s_FeatureGenerators == nullptr)
	{
		return nullptr;
	}
	std::string lowerCaseName = MakeLower(name);

	for (size_t featureGenIdx = 0; featureGenIdx < s_FeatureGenerators->size(); featureGenIdx++)
	{
		FeatureGenerator* gen = &s_FeatureGenerators->at(featureGenIdx);
		if (gen == nullptr)
		{
			continue;
		}
		std::string featureName = MakeLower(gen->m_FeatureGenName);
		if (featureName == lowerCaseName)
		{
			BaseGameEntity* feature = gen->SpawnFeature(node);
			if (map == nullptr)
			{
				map = Map::GetMap();
			}
			feature->SetPosition(position, map);
			Feature* featureFeature = dynamic_cast<Feature*>(feature);
			featureFeature->TriggerPlacementActions(map);
			return feature;
		}
	}
	return nullptr;
}

STATIC BaseGameEntity* FeatureGenerator::SpawnFeatureByIndex(size_t index,
	const Vector2& position, const XMLNode& node, const Map* map)
{
	if (s_FeatureGenerators == nullptr
		|| index >= s_FeatureGenerators->size())
	{
		return nullptr;
	}
	FeatureGenerator* gen = &s_FeatureGenerators->at(index);
	if (gen == nullptr)
	{
		return nullptr;
	}
	if (map == nullptr)
	{
		map = Map::GetMap();
	}
	BaseGameEntity* feature = gen->SpawnFeature(node);
	feature->SetPosition(position, map);
	Feature* featureFeature = dynamic_cast<Feature*>(feature);
	featureFeature->TriggerPlacementActions(map);

	return nullptr;
}