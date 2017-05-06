#pragma once

#ifndef AGENTTEMPLATENPCATTRIBUTESCONTAINER_HPP
#define AGENTTEMPLATENPCATTRIBUTESCONTAINER_HPP
#include <string>
#include <vector>

class NPCAttribute;
struct AgentTemplateNPCAttributesContainer
{
private:
protected:
	std::string m_NPCAttributeToCall = "";
	std::vector<std::string> m_WhiteList;
	std::vector<std::string> m_BlackList;
	bool m_useWhiteList = false;
	bool m_useBlackList = false;
public:
	//Constructors
	AgentTemplateNPCAttributesContainer(const std::string& NPCAttributeToCall = "");
	AgentTemplateNPCAttributesContainer(const std::string& NPCAttributeToCall,
		const std::vector<std::string>& whiteList, const std::vector<std::string>& blackList,
		bool useWhiteList, bool useBlackList);
	virtual ~AgentTemplateNPCAttributesContainer();

	//Operations
	NPCAttribute GetRandomNPCAttributeBasedOffRequirements() const;
	NPCAttribute GetRandomNPCAttributeBasedOffRequirementsWithRespectToOther(
		const AgentTemplateNPCAttributesContainer& other) const;
	bool GetSingleAttributeValueMeetsAllRequirements(const NPCAttribute& attribute) const;

	//Setters
	void SetNPCAttributeToCall(const std::string& NPCAttributeToCall);
	void SetWhiteList(const std::vector<std::string>& whiteList);
	void SetBlackList(const std::vector<std::string>& blackList);
	void AddAllToWhiteList(const std::vector<std::string>& whiteList);
	void AddAllToBlackList(const std::vector<std::string>& blackList);
	void AddToWhiteList(const std::string& addToWhiteList);
	void AddToBlackList(const std::string& addToBlackList);
	void SetUseWhiteList(bool useWhiteList);
	void SetUseBlackList(bool useBlackList);

	//Getters
	void WriteNPCAttributeContainerOutToString(std::string& str, int currentIndentation) const;
	const std::string GetNPCAttributeToCall() const;
	bool GetUseWhiteList() const;
	bool GetUseBlackList() const;
	size_t GetSizeOfWhiteList() const;
	size_t GetSizeOfBlackList() const;
	std::string GetWhiteListValueAtIndex(size_t index) const;
	std::string GetBlackListValueAtIndex(size_t index) const;
	std::string GetWhiteListValueAtIndex(int index) const;
	std::string GetBlackListValueAtIndex(int index) const;
	const std::vector<std::string>* GetWhiteList() const;
	const std::vector<std::string>* GetBlackList() const;
	std::vector<std::string>* GetEditableWhiteList();
	std::vector<std::string>* GetEditableBlackList();
	const std::vector<std::string> GetWhiteListCopy() const;
	const std::vector<std::string> GetBlackListCopy() const;
};

#endif