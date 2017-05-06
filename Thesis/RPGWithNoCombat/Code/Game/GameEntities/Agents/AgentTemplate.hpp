#pragma once

#ifndef AGENTTEMPLATE_HPP
#define AGENTTEMPLATE_HPP
#include "Game/GameEntities/Agents/AgentTypes.hpp"
#include "Engine/2D/SpriteResource.hpp"
#include "Engine/2D/SpriteAnimationSequence.hpp"
#include "Game/Enums/Directions4.hpp"
#include "Game/Enums/AnimationsIdxes.hpp"
#include "Game/GameEntities/Agents/AgentStats/AgentStat.hpp"
#include "Game/GameEntities/Agents/AgentGeneratorDialogueSegmentChoice.hpp"
#include "Game/GameEntities/Agents/AgentTemplateNPCAttributesContainer.hpp"

class NPC;
class NPCBehavior;
class BaseGameEntity;
class Player;
class Agent;
class NPCJob;
struct XMLNode;
struct MapAgentSpecifications;

#define MAX_AGENT_NAME_LENGTH 200

class AgentTemplate
{
private:
protected:
	//-------------------------------------------------------------------------------------------------------
	//Static Variables
	static std::vector<std::string>* s_DefaultHairs;

	//-------------------------------------------------------------------------------------------------------
	//Variables
	std::vector<AgentTemplateNPCAttributesContainer> m_templateNPCAttributes;
	std::vector<AgentGeneratorDialogueSegmentChoice> m_defaultDialogueSegmentChoices;
	std::vector<AgentStat> m_stats;
	std::vector<std::string> m_AttributeNamesToLoad;
	std::vector<std::string> m_maleHair;
	std::vector<std::string> m_femaleHair;
	std::vector<std::string> m_NameGeneratorNamesToUse;
	std::vector<std::string> m_JobTitlesToUse;
	std::vector<std::string> m_HairColorChoiceLimits;
	std::vector<std::string> m_ClothingNames;
	std::vector<std::vector<SpriteAnimationSequence*>> m_sequences;
	std::vector<std::vector<std::string>> m_SequenceNames;
	std::vector<NPCBehavior*> m_NPCBehaviorsIfNPC;
	std::vector<int> m_skinColors;
	std::string m_AgentNameSetToUse = "";
	std::string m_DefaultSpriteName = "";
	float m_percentageMale = 0.5f;
	float m_movementSpeed = 1.f;
	float m_sprintingMovementSpeed = 2.f;
	float m_npcMinimumMovementSpeed = 0.5f;
	float m_npcMaximumMovementSpeed = 1.5f;
	const SpriteResource* m_defaultImage = nullptr;
	std::string m_AgentName = "";
	bool m_percentageMaleSetOnce = false;
public:
	//-------------------------------------------------------------------------------------------------------
	//Constructors
	AgentTemplate();
	AgentTemplate(const AgentTemplate& other);
	virtual ~AgentTemplate();

	//-------------------------------------------------------------------------------------------------------
	//Setters
	void SetDefaultImage(const SpriteResource* res, const std::string& spriteName = "");
	void SetPercentageMale(float percentage);
	void AddTemplateNPCAttributesContainer(AgentTemplateNPCAttributesContainer& templa);
	void SetAgentNameSetToUse(const std::string& nameSet);
	void AddNPCAttributeName(const std::string& attributeName);
	void AddSkinColor(int skinColor);
	void AddHairColorChoiceLimit(const std::string& limit);
	void AddNameGeneratorName(const std::string& name);
	void AddMaleHairName(const std::string& name);
	void AddFemaleHairName(const std::string& name);
	void AddClothingName(const std::string& name);
	void SetAgentTemplateName(const char* name);
	void SetMovementSpeed(float movementSpeed);
	void SetSprintingMovementSpeed(float movementSpeed);
	void SetNPCMinimumMovementSpeed(float movementSpeed);
	void SetNPCMaximumMovementSpeed(float movementSpeed);
	void AddSpriteAnimationSequence(SpriteAnimationSequence* sequence, Directions4 dir, int idx,
		const std::string& str = "");
	void RegisterNPCBehavior(NPCBehavior* bev);
	void ReadXMLNodeChildrenForStatRangeSettings(const XMLNode& node);
	void AddJobTitle(const std::string& jobTitle);
	void AddDialogueSegmentChoice(const AgentGeneratorDialogueSegmentChoice& dialogueChoice);
protected:
	void ClearAgentName();
public:

