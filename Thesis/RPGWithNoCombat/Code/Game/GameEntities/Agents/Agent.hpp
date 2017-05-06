#pragma once

#ifndef AGENT_HPP
#define AGENT_HPP
#include "Game/GameEntities/BaseGameEntity.hpp"
#include "Game/GameEntities/Agents/AgentTypes.hpp"
#include "Game/GameEntities/Items/Item.hpp"
#include "Game/GameEntities/Agents/AgentStatPack.hpp"
#include "Engine/EventSystem/NamedProperties.hpp"

class Hair;
class CircleCollision;
struct TileCollisionTestPack;

class Agent : public BaseGameEntity
{
private:
protected:
	//Static Variables
	static int s_NameCharacterLimit;

	//Variables
	std::string m_AgentGeneratorUsed = "";
	std::string m_firstName = "";
	std::string m_secondName = "";
	std::string m_lastName = "";
	std::string m_jobTitle = "Jobless";
	std::string m_dialogueToOpenIfSpokenTo = "";
	std::string m_nameSet = "";
	AgentStatPack m_agentStatPack;
	NamedProperties* m_itemValues = nullptr;
	Hair* m_hair = nullptr;
	Agent* m_agentTalkingTo = nullptr;
	BaseGameEntity* m_EntityLastInteractedWith = nullptr;
	Item* m_defaultClothes = nullptr;
	Item* m_equippedClothing = nullptr;
	CircleCollision* m_circleCollision = nullptr;
	AABB2 m_DirectionAndAmountOutAgentCanInteractWith = AABB2(0.f, 0.f, 0.f, -1.f);
	Vector2 m_movementDirection = Vector2::vec2_zeros;
	Vector2 m_lastMovementDirectionWhenWasMoving = Vector2(0.f, -1.f);
	float m_amountOutFromAgentCenterCanInteractWith = 1.f;
	bool m_performAction1 = false;
	bool m_pauseGame = false;
	bool m_ignoreAllCollisionExceptMapBounds = false;
	bool m_ignoreMapBounds = false;
	bool m_talkingToSomeone = false;
	bool m_noMiddleNames = true;
	bool m_CanSprint = true;
	bool m_WantsToSprint = false;
	bool m_Sprinting = false;
public:
	//Constructors
	Agent(AgentTypes type = INVALID_AGENTTYPE,
		const SpriteResource* DefaultResource = nullptr, 
		Directions4 directionFacing = SOUTH);
	virtual ~Agent();


	//Updates
	virtual void Update(const float& dt) override;
protected:
	virtual void SetUpClothingForUpdate(Item* item, Directions4& dir);
	virtual void UpdateClothes(const float& dt, Directions4& dir);
	virtual void UpdateHair(const float& dt, Directions4& dir);
	virtual void UpdateDirectionMoving(Directions4& wantedAnimationDir, Directions4& dir);
	virtual void FireOnTouch(Vector2& position, TILE_COORDS& orgCoords);
	virtual void FireOnExit(Vector2& position, TILE_COORDS& orgCoords);
	virtual void FireOnEnter(Vector2& position, TILE_COORDS& orgCoords);
public:
	virtual void Render() const override;
protected:
	virtual void RenderClothes() const;
	virtual void RenderHair() const;
public:

	//Setters
	void SetAgentGeneratorUsed(const std::string& name) { m_AgentGeneratorUsed = name; };
	void SetNameSet(const std::string& nameSet) { m_nameSet = nameSet; };
	void SetTrueMovementSpeed(float speed) { m_agentStatPack.SetTrueMovementSpeed(speed, false); };
	void SetAgentType(AgentTypes type) { m_agentStatPack.SetAgentType(type); };
	void SetCurrentMovementSpeed(float speed) { m_agentStatPack.SetCurrentMovementSpeed(speed); };
	void SetSprintingMovementSpeed(float speed) { m_agentStatPack.SetSprintingMovementSpeed(speed); };
	void SetMovementDirection(const Vector2& direction) { m_movementDirection = direction; };
	void SetWantsToSprint(bool sprint) { m_WantsToSprint = sprint; };
	void SetCanSprint(bool sprint) { m_CanSprint = sprint; };
	void ResetCurrentMovementSpeedToTrueMovementSpeed();
	void SetEquippedClothing(Item* item);
	void SetHair(Hair* hair);
	void SetEntityLastInteractedWith(BaseGameEntity* entity);
	void ParseNameForFirstSecondAndLast();
	void SetFirstName(const std::string& name);
	void SetLastName(const std::string& name);
	void SetJobTitle(const std::string& jobTitle);
	void SetDialogueToOpenIfSpokenTo(const std::string& dialogueName);
	void SetIsMale(bool male);
	void SetTalkingToTarget(Agent* agent);
	void SetIsTalkingToSomeone(bool talkingToSomeone);
	void EnableTalkingToMode();
	void DisableTalkingToMode();
	virtual void SetAgentName(const std::string& name) override;
	void RandomizeAllStats();
	void RestoreMaxAndMinStatValuesToGlobalDefaults();
	void SetOrAddStatByStat(const AgentStat& stat);
	void SetStatByName(const std::string& statName, int val);
	void SetStatByAbbreviation(const std::string& statAbbreviation, int val);
	void ClearAndCopyStatsFromDefaultSet();
	const std::string GetAgentTypeAsString() const;

