#include "Game/GameEntities/Agents/Player/Player.hpp"
#include "Game/Common/GameCommonVars.hpp"
#include "Game/Controllers/Controller.hpp"
#include "Game/Dialogue/Dialogue.hpp"
#include "Game/Collision/CircleCollision.hpp"
#include "Game/GameEntities/Features/Feature.hpp"
#include "Game/Map/Map.hpp"
#include "Game/Dialogue/DialogueSegment.hpp"
#include "Engine/Math/MathToStringUtils.hpp"
#include "Game/Common/GameDebuggingCommons.hpp"

//-----------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------
//Constructors
Player::Player(const SpriteResource* resource)
	: Agent(PLAYER_AGENTTYPE, resource, SOUTH)
{
}

Player::~Player()
{
	Agent::~Agent();
}

//-----------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------
//Setters
void Player::AddQuest(Quest* quest)
{
	if (quest == nullptr)
	{
		return;
	}
}

void Player::AddQuestByTitle(const std::string& name)
{
	m_questManager.AddQuestByTitle(name);
}

void Player::SetPlayerDialogueChoice(const std::string& dialogueChoice)
{
	m_currentPlayerDialogueChoicePicked = dialogueChoice;
}

void Player::SetPlayerDialogueChoiceIdx(int idx)
{
	m_currentDialogueChoiceIdx = idx;
}

void Player::ClearPlayerDialogueChoice()
{
	m_currentPlayerDialogueChoicePicked = "";
	m_currentDialogueChoiceIdx = -1;
}

//-----------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------
//Getters
const PlayerQuestManager& Player::GetPlayerQuestManager() const
{
	return m_questManager;
}

const PlayerQuestManager* Player::GetPlayerQuestManagerPtr() const
{
	return &m_questManager;
}

PlayerQuestManager* Player::GetEditablePlayerQuestManagerPtr()
{
	return &m_questManager;
}

QuestEventReqsMet Player::GetIdxPlayerQuestThatMeetsExpectationsOf()
{
	QuestEventReqsMet eventReqsMet;
	std::vector<Quest*>* playerReggedQuests= m_questManager.GetQuestsEditablePtr();
	if (playerReggedQuests != nullptr && playerReggedQuests->size() > 0)
	{
		for (size_t questIdx = 0; questIdx < playerReggedQuests->size(); questIdx++)
		{
			Quest* quest = playerReggedQuests->at(questIdx);
			if (quest == nullptr)
			{
				continue;
			}
			quest->PlugInPlayerUnderVariableName(this);
			int questReqMet = quest->GetFirstEventThatHasRequirementsMet();
			if (questReqMet != -1)
			{
				eventReqsMet.m_questIdx = questIdx;
				eventReqsMet.m_eventIdx = questReqMet;
				return eventReqsMet;
			}
		}
	}
	Map* map = Map::GetEditableMap();
	if (map != nullptr)
	{
		std::vector<Quest*>* mapQuests = map->GetQuestsEditablePtr();
		if (mapQuests != nullptr && mapQuests->size() > 0)
		{
			for (size_t questIdx = 0; questIdx < mapQuests->size(); questIdx++)
			{
				Quest* quest = mapQuests->at(questIdx);
				if (quest == nullptr)
				{
					continue;
				}
				quest->PlugInPlayerUnderVariableName(this);
				int questReqMet = quest->GetFirstEventThatHasRequirementsMet();
				if (questReqMet != -1)
				{
					mapQuests->erase(mapQuests->begin() + questIdx);
					size_t newQuestIdx = m_questManager.GetNumberOfHeldQuests();
					m_questManager.AddQuest(quest);
					eventReqsMet.m_questIdx = newQuestIdx;
					eventReqsMet.m_eventIdx = questReqMet;
					return eventReqsMet;
				}

			}
		}
	}

	return eventReqsMet;
}

const std::string& Player::GetPlayerChoiceSegmentName() const
{
	return m_currentDialogueSegmentThatPlayerMadeChoiceAt;
}

const std::string& Player::GetPlayerDialogueChoicePicked() const
{
	return m_currentPlayerDialogueChoicePicked;
}

