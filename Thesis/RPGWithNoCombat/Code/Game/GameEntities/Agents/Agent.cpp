#include "Game/GameEntities/Agents/Agent.hpp"
#include "Game/Map/Map.hpp"
#include "Game/Common/GameCommonVars.hpp"
#include "Game/Collision/CircleCollision.hpp"
#include "Game/Collision/CollisionTestFunctions.hpp"
#include "Game/GameEntities/Agents/TileCollisionTestPack.hpp"
#include "Game/GameEntities/Hair/Hair.hpp"
#include "Engine/Math/MathToStringUtils.hpp"
#include "Game/Common/GameDebuggingCommons.hpp"
#include "Game/Debugging/NamedPropertiesToString.hpp"
#include <vector>

//-----------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------
//Static Variables
STATIC int Agent::s_NameCharacterLimit = 8;

//-----------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------
//Constructors
Agent::Agent(AgentTypes type, const SpriteResource* DefaultResource, Directions4 directionFacing)
	: BaseGameEntity(AGENT_ENTITY, DefaultResource, directionFacing),
	m_agentStatPack(type)
{
	m_agentStatPack.ClearAndCopyAllStatsFromCommonStats();
	m_itemValues = new NamedProperties();
	m_circleCollision = new CircleCollision();
	m_circleCollision->SetRadius(0.45f);
	m_collision = m_circleCollision;
	m_entityStatPack.SetRenderOffset(Vector2(0.f, 0.2f));
	m_DirectionAndAmountOutAgentCanInteractWith = AABB2(Vector2::vec2_zeros,
		Directions::GetDirectionForDirectionEnum(m_entityStatPack.GetCurrentAnimationDirectionIdx())
		* m_amountOutFromAgentCenterCanInteractWith);

	SetIsMale(m_agentStatPack.GetIsMale());
}

Agent::~Agent()
{
	BaseGameEntity::~BaseGameEntity();
	if (m_itemValues != nullptr)
	{
		delete m_itemValues;
		m_itemValues = nullptr;
	}
	if (m_circleCollision != nullptr)
	{
		delete m_circleCollision;
		m_circleCollision = nullptr;
	}
	if (m_defaultClothes != nullptr)
	{
		delete m_defaultClothes;
		m_defaultClothes = nullptr;
	}
	if (m_equippedClothing != nullptr)
	{
		delete m_equippedClothing;
		m_equippedClothing = nullptr;
	}
	if (m_hair != nullptr)
	{
		delete m_hair;
		m_hair = nullptr;
	}
}

//-----------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------
//Updates
void Agent::Update(const float& dt)
{
	float animSpeedMult = (m_Sprinting) ? 2.0f : 1.0f;

	Vector2 pos = GetPosition();
	Vector2 position = pos;
	TILE_COORDS orgCoords = m_entityStatPack.GetBLTileCoords();

	if (m_talkingToSomeone == false)
	{
		pos = pos + (m_movementDirection * (dt * m_agentStatPack.GetCurrentMovementSpeed()));
	}

	if (abs(m_agentStatPack.GetCurrentMovementSpeed()) > 0.f)
	{
		float movementDirLength = m_movementDirection.GetLength();
		m_entityStatPack.SetCurrentAnimationPlaySpeed(movementDirLength * animSpeedMult);
		if (movementDirLength > 0.f)
		{
			m_lastMovementDirectionWhenWasMoving = m_movementDirection;
		}
	}
	else
	{
		m_entityStatPack.SetCurrentAnimationPlaySpeed(0.f);
		m_lastMovementDirectionWhenWasMoving = m_movementDirection;
	}

	int wantedAnimIdx = m_entityStatPack.GetCurrentAnimationIdx();
	Directions4 wantedAnimationDir = m_entityStatPack.GetCurrentAnimationDirectionIdx();
	Directions4 dir = m_entityStatPack.GetCurrentAnimationDirectionIdx();
	UpdateDirectionMoving(wantedAnimationDir, dir);


	ChangeCurrentAnimationPlaying(wantedAnimationDir, wantedAnimIdx);
	CheckCollisionAgainstMap(pos);
	SetPosition(pos);

	FireOnTouch(position, orgCoords);
	FireOnExit(position, orgCoords);

	BaseGameEntity::Update(dt);
	if (orgCoords != m_entityStatPack.GetBLTileCoords())
	{
		position = m_entityStatPack.GetPosition();
		orgCoords = m_entityStatPack.GetBLTileCoords(); 
		FireOnEnter(position, orgCoords);
	}

	UpdateClothes(dt, dir);
	UpdateHair(dt, dir);
}

