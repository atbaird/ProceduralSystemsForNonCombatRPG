#include "Game/GameEntities/Agents/NPCs/NPCAttributes/NPCAttributeContainer.hpp"
#include "Engine/Core/EngineXMLParser.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Engine/Core/FileUtils.hpp"
#include "Engine/Math/ExtendedMath.hpp"
#include "Game/TheGame.hpp"
#include "Game/GameEntities/Agents/NPCs/NPCAttribute.hpp"
#include "Engine/Core/Performance/Performance.hpp"
#include "Game/Common/GameDebuggingCommons.hpp"

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//Static Variables
STATIC NPCAttributeContainerLoadingData* NPCAttributeContainer::s_loadingData = nullptr;
STATIC std::vector<NPCAttributeContainer>* NPCAttributeContainer::s_AttributeContainers = nullptr;
STATIC NPCAttributeContainer NPCAttributeContainer::s_EmptyAttributeContainer = NPCAttributeContainer();

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//Constructors
NPCAttributeContainer::NPCAttributeContainer()
{

}

NPCAttributeContainer::NPCAttributeContainer(const XMLNode& node)
{
	SetNPCAttributeName(node.getName());
	for (int childIdx = 0; childIdx < node.nChildNode(); childIdx++)
	{
		XMLNode child = node.getChildNode(childIdx);
		std::string childName = MakeLower(child.getName());

		for (int childAttrIdx = 0; childAttrIdx < child.nAttribute(); childAttrIdx++)
		{
			XMLAttribute attr = child.getAttribute(childAttrIdx);
			std::string AttrName = MakeLower(attr.lpszName);
			if (AttrName == "name"
				|| AttrName == "value")
			{
				childName = MakeLower(attr.lpszValue);
			}
		}
		m_PossibleSettings.push_back(childName);
	}
}

NPCAttributeContainer::~NPCAttributeContainer()
{

}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//Setters
void NPCAttributeContainer::SetNPCAttributeName(const std::string& NPCAttributeName)
{
	m_NPCAttributeName = NPCAttributeName;
	m_LowerCaseNPCAttributeName = MakeLower(NPCAttributeName);
}

void NPCAttributeContainer::SetPossibleSettings(const std::vector<std::string>& settings)
{
	m_PossibleSettings = settings;
}