const std::string& Player::GetLastPlayerChoiceSegmentName() const
{
	return m_lastDialogueSegmentThatPlayerMadeChoiceAt;
}

const std::string& Player::GetLastPlayerDialgoueChoicePicked() const
{
	return m_lastPlayerDialogueChoicePicked;
}

int Player::GetPlayerDialogueChoiceIdx() const
{
	return m_currentDialogueChoiceIdx;
}

//-----------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------
//Operations
void Player::WriteDeveloperDebugInfo(DeveloperDebugInfoBox& info) const
{
	DeveloperDebugInfoTextBlurb whatAmIBlurb = DeveloperDebugInfoTextBlurb("Entity type: AGENT, PLAYER",
		Rgba::s_White);
	info.AddTextBlurb(whatAmIBlurb);
	Agent::WriteDeveloperDebugInfo(info);
}

void Player::WriteOutEntityImportantDataToStr(std::string& str, int currentIndentationLevel) const
{
	Agent::WriteOutEntityImportantDataToStr(str, currentIndentationLevel);
}

void Player::WriteOutAllQuestsToString(std::string& str, int currentIndentationLevel) const
{
	m_questManager.WritePlayerQuestManagerToString(str, currentIndentationLevel);
}

bool Player::WriteOutQuestDataByNameToString(std::string& str, const std::string& questName,
	int currentIndentationLevel) const
{
	if (m_questManager.WriteQuestByNameToString(str, questName, currentIndentationLevel))
	{
		return true;
	}

	if (Map::WriteActiveQuestByNameToStringFromAllMaps(str, questName, currentIndentationLevel))
	{
		return true;
	}

	return false;
}

//-----------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------
//Updates
void Player::Update(const float& dt)
{
	Vector2 movementVector = Vector2::vec2_zeros;
	ReadControls(movementVector);

	if (m_performAction1Action)
	{
		if (m_talkingToSomeone)
		{
			PerformAction1Update();
		}
		else
		{
			PerformNonAction1Update();
		}
	}

	if (m_talkingToSomeone)
	{
		SetMovementDirection(Vector2::vec2_zeros);
		HandleDialogueInput();
	}
	else
	{
		SetMovementDirection(movementVector);
		if (m_currentDialogueChoiceIdx != 0)
		{
			m_currentDialogueChoiceIdx = 0;
		}
		if (m_currentPlayerDialogueChoicePicked != "")
		{
			m_lastDialogueSegmentThatPlayerMadeChoiceAt = m_currentDialogueSegmentThatPlayerMadeChoiceAt;
			m_currentDialogueSegmentThatPlayerMadeChoiceAt = "";
			m_lastPlayerDialogueChoicePicked = m_currentPlayerDialogueChoicePicked;
			m_currentPlayerDialogueChoicePicked = "";
		}
	}

	ResetCurrentMovementSpeedToTrueMovementSpeed();

	Agent::Update(dt);
}

void Player::ReadControls(Vector2& movementVector)
{
	bool action1KeyDown = false;
	bool pauseKeyDown = false;
	bool sprinting = false;
	if (GameCommons::s_PlayerController != nullptr)
	{
		movementVector = GameCommons::s_PlayerController->GetMovementVector();
		action1KeyDown = GameCommons::s_PlayerController->GetIsAction1Down();
		pauseKeyDown = GameCommons::s_PlayerController->GetIsPauseKeyDown();
		sprinting = GameCommons::s_PlayerController->GetIsSprintDown();
	}
	ReadControlsDialogueMovement(movementVector);

	m_Sprinting = sprinting;

	if (m_action1KeyIsDown && action1KeyDown)
	{
		m_action1KeyIsHeld = true;
	}
	else
	{
		m_action1KeyIsHeld = false;
	}
	if (action1KeyDown != m_action1KeyIsDown)
	{
		m_action1KeyIsDown = action1KeyDown;
	}

	if (m_action1KeyIsDown && !m_action1KeyIsHeld)
	{
		m_performAction1Action = true;
	}
	else
	{
		m_performAction1Action = false;
	}
}