void Agent::SetUpClothingForUpdate(Item* item, Directions4& dir)
{
	if (item == nullptr)
	{
		return;
	}
	item->SetEquipped(true);
	item->SetAgentEquippedTo(this);
	item->SetDebugSpriteIdxInAnimationOverride(m_entityStatPack.GetCurrentAnimationFrameIdx());
	item->SetAnimationPlayTime(m_currentAnimationPlayTime);
	item->ChangeCurrentAnimationPlaying(dir, m_entityStatPack.GetCurrentAnimationIdx());
	item->SetRenderOffset(m_entityStatPack.GetRenderOffset());
	item->SetPosition(m_entityStatPack.GetPosition());
}

void Agent::UpdateClothes(const float& dt, Directions4& dir)
{
	if (m_equippedClothing != nullptr)
	{
		SetUpClothingForUpdate(m_equippedClothing, dir);
		m_equippedClothing->Update(dt);
	}
	else if (m_defaultClothes != nullptr)
	{
		SetUpClothingForUpdate(m_defaultClothes, dir);
		m_defaultClothes->Update(dt);
	}
}

void Agent::UpdateHair(const float& dt, Directions4& dir)
{
	if (m_hair != nullptr)
	{
		m_hair->SetDebugSpriteIdxInAnimationOverride(m_entityStatPack.GetCurrentAnimationFrameIdx());
		m_hair->SetAnimationPlayTime(m_currentAnimationPlayTime);
		m_hair->ChangeCurrentAnimationPlaying(dir, m_entityStatPack.GetCurrentAnimationIdx());
		m_hair->SetRenderOffset(m_entityStatPack.GetRenderOffset());
		m_hair->SetPosition(m_entityStatPack.GetPosition());
		m_hair->Update(dt);
	}
}

void Agent::UpdateDirectionMoving(Directions4& wantedAnimationDir, Directions4& dir)
{
	if (abs(m_lastMovementDirectionWhenWasMoving.x) >= abs(m_lastMovementDirectionWhenWasMoving.y))
	{
		if (m_lastMovementDirectionWhenWasMoving.x > 0.f)
		{
			wantedAnimationDir = EAST; //move right
		}
		else
		{
			wantedAnimationDir = WEST; //move left
		}
	}
	else
	{
		if (m_lastMovementDirectionWhenWasMoving.y > 0.f)
		{
			wantedAnimationDir = NORTH; // move up
		}
		else
		{
			wantedAnimationDir = SOUTH; //move down
		}
	}
	if (wantedAnimationDir != dir)
	{
		m_DirectionAndAmountOutAgentCanInteractWith = AABB2(Vector2::vec2_zeros,
			Directions::GetDirectionForDirectionEnum(dir) * m_amountOutFromAgentCenterCanInteractWith);
	}
}

void Agent::FireOnTouch(Vector2& position, TILE_COORDS& orgCoords)
{
	NamedProperties paramsTouch;
	WriteBaseGameEntityToNamedProperties(paramsTouch, this);
	paramsTouch.Set("originalPosition", position);
	paramsTouch.Set("originalTileCoords", orgCoords);
	EventSystem::GlobalFireEvent("OnTouch", paramsTouch);
}

void Agent::FireOnExit(Vector2& position, TILE_COORDS& orgCoords)
{
	NamedProperties paramsExit;
	WriteBaseGameEntityToNamedProperties(paramsExit, this);
	paramsExit.Set("originalPosition", position);
	paramsExit.Set("originalTileCoords", orgCoords);
	EventSystem::GlobalFireEvent("OnExit", paramsExit);
}

