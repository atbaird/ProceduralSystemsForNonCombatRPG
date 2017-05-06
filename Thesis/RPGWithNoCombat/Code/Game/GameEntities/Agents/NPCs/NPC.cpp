#include "Game/GameEntities/Agents/NPCs/NPC.hpp"
#include "Game/GameEntities/Agents/NPCs/NPCBehavior.hpp"
#include "Engine/Math/MathToStringUtils.hpp"
#include "Game/Common/GameDebuggingCommons.hpp"

//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
//Constructors

NPC::NPC(const SpriteResource* resource)
	: Agent(NPC_AGENTTYPE, resource)
{

}

NPC::~NPC()
{
	Agent::~Agent();

	for (size_t idx = 0; idx < m_NPCBehaviors.size(); idx++)
	{
		NPCBehavior* bev = m_NPCBehaviors[idx];
		if (bev != nullptr)
		{
			delete bev;
		}
	}
	m_NPCBehaviors.clear();
}

//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
//Updates
void NPC::Update(const float& dt)
{
	int highestUtilIdx = -1;
	float highestUtil = -1;
	for (size_t idx = 0; idx < m_NPCBehaviors.size(); idx++)
	{
		NPCBehavior* bev = m_NPCBehaviors[idx];
		if (bev == nullptr)
		{
			continue;
		}
		float currentUtil = bev->CalcUtility();
		if (currentUtil > highestUtil)
		{
			highestUtil = currentUtil;
			highestUtilIdx = idx;
		}
	}

	if (highestUtilIdx == -1)
	{
		//SPIT OUT AN ERROR HERE!
		for (size_t idx = 0; idx < m_NPCBehaviorsThatNeedToBeCleanedIfNotUsed.size(); idx++)
		{
			NPCBehavior* bev = m_NPCBehaviorsThatNeedToBeCleanedIfNotUsed[idx];
			if (bev != nullptr)
			{
				bev->CleanUpOnUpdate();
			}
		}
	}
	else
	{
		NPCBehavior* bevRun = m_NPCBehaviors[highestUtilIdx];
		if (bevRun != nullptr)
		{
			bool ranThisBevLastFrame = bevRun == m_behaviorRanLastFrame;
			if (ranThisBevLastFrame == false && m_behaviorRanLastFrame != nullptr)
			{
				m_behaviorRanLastFrame->CleanUpOnUpdate();
			}
			bevRun->Run(dt, ranThisBevLastFrame);
			m_behaviorRanLastFrame = bevRun;
		}
		else
		{
			m_behaviorRanLastFrame = nullptr;
		}
	}

	Agent::Update(dt);
}


//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
//Setters
void NPC::SetQuestCurrentlyClaimedBy(const std::string& questCurrentlyClaimedBy)
{
	m_questCurrentlyClaimedBy = questCurrentlyClaimedBy;
}

void NPC::AddNPCAttribute(const NPCAttribute& attribute)
{
	std::string lowerCase = MakeLower(attribute.GetNPCAttributeName());
	for (size_t attrIdx = 0; attrIdx < m_NPCAttributes.size(); attrIdx++)
	{
		if (lowerCase == MakeLower(m_NPCAttributes[attrIdx].GetNPCAttributeName()))
		{
			return;
		}
	}
	m_NPCAttributes.push_back(NPCAttribute(lowerCase, attribute.GetSetting()));
}

void NPC::AddOrSetNPCAttribute(const NPCAttribute& attribute)
{
	std::string lowerCase = MakeLower(attribute.GetNPCAttributeName());
	for (size_t attrIdx = 0; attrIdx < m_NPCAttributes.size(); attrIdx++)
	{
		if (lowerCase == MakeLower(m_NPCAttributes[attrIdx].GetNPCAttributeName()))
		{
			m_NPCAttributes[attrIdx].SetSetting(attribute.GetSetting());
			return;
		}
	}
	m_NPCAttributes.push_back(lowerCase);
}

void NPC::AddNPCBehavior(NPCBehavior* bev)
{
	if (bev == nullptr)
	{
		return;
	}
	if (bev->GetIfHaveThenMakeEntityInteractable())
	{
		m_interactable = true;
	}

	m_NPCBehaviors.push_back(bev);
}

void NPC::SetJob(NPCJob* job)
{
	m_job = job;
	if (job == nullptr)
	{
		SetJobTitle("");
	}
	else
	{
		SetJobTitle(m_job->GetJobTitle());
	}
}

//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
//Getters
const std::string& NPC::GetQuestCurrentlyClaimedBy() const
{
	return m_questCurrentlyClaimedBy;
}

const std::string NPC::GetNPCAttributeValue(const std::string& attributeName) const
{
	std::string lowerCase = MakeLower(attributeName);
	for (size_t idx = 0; idx < m_NPCAttributes.size(); idx++)
	{
		if (m_NPCAttributes[idx].GetNPCAttributeName() == lowerCase)
		{
			return m_NPCAttributes[idx].GetSetting();
		}
	}
	return "";
}

