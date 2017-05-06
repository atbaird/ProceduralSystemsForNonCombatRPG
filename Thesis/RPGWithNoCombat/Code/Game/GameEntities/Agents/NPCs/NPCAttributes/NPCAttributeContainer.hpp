#pragma once

#ifndef NPCATTRIBUTECONTAINER_HPP
#define NPCATTRIBUTECONTAINER_HPP
#include <vector>
#include <string>

struct NPCAttributeContainerLoadingData
{
	std::vector<std::string> m_files;
	std::string m_commonNPCAttributeContainerFileLocation = "Data/XML/NPCAttributes/";
	int m_index = 0;
	double m_amountOfTimeToTakePerFrameReadingNPCAttributes = 0.25;
	bool m_loadedFiles = false;
};

#define STATIC
class NPCAttribute;
struct XMLNode;
class NPCAttributeContainer
{
private:
protected:
	//Static Variables
	static constexpr char* s_CommonNPCAttributeContainerFileLocation = "Data/XML/NPCAttributes/";
	static constexpr char* s_CommonNPCAttributeContainerRootXMLNode = "NPCAttributes";
	static NPCAttributeContainerLoadingData* s_loadingData;
	static std::vector<NPCAttributeContainer>* s_AttributeContainers;
	static NPCAttributeContainer s_EmptyAttributeContainer;

	//Variables
	std::string m_NPCAttributeName = "";
	std::string m_LowerCaseNPCAttributeName = "";
	std::vector<std::string> m_PossibleSettings;
public:
	//Constructors
	NPCAttributeContainer();
	NPCAttributeContainer(const XMLNode& node);
	virtual ~NPCAttributeContainer();

	//Setters
	void SetNPCAttributeName(const std::string& NPCAttributeName);
	void SetPossibleSettings(const std::vector<std::string>& settings);
	void AddPossibleSetting(const std::string& setting);

	//Getters
	const std::string& GetNPCAttributeName() const;
	const std::string& GetLowerCaseAttributeName() const;
	const std::vector<std::string>& GetPossibleSettings() const;
	const std::string GetPossibleSettingByIdx(int index) const;
	const std::string GetRandomPossibleSetting() const;
	void WriteNPCAttributeContainerToString(std::string& str, int indentationLevel = 0) const;

	//Static Functions
	static void WriteAllNPCAttributeContainersToString(std::string& str, int indentationLevel = 0);
	static void ReadInNPCAttributesFromCommonFolderAllAtOnce();
	static bool OnUpdateReadInNPCAttributesFromCommonFolder();
protected:
	static void ReadSingleNPCAttributeFromXMLFile(const std::string& fileStr);
public:
	static void ClearNPCAttributes();
	static NPCAttributeContainer* GetNPCAttributeContainerByName(const std::string& attributeName, bool& found);
	static NPCAttributeContainer* GetNPCAttributeContainerByIndex(int attributeContainerIdx, bool& found);
	static const std::string GetNPCAttributeValueByIndex(const std::string& attributeName, int attributeIdx);
	static const std::string GetNPCAttributeValueByIndex(int attributeContainerIdx, int attributeIdx);
	static const std::string GetRandomNPCAttributeValue(const std::string& attributeName);
	static const std::string GetRandomNPCAttributeValue(int attributeContainerIdx);
	static const NPCAttribute GetNPCAttributeByIndex(const std::string& attributeName, int attributeIdx);
	static const NPCAttribute GetNPCAttributeByIndex(int attributeContainerIdx, int attributeIdx);
	static const NPCAttribute GetRandomNPCAttribute(const std::string& attributeName);
	static const NPCAttribute GetRandomNPCAttribute(int attributeContainerIdx);
};

#endif