void Agent::FireOnEnter(Vector2& position, TILE_COORDS& orgCoords)
{
	NamedProperties paramsEnter;
	WriteBaseGameEntityToNamedProperties(paramsEnter, this);
	paramsEnter.Set("originalPosition", position);
	paramsEnter.Set("originalTileCoords", orgCoords);
	EventSystem::GlobalFireEvent("OnEnter", paramsEnter);
}

void Agent::Render() const
{
	BaseGameEntity::Render();
	RenderClothes();
	RenderHair();
}

void Agent::RenderClothes() const
{
	if (m_equippedClothing != nullptr)
	{
		m_equippedClothing->Render();
	}
	else if(m_defaultClothes != nullptr)
	{
		m_defaultClothes->Render();
	}
}

void Agent::RenderHair() const
{
	if (m_hair != nullptr)
	{
		m_hair->Render();
	}
}

//-----------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------
//Setters
void Agent::ResetCurrentMovementSpeedToTrueMovementSpeed()
{ 
	if (m_Sprinting)
	{
		m_agentStatPack.ResetCurrentMovementSpeedSprinting();
	}
	else
	{
		m_agentStatPack.ResetCurrentMovementSpeed();
	}
}

void Agent::SetEquippedClothing(Item* item)
{
	m_equippedClothing = item;
}

void Agent::SetHair(Hair* hair)
{
	if (m_hair != nullptr)
	{
		delete m_hair;
	}
	m_hair = hair;
}

void Agent::SetEntityLastInteractedWith(BaseGameEntity* entity)
{
	m_EntityLastInteractedWith = entity;
}

void Agent::ParseNameForFirstSecondAndLast()
{
	m_noMiddleNames = true;
	std::vector<std::string> parsedName = ParseString(m_name, ' ');
	if (parsedName.size() > 0)
	{
		m_firstName = parsedName[0];
	}
	else
	{
		return;
	}
	if (parsedName.size() > 1)
	{
		m_secondName = parsedName[1];
		m_lastName = parsedName[1];
	}
	else
	{
		return;
	}
	
	if (parsedName.size() > 2)
	{
		m_noMiddleNames = false;
		m_lastName = parsedName[parsedName.size() - 1];
	}

}

void Agent::SetFirstName(const std::string& name)
{
	m_firstName = name;
}

void Agent::SetLastName(const std::string& name)
{
	m_lastName = name;
}

void Agent::SetJobTitle(const std::string& jobTitle)
{
	if (jobTitle == "")
	{
		m_jobTitle = "Jobless";
	}
	else
	{
		m_jobTitle = jobTitle;
	}
}

void Agent::SetDialogueToOpenIfSpokenTo(const std::string& dialogueName)
{
	m_dialogueToOpenIfSpokenTo = dialogueName;
}

void Agent::SetIsMale(bool male)
{
	m_agentStatPack.SetIsMale(male);

	if (m_defaultClothes != nullptr)
	{
		delete m_defaultClothes;
		m_defaultClothes = nullptr;
	}

	if (male)
	{
		m_defaultClothes = Item::GetCopyDefaultMaleClothes();
	}
	else
	{
		m_defaultClothes = Item::GetCopyDefaultFemaleClothes();
	}
}


void Agent::SetTalkingToTarget(Agent* agent)
{
	m_agentTalkingTo = agent;
}

void Agent::SetIsTalkingToSomeone(bool talkingToSomeone)
{
	m_talkingToSomeone = talkingToSomeone;
}

void Agent::EnableTalkingToMode()
{
	m_talkingToSomeone = true;
}

void Agent::DisableTalkingToMode()
{
	m_talkingToSomeone = false;
}