	template<typename T>
	void SetItemValue(const std::string& ItemName, const T& value);

	//Getters
	const std::string& GetNameSet() const;
	const NamedProperties* GetNamedProperties() const;
	const std::vector<AgentStat>& GetStats() const;
	const std::vector<AgentStat>* GetStatsPtr() const;
	std::vector<AgentStat>* GetStatsEditablePtr();
	int GetValOfStatByName(const std::string& name, bool& found) const;
	int GetValOfStatByAbbreviation(const std::string& abbreviation, bool& found) const;
	const BaseGameEntity* GetEntityLastInteractedWith() const { return m_EntityLastInteractedWith; };
	BaseGameEntity* GetEditableEntityLastInteractedWith() { return m_EntityLastInteractedWith; };
	const std::string& GetFirstName() const { return m_firstName; };
	const std::string& GetSecondName() const { return m_secondName; };
	const std::string& GetLastName() const { return m_lastName; };
	const std::string& GetJobTitle() const { return m_jobTitle; };
	const std::string& GetDialogueToOpenIfSpokenTo() const { return m_dialogueToOpenIfSpokenTo; };
	Agent* GetTalkingTo() const { return m_agentTalkingTo; };
	bool GetTalkingToSomeone() const { return m_talkingToSomeone; };
	AgentTypes GetAgentType() const { return m_agentStatPack.GetAgentType(); };
	float GetMovementSpeed() const { return m_agentStatPack.GetCurrentMovementSpeed(); };
	const Vector2& GetMovementDirection() const { return m_movementDirection; };
	bool GetIsMale() const { return m_agentStatPack.GetIsMale(); };
	bool GetHasNoMiddleNames() const { return m_noMiddleNames; };

	template<typename T>
	void GetItemValue(const std::string& itemName, T& value);
	template<typename T>
	void GetItemValue(const std::string& itemName, T& value, const T& defaultValue);

	//operations
	virtual void WriteOutEntityImportantDataToStr(std::string& str, int currentIndentationLevel = 0) const override;
	void ClearWhoTalkingTo();
protected:
	void CheckCollisionAgainstAssociatedTiles(Vector2& pos, TileCollisionTestPack& tileCollisionPack);
	void CheckCollisionAgainstMap(Vector2& pos);
public:

	//Static Functions
	static int GetNameCharacterLimit();
};

//----------------------------------------------------------------
//----------------------------------------------------------------
//----------------------------------------------------------------
//Setters
template<typename T>
void Agent::SetItemValue(const std::string& ItemName, const T& value)
{
	if (m_itemValues == nullptr)
	{
		return;
	}
	m_itemValues->Set<T>(ItemName, value);
}


//----------------------------------------------------------------
//----------------------------------------------------------------
//----------------------------------------------------------------
//Getters
template<typename T>
void Agent::GetItemValue(const std::string& itemName, T& value)
{
	if (m_itemValues == nullptr)
	{
		return;
	}
	std::string lowerCase = MakeLower(itemName);
	m_itemValues->Get<T>(lowerCase, value);
}

template<typename T>
void Agent::GetItemValue(const std::string& itemName, T& value, const T& defaultValue)
{
	if (m_itemValues == nullptr)
	{
		value = defaultValue;
		return;
	}
	std::string lowerCase = MakeLower(itemName);
	m_itemValues->Get<T>(lowerCase, value, defaultValue);
}

#endif