void Player::ReadControlsDialogueMovement(const Vector2& movementVector)
{
	if (movementVector.y != 0.f)
	{
		if (movementVector.y > 0.f)
		{
			m_MoveSouthKeyPressed = false;
			m_MoveSouthKeyHeld = false;
			if (m_MoveNorthKeyPressed)
			{
				m_MoveNorthKeyHeld = true;
			}
			else
			{
				m_MoveNorthKeyPressed = true;
				m_MoveNorthKeyHeld = false;
			}
		}
		else
		{
			m_MoveNorthKeyPressed = false;
			m_MoveNorthKeyHeld = false;
			if (m_MoveSouthKeyPressed)
			{
				m_MoveSouthKeyHeld = true;
			}
			else
			{
				m_MoveSouthKeyPressed = true;
				m_MoveSouthKeyHeld = false;
			}
		}
	}
	else
	{
		m_MoveNorthKeyPressed = false;
		m_MoveSouthKeyPressed = false;
		m_MoveNorthKeyHeld = false;
		m_MoveSouthKeyHeld = false;
	}
}

void Player::HandleDialogueInput()
{
	Dialogue* dialogue = Dialogue::SpawnOrGetCentralDialogue();
	if (dialogue == nullptr)
	{
		return;
	}
	DialogueSegment* segment = dialogue->GetCurrentDialogueSegment();
	if (segment == nullptr)
	{
		return;
	}

	bool moveUp = m_MoveNorthKeyPressed && !m_MoveNorthKeyHeld;
	bool moveDown = m_MoveSouthKeyPressed && !m_MoveSouthKeyHeld;
	if (moveUp && !moveDown)
	{
		segment->MoveSelectionUp();
	}
	else if (!moveUp && moveDown)
	{
		segment->MoveSelectionDown();
	}
}


void Player::PerformAction1Update()
{
	//Progress Dialogue
	Dialogue* dialogue = Dialogue::SpawnOrGetCentralDialogue();
	if (dialogue == nullptr)
	{
		ClearWhoTalkingTo();
	}
	else
	{
		DialogueSegment* segment = dialogue->GetCurrentDialogueSegment();
		if (segment == nullptr)
		{
			ClearWhoTalkingTo();
			return;
		}
		std::string segmentName = segment->GetDialogueSegmentName();
		DialogueChoice currentPlayerChoice = dialogue->TriggerDialogueSegmentStep();
		bool playerDialogueChoiceValid = currentPlayerChoice.GetDialogueChoice() != "";
		if (playerDialogueChoiceValid)
		{
			if (m_currentPlayerDialogueChoicePicked != "")
			{
				m_lastPlayerDialogueChoicePicked = m_currentPlayerDialogueChoicePicked;
				m_lastDialogueSegmentThatPlayerMadeChoiceAt = m_currentDialogueSegmentThatPlayerMadeChoiceAt;
			}
			m_currentPlayerDialogueChoicePicked = currentPlayerChoice.GetDialogueChoice();
			m_currentDialogueSegmentThatPlayerMadeChoiceAt = segmentName;

			std::string nextDialogue = currentPlayerChoice.GetDialogueSegmentToOpenAfterSelected();
			if (nextDialogue != "")
			{
				bool spawnedIt = dialogue->StartDialogueSegment(nextDialogue);
				if (spawnedIt == false)
				{
					ClearWhoTalkingTo();
				}
			}
			else
			{
				ClearWhoTalkingTo();
			}

			QuestEventReqsMet questMetIdx = GetIdxPlayerQuestThatMeetsExpectationsOf();
			if (questMetIdx.m_questIdx != -1)
			{
				m_questManager.TriggerQuest((size_t)questMetIdx.m_questIdx, (size_t)questMetIdx.m_eventIdx);
			}

			if (questMetIdx.m_questIdx != -1)
			{
				m_questManager.PostTriggerQuest((size_t)questMetIdx.m_questIdx, (size_t)questMetIdx.m_eventIdx);
			}
		}
		RunDialogueFinishedFunctionallityIfDialogueSegmentIsFinished(dialogue,
			currentPlayerChoice, playerDialogueChoiceValid);

	}
}


