#pragma once

#ifndef NPCBEHAVIOR_HPP
#define NPCBEHAVIOR_HPP
#include <string>
#include "Game/GameEntities/Agents/NPCs/NPCBehaviorRegistrationHelper.hpp"

class NPC;
class NPCBehavior
{
private:
protected:
	std::string m_name;
	float m_lastRanUtilityAmtToRun = 0.f;
	NPC* m_owner = nullptr;
	bool m_canRun = false;
	bool m_finishedRunningOnce = false;
	bool m_IfHaveThenMakeEntityInteractable = false;
public:
	//Constructors
	NPCBehavior();
	NPCBehavior(const XMLNode& node);
	NPCBehavior(const NPCBehavior& other);
	virtual ~NPCBehavior();

	//Setters
	void SetName(const std::string& name, bool ensureIsLowerCase = false);
	void SetLastRanUtilityAmountToRun(float amt);
	void SetOwner(NPC* owner);
	void SetCanRun(bool run);

	//Getters
	const std::string& GetName() const;
	float GetRanUtilityAmountToRun() const;
	const NPC* GetOwner() const;
	NPC* GetEditableOwner() const;
	const bool GetIfHaveThenMakeEntityInteractable() const;

	//Operations
	const bool DoesPassChanceToRun() const;
	virtual void CleanUpOnUpdate();
	virtual int WriteNPCBehaviorToString(std::string& str, int indentationLvl) const;
	virtual NPCBehavior* Clone() = 0;
	virtual const float CalcUtility() = 0;
	virtual void Run(float dt, bool ranLastFrame) = 0;
};

#endif