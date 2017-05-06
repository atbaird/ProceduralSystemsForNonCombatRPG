#pragma once

#ifndef TALKING_HPP
#define TALKING_HPP
#include "Game/GameEntities/Agents/NPCs/NPCBehavior.hpp"

//Creation Functions
static NPCBehavior* DefaultCreateTalkingBehavior();
static NPCBehavior* CreateTalkingBehavior(const XMLNode& node);

class Agent;
class Talking : public NPCBehavior
{
private:
protected:
	//Static Variables
	static const NPCBehaviorRegistrationHelper s_TalkingBehaviorRegistration;
	static float s_UtilityWhenNotTalking;
	static float s_UtilityWhenTalking;

	//Variables
	Agent* m_agentLastTargeted = nullptr;
	bool m_talkingModeEnabled = false;

public:
	//Constructors
	Talking();
	Talking(const XMLNode& node);
	Talking(const Talking& other);
	virtual ~Talking();

	//Setters
	void ChangeDirections();

	//Getters
	Agent* GetAgentLastTargeted() const;
	bool GetTalkingModeEnabled() const;

	//Operations
	virtual void CleanUpOnUpdate() override;
	virtual NPCBehavior* Clone() override;
	virtual const float CalcUtility() override;
	virtual void Run(float dt, bool ranLastFrame) override;
	virtual int WriteNPCBehaviorToString(std::string& str, int indentationLvl) const;
};
#endif