	//-------------------------------------------------------------------------------------------------------
	//Getters
protected:
	void WriteOutStringsToString(const std::vector<std::string>& strings, std::string& str, int indentation = 0) const;
public:
	void WriteAgentGeneratorToString(std::string& str, int indentation = 0) const;
	const SpriteResource* GetDefaultImage() const { return m_defaultImage; };
	const char* GetAgentName() const { return m_AgentName.c_str(); };

	//-------------------------------------------------------------------------------------------------------
	//Copy Methods
	void CopyVariablesOffOtherTemplate(const AgentTemplate& other);
protected:
	static void CopyStringsOffOtherTemplateStrings(std::vector<std::string>& myHairNames,
		const std::vector<std::string>& otherHairNames);
	static void CopyIntsOffOtherTemplateInts(std::vector<int>& myInts, const std::vector<int>& otherInts);
	static void CopyNPCBehaviorsOffOtherTemplateBehaviors(std::vector<NPCBehavior*>& NPCBehaviorsIfNPC,
		const std::vector<NPCBehavior*>& other_NPCBehaviorsIfNPC);
	static void CopyAgentStatsOffOtherAgentStats(std::vector<AgentStat>& agentStats,
		const std::vector<AgentStat>& other_agentStats);
	static void CopyAnimationsOffOtherAnimations(std::vector<std::vector<SpriteAnimationSequence*>>& myAnimations,
		const std::vector<std::vector<SpriteAnimationSequence*>>& otherAnimations,
		std::vector<std::vector<std::string>>& myAnimationsNames,
		const std::vector<std::vector<std::string>>& otherAnimationsNames);
public:

	//-------------------------------------------------------------------------------------------------------
	//Copy onto Agent Methods
protected:
	void CopyVariablesToAgentThatDontRequireSpecifications(BaseGameEntity* entity) const;
	void CopyVariablesToAgentStats(Agent* agent) const;
	void CopyVariablesToAgentStats(Agent* agent, const MapAgentSpecifications& specifications) const;
public:
	void CopyVariablesToAgent(BaseGameEntity* entity) const;
	void CopyVariablesToAgent(BaseGameEntity* entity, const MapAgentSpecifications& specifications) const;
protected:
	void OverwriteStatIfNotNegOne(int& statToSet, int previouslyDefinedStat, int overwrittingStat) const;
	
	void CheckAllListedJobsAgainstStats(const MapAgentSpecifications& specifications, 
		std::vector<AgentStat>& stats) const;
	void CheckRequiredJobStatAgainstLowestHighestStat(int& lowestStat, int& highestStat, int currentStat, 
		NPCJob*& lowestJob,	NPCJob*& highestJob, NPCJob* job) const;
	void AgentTemplate::SetMinMaxStatBasedOffLowestHighestJobStats(std::vector<AgentStat>& stats,
		NPCJob* lowestJob, NPCJob* highestJob) const;
	void SetMinMaxStatBasedOnLowestAndHighestStat(int& minStat, int& maxStat, 
		int& lowestStat, int& highestStat) const;

public:
	void CopyVariablesUniqueToAgentOfType(BaseGameEntity* entity, AgentTypes type) const;
	void CopyVariablesUniqueToAgentOfType(BaseGameEntity* entity, AgentTypes type,
		const MapAgentSpecifications& specifications) const;
protected:
	void CopyUniqueVariablesToPlayer(Player* player) const;
	void CopyUniqueVariablesToPlayer(Player* player, const MapAgentSpecifications& specifications) const;
	void CopyUniqueVariablesToNPCCommon(NPC* npc) const;
	void CopyUniqueVariablesToNPC(NPC* npc) const;
	void CopyUniqueVariablesToNPC(NPC* npc, const MapAgentSpecifications& specifications) const;

public:

	//-------------------------------------------------------------------------------------------------------
	//Static Variables
	static void AddDefaultHair(const std::string& defaultHairName);
	static void ClearDefaultHairs();
};
#endif