#pragma once

#ifndef GAMEENTITY_HPP
#define GAMEENTITY_HPP
#include "Engine/Renderer/3DModelling/MeshBuilder.hpp"
#include "Engine/2D/SpriteAnimationSequence.hpp"
#include "Engine/2D/SpriteResource.hpp"
#include "Engine/2D/SpriteVert.hpp"
#include "Engine/EventSystem/EventSystem.hpp"
#include "Game/Collision/Collision.hpp"
#include "Game/Enums/AnimationsIdxes.hpp"
#include "Game/GameEntities/BaseGameEntityStatPack.hpp"
#include "Game/Debugging/DeveloperDebugInfoBox.hpp"
#include "Game/Debugging/DeveloperDebugInfoTextBlurb.hpp"

class Map;

#define TILE_COORDS IntVector2

class Quest;
class BaseGameEntity
{
private:
protected:
	//Static variables
	static std::vector<std::vector<BaseGameEntity*>>* s_BaseGameEntitiesByType;
	static std::vector<BaseGameEntity*>* s_AllNPCs;
	static std::vector<BaseGameEntity*>* s_AllFeatures;
	static BaseGameEntity* s_Player;
	static bool BaseGameEntity::s_tilePlayerHasTopLayer;

	//Variables
	Material m_material;
	Mesh m_mesh;
	Matrix44 m_modelMatrix;
	BaseGameEntityStatPack m_entityStatPack;
	std::vector<SpriteVert> m_Model;
	std::vector<unsigned int> m_indices;
	std::vector<std::vector<SpriteAnimationSequence*>> m_sequences;
	std::string m_name = "";
	std::string m_generatorUsed = "";
	const SpriteResource* m_currentSprite = nullptr;
	const SpriteResource* m_overridingSprite = nullptr;
	SpriteAnimationSequence* m_currentAnimation = nullptr;
	SpriteAnimationSequence* m_overridingAnimation = nullptr;
	Collision* m_collision = nullptr;
	float m_currentAnimationPlayTime = 0.f;
	int m_DebugSpriteIdxInAnimationOverride = -1;
	bool m_UpdateMesh = true;
	bool m_interactable = false;
public:
	static const int s_MaxColorChoiceOptions;
	//Constructors
	BaseGameEntity(GameEntityType type = INVALID_ENTITY, const SpriteResource* resource = nullptr, 
		Directions4 defaultDirection = SOUTH);
	virtual ~BaseGameEntity();

protected:
	void CommonConstructorSetup(bool registerThis);
	void LoadInDefaultData();
public:

	//Updates and Render
	virtual void Update(const float& dt);
protected:
	virtual void AnimationUpdate(const float& dt);
	virtual void MeshUpdate(const float& dt);
public:
	virtual void Render() const;

	//Debug Controls
	virtual	void GetEntityInfoForDeveloperDebug(std::vector<DeveloperDebugInfoBox>& infoBoxes) const;
	virtual void WriteDeveloperDebugInfo(DeveloperDebugInfoBox& info) const;
	virtual void SetDebugSpriteIdxInAnimationOverride(int overrid);
	virtual void DebugCycleThroughWalkingDirections();
	virtual void DebugCycleThroughAnimationIdx();
	virtual void DebugCycleThroughSpriteIdxInAnimation();

	//Setters
	virtual void SetGeneratorUsed(const std::string& generatorUsed);
	virtual void SetPosition(const Vector2& position);
	virtual void SetPosition(const Vector2& position, const Map* map);
	virtual void SetTint(const Rgba& tint);
	virtual void SetOveridingSequence(SpriteAnimationSequence* sequence);
	virtual void SetColorSchemeChoice(int colorChoice);
	virtual void ResetOveridingSequence();
	virtual void AddAnimationSequence(SpriteAnimationSequence* sequence, Directions4 dir, int idx);
	virtual void ChangeSpriteResource(const SpriteResource* resource);
	virtual void ChangeCurrentAnimationPlaying(Directions4 animationDirIdx, int animationIdx);
	virtual void SetAnimationPlaySpeed(float playSpeed);
	virtual void SetAnimationPlayTime(float playTime);
	virtual void SetRenderOffset(const Vector2& offset) { m_entityStatPack.SetRenderOffset(offset); };
	virtual void SetEntityType(GameEntityType type) { m_entityStatPack.SetGameEntityType(type); };
	virtual void SetAgentName(const std::string& name) { m_name = name; };

	//Getters
	virtual std::string GetEntityTypeAsString() const;
	virtual const std::string& GetGeneratorUsed() const { return m_generatorUsed; };
	virtual const std::string& GetName() const { return m_name; };
	virtual Collision* GetCollision() const { return m_collision; };
	virtual GameEntityType GetEntityType() const { return m_entityStatPack.GetGameEntityType(); };
	virtual const TILE_COORDS& GetTileCoords() const { return m_entityStatPack.GetBLTileCoords(); };
	virtual const Vector2& GetPosition() const { return m_entityStatPack.GetPosition(); };
	virtual const Vector2& GetRenderOffset() const { return m_entityStatPack.GetRenderOffset(); };
	virtual Directions4 GetCurrentAnimationsIdx() const { return m_entityStatPack.GetCurrentAnimationDirectionIdx(); };
	virtual int GetCurrentAnimationIdx() const { return m_entityStatPack.GetCurrentAnimationIdx(); };
	virtual int GetDebugSpriteIdxInAnimationOverride() const { return m_DebugSpriteIdxInAnimationOverride; };

	virtual bool GetAmInvalid() const { return m_entityStatPack.GetGameEntityType() == INVALID_ENTITY; };
	virtual bool GetAmAgent() const { return m_entityStatPack.GetGameEntityType() == AGENT_ENTITY; };
	virtual bool GetAmItem() const { return m_entityStatPack.GetGameEntityType() == ITEM_ENTITY; };
	virtual bool GetIsInteractable() const { return m_interactable; };

	//Operations
	virtual void WriteOutEntityImportantDataToStr(std::string& str, int currentIndentationLevel = 0) const;
	virtual void SignUpForEvents();
	virtual void UnsignUpForEvents();
	static void WriteBaseGameEntityToNamedProperties(NamedProperties& params, BaseGameEntity* entity);
	static BaseGameEntity* ReadBaseGameEntityFromNameProperties(NamedProperties& params);
	static BaseGameEntity* ReadBaseGameEntityFromNameProperties(NamedProperties& params, GameEntityType& type);

	//Static methods
	static void GetListOfQuestsThatAllReferenceGivenEntity(const BaseGameEntity* entity, 
		std::vector<Quest*>& questReferences);
	static void TotalClearGameEntities();
	static void SetPointerToPlayer(BaseGameEntity* player);
	static const BaseGameEntity* GetPointerToPlayer();
	static BaseGameEntity* GetEditablePointerToPlayer();
	static void SetBaseGameEntitiesPointer(std::vector<std::vector<BaseGameEntity*>>* baseEntitiesByType);
	static void SetAllNPCsPointer(std::vector<BaseGameEntity*>* baseEntitiesByType);
	static void SetAllFeaturesPointer(std::vector<BaseGameEntity*>* baseEntitiesByType);
	static void TestIfPlayerIsOnTileWithTransparentTop();
	static bool GetIfPlayerIsInTileWithTransparentTop();
	static const std::vector<BaseGameEntity*>* GetAllFeaturesPtr();
	static const std::vector<BaseGameEntity*>* GetAllNPCsPtr();

protected:
};
#endif