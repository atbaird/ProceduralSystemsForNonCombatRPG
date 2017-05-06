#pragma once

#ifndef BASEGAMEENTITYSTATPACK_HPP
#define BASEGAMEENTITYSTATPACK_HPP
#include "Engine/Math/IntVector2.hpp"
#include "Engine/Math/Vector2.hpp"
#include "Game/Enums/Directions4.hpp"
#include "Game/Enums/GameEntityType.hpp"

#define TILE_COORDS IntVector2

struct BaseGameEntityStatPack
{
private:
protected:
	//Variables
	Vector2 m_position = Vector2(0.f, 0.f);
	Vector2 m_renderOffset = Vector2(0.f, 0.f);
	TILE_COORDS m_blTileCoords = TILE_COORDS(0, 0);
	float m_currentAnimationPlaySpeed = 1.f;
	float m_angleEntityFacing = 0.f;
	float m_angleDegreesEntityFacing = 0.f;
	GameEntityType m_entityType = INVALID_ENTITY;
	Directions4 m_currentAnimationDirectionIdx = SOUTH;
	int m_currentAnimationFrameIdx = -1;
	int m_currentAnimationIdx = 0;
	int m_colorSchemeChoice = 0;
public:
	//Constructors
	BaseGameEntityStatPack(Directions4 defaultDirection = SOUTH, GameEntityType type = INVALID_ENTITY);
	virtual ~BaseGameEntityStatPack();

	//Setters
	void SetPosition(const Vector2& position);
	void SetRenderOffset(const Vector2& offset);
	void SetBLTileCoords(const TILE_COORDS& coords);
	void SetCurrentAnimationPlaySpeed(float speed);
	void SetAngleEntityFacing(float angle);
	void SetAngleDegreesEntityFacing(float angleDeg);
	void SetGameEntityType(GameEntityType type);
	void SetCurrentAnimationDirectionIdx(Directions4 dir);
	void SetCurrentAnimationFrameIdx(int currentAnimationFrameIdx);
	void SetCurrentAnimationIdx(int currentAnimationIdx);
	void SetColorSchemeChoice(int color);

	//Getters
	const Vector2& GetPosition() const;
	const Vector2& GetRenderOffset() const;
	const TILE_COORDS& GetBLTileCoords() const;
	float GetCurrentAnimationPlaySpeed() const;
	float GetAngleEntityFacing() const;
	float GetAngleDegreesEntityFacing() const;
	GameEntityType GetGameEntityType() const;
	Directions4 GetCurrentAnimationDirectionIdx() const;
	int GetCurrentAnimationFrameIdx() const;
	int GetCurrentAnimationIdx() const;
	int GetColorSchemeChoice() const;
};

#endif