void Player::RunDialogueFinishedFunctionallityIfDialogueSegmentIsFinished(
	Dialogue* dialogue, DialogueChoice& currentPlayerChoice,
	bool playerDialogueChoiceValid)
{
	currentPlayerChoice;
	if (dialogue->GetIfCurrentDialogueSegmentIsFinished())
	{
		//Dialogue Segment is Closing.
		QuestEventReqsMet questMetIdx = GetIdxPlayerQuestThatMeetsExpectationsOf();
		if (questMetIdx.m_questIdx != -1)
		{
			m_questManager.TriggerQuest((size_t)questMetIdx.m_questIdx, (size_t)questMetIdx.m_eventIdx);
		}

		if (!playerDialogueChoiceValid)
		{
			DialogueSegment* segment = dialogue->GetCurrentDialogueSegment();
			if (segment != nullptr && segment->GetDialogueToOpenNext() != "")
			{
				std::string dialogueNext = segment->GetDialogueToOpenNext();

				bool spawnedIt = dialogue->StartDialogueSegment(dialogueNext);
				if (spawnedIt == false)
				{
					ClearWhoTalkingTo();
				}
				if (questMetIdx.m_questIdx != -1)
				{
					m_questManager.PostTriggerQuest((size_t)questMetIdx.m_questIdx,
						(size_t)questMetIdx.m_eventIdx);
				}
				return;
			}
			else
			{
				ClearWhoTalkingTo();
			}
		}
		else
		{
			std::string nextDialogue = currentPlayerChoice.GetDialogueSegmentToOpenAfterSelected();
			if (nextDialogue != "")
			{
				bool spawnedIt = dialogue->StartDialogueSegment(nextDialogue);
				if (spawnedIt == false)
				{
					ClearWhoTalkingTo();
				}

			}
			else
			{
				ClearWhoTalkingTo();
			}
		}

		if (questMetIdx.m_questIdx != -1)
		{
			m_questManager.PostTriggerQuest((size_t)questMetIdx.m_questIdx, (size_t)questMetIdx.m_eventIdx);
		}
	}
}

void Player::PerformNonAction1Update()
{
	ClearPlayerDialogueChoice();
	//Check if can interact with something in front of self.
	Directions4 currentAnimationDirectionIdx = m_entityStatPack.GetCurrentAnimationDirectionIdx();
	Vector2 dir = Directions::GetDirectionForDirectionEnum(currentAnimationDirectionIdx);
	float angle = Directions::GetAngleDegreesForDirectionEnum(currentAnimationDirectionIdx);
	float myRadius = m_circleCollision->GetRadius();
	if (s_BaseGameEntitiesByType != nullptr)
	{
		for (size_t entityTypeIdx = 0; entityTypeIdx < s_BaseGameEntitiesByType->size(); entityTypeIdx++)
		{
			if (entityTypeIdx != AGENT_ENTITY
				&& entityTypeIdx != FEATURE_ENTITY) //Reason doing it this way is might want to add 
				//interactability to other types later.
			{
				continue;
			}
			std::vector<BaseGameEntity*>& entitiesByType = s_BaseGameEntitiesByType->at(entityTypeIdx);
			for (size_t entityIdx = 0; entityIdx < entitiesByType.size(); entityIdx++)
			{
				BaseGameEntity* entity = entitiesByType[entityIdx];
				if(CheckGameEntityForWithinActionRange(entity, myRadius + 0.25f, angle))
				{
					PerformNonAction1OnEntity(entity);
				}
			}
		}
	}
}