void Agent::SetAgentName(const std::string& name)
{
	BaseGameEntity::SetAgentName(name);
	std::vector < std::string> parsed = ParseString(name, ' ');
	m_noMiddleNames = true;
	if (parsed.size() > 0)
	{
		m_firstName = parsed[0];
		m_lastName = "";
	}
	if (parsed.size() > 1)
	{
		m_secondName = parsed[1];
		m_lastName = m_secondName;
	}
	if (parsed.size() > 2)
	{
		m_noMiddleNames = false;
		m_lastName = parsed[parsed.size() - 1];
	}
}

void Agent::RandomizeAllStats()
{
	m_agentStatPack.RandomizeAllStats();
}

void Agent::RestoreMaxAndMinStatValuesToGlobalDefaults()
{
	m_agentStatPack.RestoreMaxAndMinStatValuesToGlobalDefaults();
}

void Agent::SetOrAddStatByStat(const AgentStat& stat)
{
	m_agentStatPack.SetOrAddAgentStat(stat);
}

void Agent::SetStatByName(const std::string& statName, int val)
{
	m_agentStatPack.SetOrAddAgentStat(statName, val);
}

void Agent::SetStatByAbbreviation(const std::string& statAbbreviation, int val)
{
	m_agentStatPack.SetOrAddAgentStatAbbreviation(statAbbreviation, val);
}

void Agent::ClearAndCopyStatsFromDefaultSet()
{
	m_agentStatPack.ClearAndCopyAllStatsFromCommonStats();
}

const std::string Agent::GetAgentTypeAsString() const
{
	AgentTypes type = GetAgentType();
	switch (type)
	{
	case INVALID_AGENTTYPE: return "INVALID";
	case PLAYER_AGENTTYPE: return "PLAYER";
	case NPC_AGENTTYPE: return "NPC";
	default: return "";
	}
}

//-----------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------
//Getters
const std::string& Agent::GetNameSet() const
{
	return m_nameSet;
}

const NamedProperties* Agent::GetNamedProperties() const
{
	return m_itemValues;
}

const std::vector<AgentStat>& Agent::GetStats() const
{
	return m_agentStatPack.GetAllStats();
}

const std::vector<AgentStat>* Agent::GetStatsPtr() const
{
	return m_agentStatPack.GetAllStatsPtr();
}

std::vector<AgentStat>* Agent::GetStatsEditablePtr()
{
	return m_agentStatPack.GetAllStatsEditablePtr();
}

int Agent::GetValOfStatByName(const std::string& name, bool& found) const
{
	return m_agentStatPack.GetStatValueByStatName(name, found);
}

int Agent::GetValOfStatByAbbreviation(const std::string& abbreviation, bool& found) const
{
	return m_agentStatPack.GetStatValueByStatAbbreviation(abbreviation, found);
}

