#include "Game/Dialogue/DialogueRequirements/Inheritors/DialogueRequirementNPCHasJob.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Game/GameEntities/Agents/NPCs/NPC.hpp"
#include "Game/GameEntities/Agents/Player/Player.hpp"
#include "Game/Common/GameDebuggingCommons.hpp"

//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//Spawning Functions
DialogueRequirement* DialogueReqNPCHasJobCreationFunc(const XMLNode& node)
{
	return new DialogueRequirementNPCHasJob(node);
}

DialogueRequirement* DialogueReqNPCHasJobDefaultCreationFunc()
{
	return new DialogueRequirementNPCHasJob();
}

//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//Static Variables
STATIC const DialogueRequirementRegistrationHelper
	DialogueRequirementNPCHasJob::s_NPCHasAttributeValueRegHelper
		= DialogueRequirementRegistrationHelper("NPCHasJob", 
			DialogueReqNPCHasJobCreationFunc,
			DialogueReqNPCHasJobDefaultCreationFunc);

//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//Constructors
DialogueRequirementNPCHasJob::DialogueRequirementNPCHasJob()
	: DialogueRequirement()
{

}

DialogueRequirementNPCHasJob::DialogueRequirementNPCHasJob(const XMLNode& node)
	: DialogueRequirement(node)
{
	for (int attrIdx = 0; attrIdx < node.nAttribute(); attrIdx++)
	{
		XMLAttribute attr = node.getAttribute(attrIdx);
		std::string attrName = MakeLower(attr.lpszName);
		if (attrName == "jobtitle"
			|| attrName == "title"
			|| attrName == "job"
			|| attrName == "jobname"
			|| attrName == "name")
		{
			SetNPCJobTitle(attr.lpszValue);
		}
	}
}

DialogueRequirementNPCHasJob::DialogueRequirementNPCHasJob(const DialogueRequirementNPCHasJob& other)
	: DialogueRequirement(other),
	m_NPCJobTitle(other.m_NPCJobTitle)
{

}

DialogueRequirementNPCHasJob::~DialogueRequirementNPCHasJob()
{
	DialogueRequirement::~DialogueRequirement();
}

//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//Setters
void DialogueRequirementNPCHasJob::SetNPCJobTitle(const std::string& jobTitle)
{
	m_NPCJobTitle = MakeLower(jobTitle);
}

//Getters
const std::string& DialogueRequirementNPCHasJob::GetNPCJobTitle() const
{
	return m_NPCJobTitle;
}

//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//Operations
void DialogueRequirementNPCHasJob::WriteDialogueTriggerToString(std::string& str, int indentationAmt) const
{
	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Dialogue Requirement Name: NPC Has Job\n";
	
	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "NPC Job Title: " + m_NPCJobTitle + "\n";
}

bool DialogueRequirementNPCHasJob::CheckIfRequirementIsMet() const
{
	const BaseGameEntity* entityPly = BaseGameEntity::GetPointerToPlayer();
	if (entityPly == nullptr)
	{
		return false;
	}
	const Player* player = dynamic_cast<const Player*>(entityPly);
	if (player == nullptr)
	{
		return false;
	}
	Agent* agent = player->GetTalkingTo();
	if (agent == nullptr)
	{
		return false;
	}
	NPC* npc = dynamic_cast<NPC*>(agent);
	if (npc == nullptr)
	{
		return false;
	}
	std::string jobTitle = MakeLower(npc->GetJobTitle());
	if (jobTitle == m_NPCJobTitle)
	{
		return true;
	}
	return false;
}

DialogueRequirement* DialogueRequirementNPCHasJob::Clone() const
{
	return new DialogueRequirementNPCHasJob(*this);
}