bool Player::CheckGameEntityForWithinActionRange(BaseGameEntity* entity, float myRadius, float angle)
{
	if (entity == this
		|| entity == nullptr)
	{
		return false;
	}
	Collision* entityCollision = entity->GetCollision();
	float entityRadius = 0.f;

	Vector2 amtToAdjustPosition = Vector2::vec2_zeros;
	if (entityCollision != nullptr)
	{
		CircleCollision* circle = dynamic_cast<CircleCollision*>(entityCollision);
		if (circle != nullptr)
		{
			entityRadius = circle->GetRadius();
		}
	}
	else if (entity->GetEntityType() == FEATURE_ENTITY)
	{
		entityRadius = 0.5f;
		amtToAdjustPosition = entity->GetRenderOffset() - GetRenderOffset();
	}
	float radiusesCombined = myRadius + entityRadius;
	Vector2 entityPosition = entity->GetPosition() + amtToAdjustPosition;
	Vector2 myPosition = m_entityStatPack.GetPosition();
	Vector2 positionDifference = entityPosition - myPosition;
	Vector2 normalizedPosDiff = positionDifference;
	float lengthOfDiff = normalizedPosDiff.normalize();
	float angleDeg = Directions::GetAngleDegreesForDirectionVector(normalizedPosDiff);
	float angleDiff = Directions::CalcShortestAngleDifferenceFromDegrees(angle, angleDeg);

	bool distanceIsInrange = lengthOfDiff <= radiusesCombined;
	bool withinAngleDifference = abs(angleDiff) <= 30.f;
	bool entityInteractable = entity->GetIsInteractable();
	bool combinedCheck = distanceIsInrange && withinAngleDifference && entityInteractable;

	if (combinedCheck)
	{
		return true;
	}
	return false;
}

void Player::PerformNonAction1OnEntity(BaseGameEntity* entity)
{
	Agent* agent = dynamic_cast<Agent*>(entity);
	Feature* feature = dynamic_cast<Feature*>(entity);
	if (agent != nullptr)
	{
		PerformNonAction1UpdateAgent(entity, agent);
	}
	else if (feature != nullptr)
	{
		PerformNonAction1UpdateFeature(entity, feature);
	}
}

void Player::PerformNonAction1UpdateAgent(BaseGameEntity* entity, Agent* agent)
{
	//Check ifMeetsAnyQuests Requirements for an Event to trigger.
	//bool checkQuests = ;
	//Trigger Pre-Interaction events.
	SetEntityLastInteractedWith(entity);
	QuestEventReqsMet questMetIdx = GetIdxPlayerQuestThatMeetsExpectationsOf();
	if (questMetIdx.m_questIdx != -1)
	{
		m_questManager.TriggerQuest((size_t)questMetIdx.m_questIdx, (size_t)questMetIdx.m_eventIdx);
	}
	std::string dialogueName = agent->GetDialogueToOpenIfSpokenTo();
	if (dialogueName == "")
	{
		return;
	}
	Dialogue* dialogue = Dialogue::SpawnOrGetCentralDialogue();
	if (dialogue != nullptr)
	{
		{
			m_talkingToSomeone = true;
			SetTalkingToTarget(agent);
			agent->SetTalkingToTarget(this);
			agent->SetIsTalkingToSomeone(true);
		}
		bool spawnedIt = dialogue->StartDialogueSegment(dialogueName);
		if (spawnedIt == false)
		{
			ClearWhoTalkingTo();
		}
	}
	//Trigger post interaction events.
	if (questMetIdx.m_questIdx != -1)
	{
		m_questManager.PostTriggerQuest((size_t)questMetIdx.m_questIdx, (size_t)questMetIdx.m_eventIdx);
	}
}

void Player::PerformNonAction1UpdateFeature(BaseGameEntity* entity, Feature* feature)
{
	//Check ifMeetsAnyQuests Requirements for an Event to trigger.
	//bool checkQuests = ;
	//Trigger Pre-Interaction events.
	SetEntityLastInteractedWith(entity);
	QuestEventReqsMet questMetIdx = GetIdxPlayerQuestThatMeetsExpectationsOf();
	if (questMetIdx.m_questIdx != -1)
	{
		m_questManager.TriggerQuest((size_t)questMetIdx.m_questIdx, (size_t)questMetIdx.m_eventIdx);
	}
	feature->EntityInteractWithFeature(this);
	//Trigger post interaction events.
	if (questMetIdx.m_questIdx != -1)
	{
		m_questManager.PostTriggerQuest((size_t)questMetIdx.m_questIdx, (size_t)questMetIdx.m_eventIdx);
	}
}