void NPCAttributeContainer::AddPossibleSetting(const std::string& setting)
{
	std::string lowerCase = MakeLower(setting);
	for (size_t settingIdx = 0; settingIdx < m_PossibleSettings.size(); settingIdx++)
	{
		if (lowerCase == m_PossibleSettings[settingIdx])
		{
			return;
		}
	}
	m_PossibleSettings.push_back(lowerCase);
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//Getters
const std::string& NPCAttributeContainer::GetNPCAttributeName() const
{
	return m_NPCAttributeName;
}

const std::string& NPCAttributeContainer::GetLowerCaseAttributeName() const
{
	return m_LowerCaseNPCAttributeName;
}

const std::vector<std::string>& NPCAttributeContainer::GetPossibleSettings() const
{
	return m_PossibleSettings;
}

const std::string NPCAttributeContainer::GetPossibleSettingByIdx(int index) const
{
	if (index < 0 || (size_t)index >= m_PossibleSettings.size())
	{
		return "";
	}
	return m_PossibleSettings[index];
}

const std::string NPCAttributeContainer::GetRandomPossibleSetting() const
{
	int randomVal = GetRandomIntLessThan(m_PossibleSettings.size());
	return GetPossibleSettingByIdx(randomVal);
}

void NPCAttributeContainer::WriteNPCAttributeContainerToString(std::string& str, int indentationLevel) const
{
	GameDebuggingCommons::AddIndentation(str, indentationLevel);
	str += "NPC Attribute Name: " + m_NPCAttributeName + "\n";

	indentationLevel += 3;
	int nextLevel = indentationLevel + 3;
	GameDebuggingCommons::AddIndentation(str, indentationLevel);
	str += "Possible Values: \n";
	for (size_t i = 0; i < m_PossibleSettings.size(); i++)
	{
		GameDebuggingCommons::AddIndentation(str, nextLevel);
		str += m_PossibleSettings[i] + "\n";
	}
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//Static Functions
STATIC void NPCAttributeContainer::WriteAllNPCAttributeContainersToString(std::string& str, int indentationLevel)
{
	if (s_AttributeContainers == nullptr)
	{
		return;
	}

	for (size_t i = 0; i < s_AttributeContainers->size(); i++)
	{
		const NPCAttributeContainer& container = s_AttributeContainers->at(i);
		container.WriteNPCAttributeContainerToString(str, indentationLevel);
	}
}

STATIC void NPCAttributeContainer::ReadInNPCAttributesFromCommonFolderAllAtOnce()
{
	std::string commonNPCAttributeContainerFileLocation = s_CommonNPCAttributeContainerFileLocation;
	GameSettings* settings = TheGame::GetGameSettings();
	if (settings != nullptr)
	{
		commonNPCAttributeContainerFileLocation = settings->m_commonNPCAttributeContainerFileLocation;
	}
	std::vector<std::string> files = FileUtils::EnumerateFilesInDirectory(
		commonNPCAttributeContainerFileLocation, "*", true);

	for (size_t fileIdx = 0; fileIdx < files.size(); fileIdx++)
	{
		std::string fileStr = files.at(fileIdx);
		ReadSingleNPCAttributeFromXMLFile(fileStr);
	}
}

STATIC bool NPCAttributeContainer::OnUpdateReadInNPCAttributesFromCommonFolder()
{
	if (s_loadingData == nullptr)
	{
		s_loadingData = new NPCAttributeContainerLoadingData();
	}
	GameSettings* settings = TheGame::GetGameSettings();

	if (!s_loadingData->m_loadedFiles)
	{
		s_loadingData->m_loadedFiles = true;
		if (settings != nullptr)
		{
			s_loadingData->m_amountOfTimeToTakePerFrameReadingNPCAttributes =
				settings->m_amountOfTimeLoadingNPCAttributesPerFrame;
			s_loadingData->m_commonNPCAttributeContainerFileLocation =
				settings->m_commonNPCAttributeContainerFileLocation;
		}
		s_loadingData->m_files = FileUtils::EnumerateFilesInDirectory(
			s_loadingData->m_commonNPCAttributeContainerFileLocation, "*", true);

		if (s_loadingData->m_files.size() == 0)
		{
			return true;
		}
		return false;
	}

	if (s_loadingData->m_amountOfTimeToTakePerFrameReadingNPCAttributes <= 0)
	{
		return true;
	}
	int numberReadThisFrame = 0;

	while ((size_t)s_loadingData->m_index < s_loadingData->m_files.size())
	{
		std::string fileStr = s_loadingData->m_files[s_loadingData->m_index];
		ReadSingleNPCAttributeFromXMLFile(fileStr);
		s_loadingData->m_index++;
		numberReadThisFrame++;
		uint64_t timeDiff = Performance::GetCurrentPerformanceCount() - g_TheGame->GetFrameStartTime();
		double timeDiffDouble = Performance::PerformanceCountToSeconds(timeDiff);
		if (timeDiffDouble >= s_loadingData->m_amountOfTimeToTakePerFrameReadingNPCAttributes)
		{
			return false;
		}
	}

	if ((size_t)s_loadingData->m_index >= s_loadingData->m_files.size())
	{
		return true;
	}
	return false;
}

STATIC void NPCAttributeContainer::ReadSingleNPCAttributeFromXMLFile(const std::string& fileStr)
{
	std::string last3Letters = fileStr.substr(fileStr.size() - 3, 3);
	if (SimpleStrCmpLower(last3Letters, "xml") == false)
	{
		return;
	}

	bool errorOccurred = false;
	XMLNode root = EngineXMLParser::ParseXMLFile(fileStr,
		s_CommonNPCAttributeContainerRootXMLNode, errorOccurred);
	if (errorOccurred == true)
	{
		return;
	}

	for (int childIdx = 0; childIdx < root.nChildNode(); childIdx++)
	{
		XMLNode child = root.getChildNode(childIdx);
		{
			NPCAttributeContainer attribute = NPCAttributeContainer(child);
			if (s_AttributeContainers == nullptr)
			{
				s_AttributeContainers = new std::vector<NPCAttributeContainer>();
			}
			s_AttributeContainers->push_back(attribute);
		}
	}
}

STATIC void NPCAttributeContainer::ClearNPCAttributes()
{
	if (s_loadingData != nullptr)
	{
		delete s_loadingData;
		s_loadingData = nullptr;
	}
	if (s_AttributeContainers == nullptr)
	{
		return;
	}
	delete s_AttributeContainers;
	s_AttributeContainers = nullptr;
}

STATIC NPCAttributeContainer* NPCAttributeContainer::GetNPCAttributeContainerByName(
	const std::string& attributeName, bool& found)
{
	found = false;
	if (s_AttributeContainers == nullptr)
	{
		return nullptr;
	}

	std::string lowerCase = MakeLower(attributeName);

	for (size_t idx = 0; idx < s_AttributeContainers->size(); idx++)
	{
		NPCAttributeContainer& attribute = s_AttributeContainers->at(idx);
		if (attribute.GetLowerCaseAttributeName() == lowerCase)
		{
			found = true;
			return &attribute;
		}
	}
	return nullptr;
}

STATIC NPCAttributeContainer* NPCAttributeContainer::GetNPCAttributeContainerByIndex(
	int attributeContainerIdx, bool& found)
{
	found = false;
	if (s_AttributeContainers == nullptr
		|| attributeContainerIdx < 0
		|| (size_t)attributeContainerIdx >= s_AttributeContainers->size())
	{
		return nullptr;
	}
	found = true;
	return &s_AttributeContainers->at(attributeContainerIdx);
}

STATIC const std::string NPCAttributeContainer::GetNPCAttributeValueByIndex(
	const std::string& attributeName, int attributeIdx)
{
	bool found = false;
	NPCAttributeContainer* attribute = GetNPCAttributeContainerByName(attributeName, found);
	if (!found
		|| attribute == nullptr)
	{
		return "";
	}
	return attribute->GetPossibleSettingByIdx(attributeIdx);
}

STATIC const std::string NPCAttributeContainer::GetNPCAttributeValueByIndex(
	int attributeContainerIdx, int attributeIdx)
{
	bool found = false;
	NPCAttributeContainer* attribute = GetNPCAttributeContainerByIndex(attributeContainerIdx, found);
	if (!found
		|| attribute == nullptr)
	{
		return "";
	}
	return attribute->GetPossibleSettingByIdx(attributeIdx);
}

STATIC const std::string NPCAttributeContainer::GetRandomNPCAttributeValue(
	const std::string& attributeName)
{
	bool found = false;
	NPCAttributeContainer* attribute = GetNPCAttributeContainerByName(attributeName, found);
	if (!found
		|| attribute == nullptr)
	{
		return "";
	}
	return attribute->GetRandomPossibleSetting();
}

STATIC const std::string NPCAttributeContainer::GetRandomNPCAttributeValue(
	int attributeContainerIdx)
{
	bool found = false;
	NPCAttributeContainer* attribute = GetNPCAttributeContainerByIndex(attributeContainerIdx, found);
	if (!found
		|| attribute == nullptr)
	{
		return "";
	}
	return attribute->GetRandomPossibleSetting();
}

STATIC const NPCAttribute NPCAttributeContainer::GetNPCAttributeByIndex(const std::string& attributeName, int attributeIdx)
{
	bool found = false;
	NPCAttributeContainer* attribute = GetNPCAttributeContainerByName(attributeName, found);
	if (!found
		|| attribute == nullptr)
	{
		return NPCAttribute::s_EmptyAttribute;
	}
	NPCAttribute attr = NPCAttribute(attribute->GetLowerCaseAttributeName(), 
		attribute->GetPossibleSettingByIdx(attributeIdx));
	return attr;
}

STATIC const NPCAttribute NPCAttributeContainer::GetNPCAttributeByIndex(int attributeContainerIdx, int attributeIdx)
{
	bool found = false;
	NPCAttributeContainer* attribute = GetNPCAttributeContainerByIndex(attributeContainerIdx, found);
	if (!found
		|| attribute == nullptr)
	{
		return NPCAttribute::s_EmptyAttribute;
	}
	NPCAttribute attr = NPCAttribute(attribute->GetLowerCaseAttributeName(),
		attribute->GetPossibleSettingByIdx(attributeIdx));
	return attr;
}

STATIC const NPCAttribute NPCAttributeContainer::GetRandomNPCAttribute(const std::string& attributeName)
{
	bool found = false;
	NPCAttributeContainer* attribute = GetNPCAttributeContainerByName(attributeName, found);
	if (!found
		|| attribute == nullptr)
	{
		return NPCAttribute::s_EmptyAttribute;
	}
	NPCAttribute attr = NPCAttribute(attribute->GetLowerCaseAttributeName(), attribute->GetRandomPossibleSetting());
	return attr;
}

STATIC const NPCAttribute NPCAttributeContainer::GetRandomNPCAttribute(int attributeContainerIdx)
{
	bool found = false;
	NPCAttributeContainer* attribute = GetNPCAttributeContainerByIndex(attributeContainerIdx, found);
	if (!found
		|| attribute == nullptr)
	{
		return NPCAttribute::s_EmptyAttribute;
	}
	NPCAttribute attr = NPCAttribute(attribute->GetLowerCaseAttributeName(), attribute->GetRandomPossibleSetting());
	return attr;
}