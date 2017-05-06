#include "Game/GameEntities/Agents/AgentTemplateNPCAttributesContainer.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Game/GameEntities/Agents/NPCs/NPCAttribute.hpp"
#include "Game/GameEntities/Agents/NPCs/NPCAttributes/NPCAttributeContainer.hpp"
#include "Engine/Math/ExtendedMath.hpp"
#include "Game/Common/GameDebuggingCommons.hpp"

//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
//Constructors
AgentTemplateNPCAttributesContainer::AgentTemplateNPCAttributesContainer(const std::string& NPCAttributeToCall)
{
	SetNPCAttributeToCall(NPCAttributeToCall);
}

AgentTemplateNPCAttributesContainer::AgentTemplateNPCAttributesContainer(const std::string& NPCAttributeToCall,
	const std::vector<std::string>& whiteList, const std::vector<std::string>& blackList, 
	bool useWhiteList, bool useBlackList)
	: m_WhiteList(whiteList),
	m_BlackList(blackList),
	m_useWhiteList(useWhiteList),
	m_useBlackList(useBlackList)
{
	SetNPCAttributeToCall(NPCAttributeToCall);
}

AgentTemplateNPCAttributesContainer::~AgentTemplateNPCAttributesContainer()
{

}

//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
//Operations
NPCAttribute AgentTemplateNPCAttributesContainer::GetRandomNPCAttributeBasedOffRequirements() const
{
	NPCAttribute attribute = NPCAttribute::s_EmptyAttribute;

	bool found = false;
	NPCAttributeContainer* attributeContainer
		= NPCAttributeContainer::GetNPCAttributeContainerByName(m_NPCAttributeToCall, found);
	if (attributeContainer == nullptr
		|| found == false)
	{
		return attribute;
	}
	std::vector<std::string> possibilities = attributeContainer->GetPossibleSettings();

	if (m_useBlackList)
	{
		for (size_t possibilityIdx = 0; possibilityIdx < possibilities.size(); possibilityIdx++)
		{
			for (size_t blacklistIdx = 0; blacklistIdx < m_BlackList.size(); blacklistIdx++)
			{
				if (possibilities[possibilityIdx] == m_BlackList[blacklistIdx])
				{
					possibilities.erase(possibilities.begin() + possibilityIdx);
					possibilityIdx--;
					break;
				}
			}
		}
	}
	if (m_useWhiteList)
	{
		for (size_t possibilityIdx = 0; possibilityIdx < possibilities.size(); possibilityIdx++)
		{
			bool foundTest = false;
			for (size_t whitelistIdx = 0; whitelistIdx < m_WhiteList.size(); whitelistIdx++)
			{
				if (possibilities[possibilityIdx] == m_WhiteList[whitelistIdx])
				{
					foundTest = true;
					break;
				}
			}
			if (!foundTest)
			{
				possibilities.erase(possibilities.begin() + possibilityIdx);
				possibilityIdx--;
			}
		}
	}
	if (possibilities.size() == 0)
	{
		return attribute;
	}
	size_t randPossibilityIdx = GetRandomIntLessThan(possibilities.size());

	attribute.SetNPCAttributeName(m_NPCAttributeToCall);
	attribute.SetSetting(possibilities[randPossibilityIdx]);


	return attribute;
}


