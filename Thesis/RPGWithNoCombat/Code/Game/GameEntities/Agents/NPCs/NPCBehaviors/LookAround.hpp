#pragma once

#ifndef LOOKAROUND_HPP
#define LOOKAROUND_HPP
#include "Game/GameEntities/Agents/NPCs/NPCBehavior.hpp"

//Spawner Functions
static NPCBehavior* DefaultCreateLookAroundBehavior();
static NPCBehavior* CreateLookAroundBehavior(const XMLNode& node);

class Vector2;
class LookAround : public NPCBehavior
{
	//Static Variables
	static const NPCBehaviorRegistrationHelper s_LookAroundBehaviorRegistration;
	static const float s_DefaultUtilityScore;

	//Variables
	float m_minSecondsBeforeChangeMovementDirection = 0.5f;
	float m_additionalSecondsBeforeChangeDir = 0.3f;
	float m_secondsLeftBeforeChangeDirections = 0.f;

public:
	//Constructors
	LookAround();
	LookAround(const XMLNode& node);
	LookAround(const LookAround& other);
	virtual ~LookAround();

	//Setters
	void SetMinimumSecondsBeforeChangeMovementDirection(float seconds);
	void SetMaximumSecondsBeforeChangeMovementDirection(float seconds);
	void SetAdditionalSecondsBeforeChangeMovementDirection(float seconds);
	void SetSecondsBeforeChangeMovementDirection(const Vector2& timeRange);
	void ChangeDirections();

	//Getters
	float GetMinimumSecondsBeforeChangeMovementDirection() const;
	float GetAdditionalSecondsBeforeChangeDirection() const;
	float GetMaximumSecondsBeforeChangeMovementDirection() const;
	float GetSecondsLeftBeforeChangeDirections() const;

	//Operations
	virtual NPCBehavior* Clone() override;
	virtual const float CalcUtility() override;
	virtual void CleanUpOnUpdate() override;
	virtual void Run(float dt, bool ranLastFrame) override;
	virtual int WriteNPCBehaviorToString(std::string& str, int indentationLvl) const override;
};
#endif