//-----------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------
//operations
void Agent::WriteOutEntityImportantDataToStr(std::string& str, int currentIndentationLevel) const
{
	BaseGameEntity::WriteOutEntityImportantDataToStr(str, currentIndentationLevel);
	currentIndentationLevel += 3;
	GameDebuggingCommons::AddIndentation(str, currentIndentationLevel);
	str += "Agent Type: " + GetAgentTypeAsString() + "\n";

	GameDebuggingCommons::AddIndentation(str, currentIndentationLevel);
	str += "Agent Generator Used: " + m_AgentGeneratorUsed + "\n";

	bool male = GetIsMale();
	GameDebuggingCommons::AddIndentation(str, currentIndentationLevel);
	str += "Gender: ";
	if (male)
	{
		str += "MALE\n";
	}
	else
	{
		str += "FEMALE\n";
	}

	GameDebuggingCommons::AddIndentation(str, currentIndentationLevel);
	str += "Dialogue to open if spoken to: " + m_dialogueToOpenIfSpokenTo + "\n";


	int levelUp = currentIndentationLevel + 3;
	GameDebuggingCommons::AddIndentation(str, currentIndentationLevel);
	str += "Hair: ";
	if (m_hair == nullptr)
	{
		str += "NULL\n";
	}
	else
	{
		str += "\n";
		m_hair->WriteOutEntityImportantDataToStr(str, levelUp);
	}

	if (m_equippedClothing != nullptr)
	{
		GameDebuggingCommons::AddIndentation(str, currentIndentationLevel);
		str += "Equipped Clothes: ";
		m_equippedClothing->WriteOutEntityImportantDataToStr(str, levelUp);
	}

	if (m_defaultClothes != nullptr)
	{
		GameDebuggingCommons::AddIndentation(str, currentIndentationLevel);
		str += "Default Clothes: ";
		m_defaultClothes->WriteOutEntityImportantDataToStr(str, levelUp);
	}


	GameDebuggingCommons::AddIndentation(str, currentIndentationLevel);
	str += "Agent Stats: \n";
	const std::vector<AgentStat>& stats = m_agentStatPack.GetAllStats();
	for (size_t i = 0; i < stats.size(); i++)
	{
		const AgentStat& stat = stats[i];
		stat.WriteAgentStatToString(str, levelUp);
	}

	GameDebuggingCommons::AddIndentation(str, currentIndentationLevel);
	str += "Item Values: ";
	if (m_itemValues == nullptr || m_itemValues->Size() == 0)
	{
		str += "NONE\n";
	}
	else
	{
		str += "\n";
		NamedPropertiesToString::ConvertAllKnownNamedPropertyValuesToString(m_itemValues, str, levelUp);
	}
}

void Agent::ClearWhoTalkingTo()
{
	m_talkingToSomeone = false;
	if (m_agentTalkingTo != nullptr)
	{
		m_agentTalkingTo->SetTalkingToTarget(nullptr);
		m_agentTalkingTo->SetIsTalkingToSomeone(false);
		m_agentTalkingTo = nullptr;
	}
}