NPCAttribute AgentTemplateNPCAttributesContainer::GetRandomNPCAttributeBasedOffRequirementsWithRespectToOther(
	const AgentTemplateNPCAttributesContainer& other) const
{
	if (m_NPCAttributeToCall != other.m_NPCAttributeToCall)
	{
		return NPCAttribute::s_EmptyAttribute;
	}
	//Combine lists
	std::vector<std::string> blackList;
	std::vector<std::string> whiteList;
	blackList.reserve(m_BlackList.size() + other.m_BlackList.size());
	whiteList.reserve(m_WhiteList.size() + other.m_WhiteList.size());
	bool useWhiteList = m_useWhiteList || other.m_useWhiteList;
	bool useBlackList = m_useBlackList || other.m_useBlackList;
	blackList.insert(blackList.end(), m_BlackList.begin(), m_BlackList.end());
	whiteList.insert(whiteList.end(), m_WhiteList.begin(), m_WhiteList.end());
	blackList.insert(blackList.end(), other.m_BlackList.begin(), other.m_BlackList.end());
	whiteList.insert(whiteList.end(), other.m_WhiteList.begin(), other.m_WhiteList.end());

	//Find attribute to call
	NPCAttribute attribute = NPCAttribute::s_EmptyAttribute;
	bool found = false;
	NPCAttributeContainer* attributeContainer
		= NPCAttributeContainer::GetNPCAttributeContainerByName(m_NPCAttributeToCall, found);
	if (attributeContainer == nullptr
		|| found == false)
	{
		return attribute;
	}

	//Parse possibilities
	std::vector<std::string> possibilities = attributeContainer->GetPossibleSettings();

	if (useBlackList)
	{
		for (size_t possibilityIdx = 0; possibilityIdx < possibilities.size(); possibilityIdx++)
		{
			for (size_t blacklistIdx = 0; blacklistIdx < blackList.size(); blacklistIdx++)
			{
				if (possibilities[possibilityIdx] == blackList[blacklistIdx])
				{
					possibilities.erase(possibilities.begin() + possibilityIdx);
					possibilityIdx--;
					break;
				}
			}
		}
	}
	if (useWhiteList)
	{
		for (size_t possibilityIdx = 0; possibilityIdx < possibilities.size(); possibilityIdx++)
		{
			bool foundTest = false;
			for (size_t whitelistIdx = 0; whitelistIdx < whiteList.size(); whitelistIdx++)
			{
				if (possibilities[possibilityIdx] == whiteList[whitelistIdx])
				{
					foundTest = true;
					break;
				}
			}
			if (!foundTest)
			{
				possibilities.erase(possibilities.begin() + possibilityIdx);
				possibilityIdx--;
			}
		}
	}

	//Get NPCAttribute.
	if (possibilities.size() == 0)
	{
		return attribute;
	}
	size_t randPossibilityIdx = GetRandomIntLessThan(possibilities.size());

	attribute.SetNPCAttributeName(m_NPCAttributeToCall);
	attribute.SetSetting(possibilities[randPossibilityIdx]);


	return attribute;
}

bool AgentTemplateNPCAttributesContainer::GetSingleAttributeValueMeetsAllRequirements(
	const NPCAttribute& attribute) const
{
	if (m_NPCAttributeToCall != attribute.GetNPCAttributeName())
	{
		return false;
	}

	for (size_t idx = 0; idx < m_BlackList.size(); idx++)
	{
		if (attribute.GetSetting() == m_BlackList[idx])
		{
			return false;
		}
	}

	if (m_WhiteList.size() != 0)
	{
		bool foundIt = false;
		for (size_t idx = 0; idx < m_WhiteList.size(); idx++)
		{
			if (m_WhiteList[idx] == attribute.GetSetting())
			{
				foundIt = true;
				break;
			}
		}
		if (!foundIt)
		{
			return false;
		}
	}
	return true;
}

//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
//Setters
void AgentTemplateNPCAttributesContainer::SetNPCAttributeToCall(const std::string& NPCAttributeToCall)
{
	m_NPCAttributeToCall = MakeLower(NPCAttributeToCall);
}

void AgentTemplateNPCAttributesContainer::SetWhiteList(const std::vector<std::string>& whiteList)
{
	m_WhiteList.clear();
	if (whiteList.size() == 0)
	{
		return;
	}
	m_WhiteList.reserve(whiteList.size());
	for (size_t idx = 0; idx < whiteList.size(); idx++)
	{
		AddToWhiteList(whiteList[idx]);
	}
}

void AgentTemplateNPCAttributesContainer::SetBlackList(const std::vector<std::string>& blackList)
{
	m_BlackList.clear();
	if (blackList.size() == 0)
	{
		return;
	}
	m_BlackList.reserve(blackList.size());
	for (size_t idx = 0; idx < blackList.size(); idx++)
	{
		AddToBlackList(blackList[idx]);
	}
}

void AgentTemplateNPCAttributesContainer::AddAllToWhiteList(const std::vector<std::string>& whiteList)
{
	if (whiteList.size() == 0)
	{
		return;
	}
	m_WhiteList.reserve(whiteList.size());
	for (size_t idx = 0; idx < whiteList.size(); idx++)
	{
		AddToWhiteList(whiteList[idx]);
	}
}

void AgentTemplateNPCAttributesContainer::AddAllToBlackList(const std::vector<std::string>& blackList)
{
	if (blackList.size() == 0)
	{
		return;
	}
	m_BlackList.reserve(blackList.size());
	for (size_t idx = 0; idx < blackList.size(); idx++)
	{
		AddToBlackList(blackList[idx]);
	}
}

