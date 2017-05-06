#pragma once

#ifndef BASICPATROL_HPP
#define BASICPATROL_HPP
#include "Game/GameEntities/Agents/NPCs/NPCBehavior.hpp"

//Spawner Functions
static NPCBehavior* DefaultCreateBasicPatrolBehavior();
static NPCBehavior* CreateBasicPatrolBehavior(const XMLNode& node);

class Vector2;
class BasicPatrol : public NPCBehavior
{
private:
protected:
	//Static Variables
	static const NPCBehaviorRegistrationHelper s_BasicPatrolBehaviorRegistration;
	static const float s_DefaultUtilityScore;

	//Variables
	std::vector<Vector2> m_patrolPoints;

public:
	//Constructors
	BasicPatrol();
	BasicPatrol(const XMLNode& node);
	BasicPatrol(const BasicPatrol& other);
	virtual ~BasicPatrol();


	//Operations
	virtual NPCBehavior* Clone() override;
	virtual const float CalcUtility() override;
	virtual void Run(float dt, bool ranLastFrame) override;
	virtual int WriteNPCBehaviorToString(std::string& str, int indentationLvl) const override;
};

#endif