const bool NPC::DetermineNPCHasAttribute(const std::string& attributeName) const
{
	std::string lowerCaseName = MakeLower(attributeName);
	for (size_t idx = 0; idx < m_NPCAttributes.size(); idx++)
	{
		if (MakeLower(m_NPCAttributes[idx].GetNPCAttributeName()) == lowerCaseName)
		{
			return true;
		}
	}
	return false;
}

const bool NPC::DetermineNPCHasAttributeSetting(const std::string& attributeName, 
	const std::string& attributeValue) const
{
	bool foundNPCAttribute = false;
	return DetermineNPCHasAttributeSetting(attributeName, attributeValue, foundNPCAttribute);
}

const bool NPC::DetermineNPCHasAttributeSetting(const std::string& attributeName,
	const std::string& attributeValue,
	bool& foundNPCAttribute) const
{
	foundNPCAttribute = false;
	std::string lowerCaseName = MakeLower(attributeName);
	for (size_t idx = 0; idx < m_NPCAttributes.size(); idx++)
	{
		if (MakeLower(m_NPCAttributes[idx].GetNPCAttributeName()) == lowerCaseName)
		{
			foundNPCAttribute = true;
			std::string lowerCaseAttributeValue = MakeLower(attributeValue);
			if (lowerCaseAttributeValue == MakeLower(m_NPCAttributes[idx].GetSetting()))
			{
				return true;
			}

			return false;
		}
	}
	return false;
}

const std::vector<NPCAttribute>* NPC::GetNPCAttributes() const
{
	return &m_NPCAttributes;
}

const std::vector<NPCBehavior*>* NPC::GetNPCBehaviors() const
{
	return &m_NPCBehaviors;
}

const NPCJob* NPC::GetJob() const
{
	return m_job;
}

NPCJob* NPC::GetEditableJob()
{
	return m_job;
}

//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
//Operations
void NPC::WriteDeveloperDebugInfo(DeveloperDebugInfoBox& info) const
{
	DeveloperDebugInfoTextBlurb whatAmIBlurb = DeveloperDebugInfoTextBlurb("Entity type: AGENT, NPC",
		Rgba::s_White);
	info.AddTextBlurb(whatAmIBlurb);
	Agent::WriteDeveloperDebugInfo(info);
}

void NPC::WriteOutEntityImportantDataToStr(std::string& str, int currentIndentationLevel) const
{
	Agent::WriteOutEntityImportantDataToStr(str, currentIndentationLevel);
	currentIndentationLevel += 3;
	GameDebuggingCommons::AddIndentation(str, currentIndentationLevel);
	str += "Quest first claimed by: " + m_questCurrentlyClaimedBy + "\n";

	int nextLevelUp = currentIndentationLevel + 3;
	GameDebuggingCommons::AddIndentation(str, currentIndentationLevel);
	str += "NPC Attributes: ";
	if (m_NPCAttributes.size() == 0)
	{
		str += "NONE\n";
	}
	else
	{
		str += "\n";
		GameDebuggingCommons::AddIndentation(str, currentIndentationLevel);
		str += "Number of attributes = " + std::to_string(m_NPCAttributes.size()) + "\n";
		for (size_t i = 0; i < m_NPCAttributes.size(); i++)
		{
			const NPCAttribute& attr = m_NPCAttributes[i];
			attr.WriteOutEntityImportantDataToStr(str, nextLevelUp);
		}
	}

	GameDebuggingCommons::AddIndentation(str, currentIndentationLevel);
	str += "NPC Behaviors: ";
	if (m_NPCBehaviors.size() == 0)
	{
		str += "NONE\n";
	}
	else
	{
		str += std::to_string(m_NPCBehaviors.size()) + "\n";
		for (size_t i = 0; i < m_NPCBehaviors.size(); i++)
		{
			NPCBehavior* bev = m_NPCBehaviors[i];
			if (bev != nullptr)
			{
				bev->WriteNPCBehaviorToString(str, nextLevelUp);
			}
			else
			{
				GameDebuggingCommons::AddIndentation(str, nextLevelUp);
				str += "NULL\n";
			}
		}
	}

	GameDebuggingCommons::AddIndentation(str, currentIndentationLevel);
	str += "Behavior ran last frame: ";
	if (m_behaviorRanLastFrame == nullptr)
	{
		str += "NULL\n";
	}
	else
	{
		str += m_behaviorRanLastFrame->GetName() + "\n";
	}

	GameDebuggingCommons::AddIndentation(str, currentIndentationLevel);
	str += "NPC Job: ";
	if (m_job == nullptr)
	{
		str += "JOBLESS\n";
	}
	else
	{
		str += m_job->GetJobTitle() + "\n";
	}
}