void AgentTemplateNPCAttributesContainer::AddToWhiteList(const std::string& addToWhiteList)
{
	std::string lowerCase = MakeLower(addToWhiteList);
	for (size_t idx = 0; idx < m_WhiteList.size(); idx++)
	{
		if (lowerCase == m_WhiteList[idx])
		{
			return;
		}
	}
	m_WhiteList.push_back(lowerCase);
}

void AgentTemplateNPCAttributesContainer::AddToBlackList(const std::string& addToBlackList)
{
	std::string lowerCase = MakeLower(addToBlackList);
	for (size_t idx = 0; idx < m_BlackList.size(); idx++)
	{
		if (lowerCase == m_BlackList[idx])
		{
			return;
		}
	}
	m_BlackList.push_back(lowerCase);
}

void AgentTemplateNPCAttributesContainer::SetUseWhiteList(bool useWhiteList)
{
	m_useWhiteList = useWhiteList;
}

void AgentTemplateNPCAttributesContainer::SetUseBlackList(bool useBlackList)
{
	m_useBlackList = useBlackList;
}

//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
//Getters
void AgentTemplateNPCAttributesContainer::WriteNPCAttributeContainerOutToString(std::string& str, 
	int currentIndentation) const
{
	GameDebuggingCommons::AddIndentation(str, currentIndentation);
	str += "NPC Attribute Name: " + m_NPCAttributeToCall + "\n";

	currentIndentation += 3;
	int nextTierIndentation = currentIndentation + 3;
	if (m_WhiteList.size() > 0)
	{
		GameDebuggingCommons::AddIndentation(str, currentIndentation);
		str += "NPC Attribute Whitelist:\n";
		for (size_t i = 0; i < m_WhiteList.size(); i++)
		{
			GameDebuggingCommons::AddIndentation(str, nextTierIndentation);
			str += m_WhiteList[i] + "\n";
		}
	}

	if (m_BlackList.size() > 0)
	{
		GameDebuggingCommons::AddIndentation(str, currentIndentation);
		str += "NPC Attribute Blacklist:\n";
		for (size_t i = 0; i < m_WhiteList.size(); i++)
		{
			GameDebuggingCommons::AddIndentation(str, nextTierIndentation);
			str += m_BlackList[i] + "\n";
		}
	}
}

const std::string AgentTemplateNPCAttributesContainer::GetNPCAttributeToCall() const
{
	return m_NPCAttributeToCall;
}

bool AgentTemplateNPCAttributesContainer::GetUseWhiteList() const
{
	return m_useWhiteList;
}

bool AgentTemplateNPCAttributesContainer::GetUseBlackList() const
{
	return m_useBlackList;
}

size_t AgentTemplateNPCAttributesContainer::GetSizeOfWhiteList() const
{
	return m_WhiteList.size();
}

size_t AgentTemplateNPCAttributesContainer::GetSizeOfBlackList() const
{
	return m_BlackList.size();
}

std::string AgentTemplateNPCAttributesContainer::GetWhiteListValueAtIndex(size_t index) const
{
	if (index >= m_WhiteList.size())
	{
		return "";
	}
	return m_WhiteList[index];
}

std::string AgentTemplateNPCAttributesContainer::GetBlackListValueAtIndex(size_t index) const
{
	if (index >= m_BlackList.size())
	{
		return "";
	}
	return m_BlackList[index];
}

std::string AgentTemplateNPCAttributesContainer::GetWhiteListValueAtIndex(int index) const
{
	if (index < 0 || (size_t)index >= m_WhiteList.size())
	{
		return "";
	}
	return m_WhiteList[index];
}

std::string AgentTemplateNPCAttributesContainer::GetBlackListValueAtIndex(int index) const
{
	if (index < 0 || (size_t)index >= m_BlackList.size())
	{
		return "";
	}
	return m_BlackList[index];
}

const std::vector<std::string>* AgentTemplateNPCAttributesContainer::GetWhiteList() const
{
	return &m_WhiteList;
}

const std::vector<std::string>* AgentTemplateNPCAttributesContainer::GetBlackList() const
{
	return &m_BlackList;
}

std::vector<std::string>* AgentTemplateNPCAttributesContainer::GetEditableWhiteList()
{
	return &m_WhiteList;
}

std::vector<std::string>* AgentTemplateNPCAttributesContainer::GetEditableBlackList()
{
	return &m_BlackList;
}

const std::vector<std::string> AgentTemplateNPCAttributesContainer::GetWhiteListCopy() const
{
	return m_WhiteList;
}

const std::vector<std::string> AgentTemplateNPCAttributesContainer::GetBlackListCopy() const
{
	return m_BlackList;
}