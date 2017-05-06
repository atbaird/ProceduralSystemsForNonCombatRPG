#pragma once

#ifndef WANDER_HPP
#define WANDER_HPP
#include "Game/GameEntities/Agents/NPCs/NPCBehavior.hpp"

//Spawner Functions
static NPCBehavior* DefaultCreateWanderBehavior();
static NPCBehavior* CreateWanderBehavior(const XMLNode& node);

class Vector2;
class Wander : public NPCBehavior
{
private:
protected:
	//Static Variables
	static const NPCBehaviorRegistrationHelper s_WanderBehaviorRegistration;
	static constexpr float s_DefaultUtilityScore = 5.f;

	//Variables
	float m_minSecondsBeforeChangeMovementDirection = 1.f;
	float m_additionalSecondsBeforeChangeDir = 0.5f;
	float m_secondsLeftBeforeChangeDirections = 0.f;
public:
	//Constructors
	Wander();
	Wander(const XMLNode& node);
	Wander(const Wander& other);
	virtual ~Wander();

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
	virtual void Run(float dt, bool ranLastFrame) override;
	virtual int WriteNPCBehaviorToString(std::string& str, int indentationLvl) const override;
};

#endif