void Agent::CheckCollisionAgainstAssociatedTiles(Vector2& pos, TileCollisionTestPack& tileCollisionPack)
{
	//NSEW first
	if (tileCollisionPack.northTile != nullptr
		&& tileCollisionPack.northIsSolid)
	{
		CollisionTestResult result =
			CollisionTestFunctions::TestCollisionFromSquareCollisionToCircleCollisionNESW(
				tileCollisionPack.northTile->GetSquareCollision(),
				tileCollisionPack.northTile->GetBLPos(), m_circleCollision, pos, false, false);
		if (result.GetOverlapDetected())
		{
			pos -= result.GetDistanceBetweenCenters();
		}
	}
	else if (tileCollisionPack.northTile == nullptr
		&& tileCollisionPack.currentTile != nullptr)
	{
		CollisionTestResult result =
			CollisionTestFunctions::TestCollisionFromSquareCollisionToCircleCollisionNESW(
				tileCollisionPack.currentTile->GetSquareCollision(),
				tileCollisionPack.currentTile->GetBLPos() + Vector2(0,1), m_circleCollision, pos, false, false);
		if (result.GetOverlapDetected())
		{
			pos -= result.GetDistanceBetweenCenters();
		}
	}

	if (tileCollisionPack.eastTile != nullptr && tileCollisionPack.eastIsSolid)
	{
		CollisionTestResult result =
			CollisionTestFunctions::TestCollisionFromSquareCollisionToCircleCollisionNESW(
				tileCollisionPack.eastTile->GetSquareCollision(),
				tileCollisionPack.eastTile->GetBLPos(), m_circleCollision, pos, true, false);
		if (result.GetOverlapDetected())
		{
			pos -= result.GetDistanceBetweenCenters();
		}
	}

	if (tileCollisionPack.southTile != nullptr && tileCollisionPack.southIsSolid)
	{
		CollisionTestResult result =
			CollisionTestFunctions::TestCollisionFromSquareCollisionToCircleCollisionNESW(
				tileCollisionPack.southTile->GetSquareCollision(),
				tileCollisionPack.southTile->GetBLPos(), m_circleCollision, pos, false, true);
		if (result.GetOverlapDetected())
		{
			pos -= result.GetDistanceBetweenCenters();
		}
	}

	if (tileCollisionPack.westTile != nullptr && tileCollisionPack.westIsSolid)
	{
		CollisionTestResult result =
			CollisionTestFunctions::TestCollisionFromSquareCollisionToCircleCollisionNESW(
				tileCollisionPack.westTile->GetSquareCollision(),
				tileCollisionPack.westTile->GetBLPos(), m_circleCollision, pos, true, true);
		if (result.GetOverlapDetected())
		{
			pos -= result.GetDistanceBetweenCenters();
		}
	}

	//Diag second
	if (tileCollisionPack.northEastTile != nullptr && tileCollisionPack.northEastIsSolid)
	{
		CollisionTestResult result =
			CollisionTestFunctions::TestCollisionFromSquareCollisionToCircleCollisionDiag(
				tileCollisionPack.northEastTile->GetSquareCollision(),
				tileCollisionPack.northEastTile->GetBLPos(), m_circleCollision, pos, false, false);
		if (result.GetOverlapDetected())
		{
			pos += result.GetDistanceBetweenCenters();
		}
	}

	if (tileCollisionPack.northWestTile != nullptr && tileCollisionPack.northWestIsSolid)
	{
		CollisionTestResult result =
			CollisionTestFunctions::TestCollisionFromSquareCollisionToCircleCollisionDiag(
				tileCollisionPack.northWestTile->GetSquareCollision(),
				tileCollisionPack.northWestTile->GetBLPos(), m_circleCollision, pos, true, false);
		if (result.GetOverlapDetected())
		{
			pos -= result.GetDistanceBetweenCenters();
		}
	}

	if (tileCollisionPack.southEastTile != nullptr && tileCollisionPack.southEastIsSolid)
	{
		CollisionTestResult result =
			CollisionTestFunctions::TestCollisionFromSquareCollisionToCircleCollisionDiag(
				tileCollisionPack.southEastTile->GetSquareCollision(),
				tileCollisionPack.southEastTile->GetBLPos(), m_circleCollision, pos, false, true);
		if (result.GetOverlapDetected())
		{
			pos += result.GetDistanceBetweenCenters();
		}
	}

	if (tileCollisionPack.southWestTile != nullptr && tileCollisionPack.southWestIsSolid)
	{
		CollisionTestResult result =
			CollisionTestFunctions::TestCollisionFromSquareCollisionToCircleCollisionDiag(
				tileCollisionPack.southWestTile->GetSquareCollision(),
				tileCollisionPack.southWestTile->GetBLPos(), m_circleCollision, pos, true, true);
		if (result.GetOverlapDetected())
		{
			pos -= result.GetDistanceBetweenCenters();
		}
	}
}


void Agent::CheckCollisionAgainstMap(Vector2& pos)
{
	const Map* map = Map::GetMap();
	if (map != nullptr)
	{
		if (m_ignoreAllCollisionExceptMapBounds == false)
		{
			TILE_COORDS tilePos = map->ConvertPositionToTileCoords(pos);
			const Tile* tile = map->GetTileAtCoords(tilePos);
			if (tile != nullptr)
			{
				TileCollisionTestPack collisionPack;
				TileCollisionTestPack::LoadTileCollisionTestPack(collisionPack, tile);
				CheckCollisionAgainstAssociatedTiles(pos, collisionPack);
			}
		}

		if (m_ignoreMapBounds == false)
		{
			//Finally, guarantee not outside the map.
			Vector2 blPos = map->GetBLMapPos();
			Vector2 trPos = map->GetTRMapPos();

			if (pos.x < blPos.x)
			{
				pos.x = blPos.x;
			}
			else if (pos.x > trPos.x)
			{
				pos.x = trPos.x;
			}

			if (pos.y < blPos.y)
			{
				pos.y = blPos.y;
			}
			else if (pos.y > trPos.y)
			{
				pos.y = trPos.y;
			}
		}

	}
}

//-----------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------
//Static Functions
STATIC int Agent::GetNameCharacterLimit()
{
	return s_NameCharacterLimit;
}