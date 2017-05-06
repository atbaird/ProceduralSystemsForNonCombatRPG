#pragma once

#ifndef NPC_HPP
#define NPC_HPP
#include "Game/GameEntities/Agents/Agent.hpp"
#include "Game/GameEntities/Agents/NPCs/NPCJobs/NPCJob.hpp"
#include "Game/GameEntities/Agents/NPCs/NPCAttribute.hpp"

class NPCBehavior;
class NPC : public Agent
{
private:
protected:
	std::string m_questCurrentlyClaimedBy = "";
	std::vector<NPCAttribute> m_NPCAttributes;
	std::vector<NPCBehavior*> m_NPCBehaviors;
	std::vector<NPCBehavior*> m_NPCBehaviorsThatNeedToBeCleanedIfNotUsed;
	NPCBehavior* m_behaviorRanLastFrame = nullptr;
	NPCJob* m_job = nullptr;
public:
	//Constructors
	NPC(const SpriteResource* resource = nullptr);
	virtual ~NPC();

	//Updates
	virtual void Update(const float& dt) override;

	//Setters
	void SetQuestCurrentlyClaimedBy(const std::string& questCurrentlyClaimedBy);
	void AddNPCAttribute(const NPCAttribute& attribute);
	void AddOrSetNPCAttribute(const NPCAttribute& attribute);
	void AddNPCBehavior(NPCBehavior* bev);
	void SetJob(NPCJob* job);

	//Getters
	const std::string& GetQuestCurrentlyClaimedBy() const;
	const std::string GetNPCAttributeValue(const std::string& attributeName) const;
	const bool DetermineNPCHasAttribute(const std::string& attributeName) const;
	const bool DetermineNPCHasAttributeSetting(const std::string& attributeName, 
		const std::string& attributeValue) const;
	const bool DetermineNPCHasAttributeSetting(const std::string& attributeName, const std::string& attributeValue,
		bool& foundNPCAttribute) const;
	const std::vector<NPCAttribute>* GetNPCAttributes() const;
	const std::vector<NPCBehavior*>* GetNPCBehaviors() const;
	const NPCJob* GetJob() const;
	NPCJob* GetEditableJob();

	//Operations
	virtual void WriteDeveloperDebugInfo(DeveloperDebugInfoBox& info) const override;
	virtual void WriteOutEntityImportantDataToStr(std::string& str, int currentIndentationLevel = 0) const override;
};
#endif