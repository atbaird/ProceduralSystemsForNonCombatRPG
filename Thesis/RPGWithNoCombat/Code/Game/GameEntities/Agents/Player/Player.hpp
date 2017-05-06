#pragma once

#ifndef PLAYER_HPP
#define PLAYER_HPP
#include "Game/GameEntities/Agents/Agent.hpp"
#include "Game/GameEntities/Agents/Player/PlayerQuestManager.hpp"

class Feature;
class Dialogue;
class DialogueChoice;
class Player : public Agent
{
private:
protected:
	PlayerQuestManager m_questManager;
	std::string m_currentDialogueSegmentThatPlayerMadeChoiceAt = "";
	std::string m_lastDialogueSegmentThatPlayerMadeChoiceAt = "";
	std::string m_currentPlayerDialogueChoicePicked = "";
	std::string m_lastPlayerDialogueChoicePicked = "";
	int m_currentDialogueChoiceIdx = -1;
	bool m_MoveNorthKeyPressed = false;
	bool m_MoveSouthKeyPressed = false;
	bool m_MoveNorthKeyHeld = false;
	bool m_MoveSouthKeyHeld = false;
	bool m_action1KeyIsDown = false;
	bool m_action1KeyIsHeld = false;
	bool m_performAction1Action = false;
public:
	//Constructors
	Player(const SpriteResource* resource = nullptr);
	virtual ~Player();

	//Setters
	void AddQuest(Quest* quest);
	void AddQuestByTitle(const std::string& name);
	void SetPlayerDialogueChoice(const std::string& dialogueChoice);
	void SetPlayerDialogueChoiceIdx(int idx);
	void ClearPlayerDialogueChoice();

	//Getters
	const PlayerQuestManager& GetPlayerQuestManager() const;
	const PlayerQuestManager* GetPlayerQuestManagerPtr() const;
	PlayerQuestManager* GetEditablePlayerQuestManagerPtr();
	QuestEventReqsMet GetIdxPlayerQuestThatMeetsExpectationsOf();
	const std::string& GetPlayerChoiceSegmentName() const;
	const std::string& GetPlayerDialogueChoicePicked() const;
	const std::string& GetLastPlayerChoiceSegmentName() const;
	const std::string& GetLastPlayerDialgoueChoicePicked() const;
	int GetPlayerDialogueChoiceIdx() const;

	//Operations
	virtual void WriteDeveloperDebugInfo(DeveloperDebugInfoBox& info) const override;
	virtual void WriteOutEntityImportantDataToStr(std::string& str, int currentIndentationLevel = 0) const override;
	void WriteOutAllQuestsToString(std::string& str, int currentIndentationLevel = 0) const;
	bool WriteOutQuestDataByNameToString(std::string& str, const std::string& questName,
		int currentIndentationLevel = 0) const;

	//Updates
	virtual void Update(const float& dt) override;
protected:
	virtual void ReadControls(Vector2& movementVector);
	virtual void ReadControlsDialogueMovement(const Vector2& movementVector);
	virtual void HandleDialogueInput();
	virtual void PerformAction1Update();
	virtual void RunDialogueFinishedFunctionallityIfDialogueSegmentIsFinished(
		Dialogue* dialogue, DialogueChoice& currentPlayerChoice,
		bool playerDialogueChoiceValid);
	virtual void PerformNonAction1Update();
	virtual bool CheckGameEntityForWithinActionRange(BaseGameEntity* entity, float myRadius, float angle);
	virtual void PerformNonAction1OnEntity(BaseGameEntity* entity);
	virtual void PerformNonAction1UpdateAgent(BaseGameEntity* entity, Agent* agent);
	virtual void PerformNonAction1UpdateFeature(BaseGameEntity* entity, Feature* feature);
};
#endif