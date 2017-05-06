#include "Game/GameEntities/BaseGameEntityStatPack.hpp"

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//Constructors
BaseGameEntityStatPack::BaseGameEntityStatPack(Directions4 defaultDirection, GameEntityType type)
	: m_currentAnimationDirectionIdx(defaultDirection),
	m_entityType(type)
{

}

BaseGameEntityStatPack::~BaseGameEntityStatPack()
{

}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//Setters
void BaseGameEntityStatPack::SetPosition(const Vector2& position)
{
	m_position = position;
}

void BaseGameEntityStatPack::SetRenderOffset(const Vector2& offset)
{
	m_renderOffset = offset;
}

void BaseGameEntityStatPack::SetBLTileCoords(const TILE_COORDS& coords)
{
	m_blTileCoords = coords;
}

void BaseGameEntityStatPack::SetCurrentAnimationPlaySpeed(float speed)
{
	m_currentAnimationPlaySpeed = speed;
}

void BaseGameEntityStatPack::SetAngleEntityFacing(float angle)
{
	m_angleEntityFacing = angle;
}

void BaseGameEntityStatPack::SetAngleDegreesEntityFacing(float angleDeg)
{
	m_angleDegreesEntityFacing = angleDeg;
}

void BaseGameEntityStatPack::SetGameEntityType(GameEntityType type)
{
	m_entityType = type;
}

void BaseGameEntityStatPack::SetCurrentAnimationDirectionIdx(Directions4 dir)
{
	m_currentAnimationDirectionIdx = dir;
}

void BaseGameEntityStatPack::SetCurrentAnimationFrameIdx(int currentAnimationFrameIdx)
{
	m_currentAnimationFrameIdx = currentAnimationFrameIdx;
}

void BaseGameEntityStatPack::SetCurrentAnimationIdx(int currentAnimationIdx)
{
	m_currentAnimationIdx = currentAnimationIdx;
}

void BaseGameEntityStatPack::SetColorSchemeChoice(int color)
{
	m_colorSchemeChoice = color;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//Getters
const Vector2& BaseGameEntityStatPack::GetPosition() const
{
	return m_position;
}

const Vector2& BaseGameEntityStatPack::GetRenderOffset() const
{
	return m_renderOffset;
}

const TILE_COORDS& BaseGameEntityStatPack::GetBLTileCoords() const
{
	return m_blTileCoords;
}

float BaseGameEntityStatPack::GetCurrentAnimationPlaySpeed() const
{
	return m_currentAnimationPlaySpeed;
}

float BaseGameEntityStatPack::GetAngleEntityFacing() const
{
	return m_angleEntityFacing;
}

float BaseGameEntityStatPack::GetAngleDegreesEntityFacing() const
{
	return m_angleDegreesEntityFacing;
}

GameEntityType BaseGameEntityStatPack::GetGameEntityType() const
{
	return m_entityType;
}

Directions4 BaseGameEntityStatPack::GetCurrentAnimationDirectionIdx() const
{
	return m_currentAnimationDirectionIdx;
}

int BaseGameEntityStatPack::GetCurrentAnimationFrameIdx() const
{
	return m_currentAnimationFrameIdx;
}

int BaseGameEntityStatPack::GetCurrentAnimationIdx() const
{
	return m_currentAnimationIdx;
}

int BaseGameEntityStatPack::GetColorSchemeChoice() const
{
	return m_colorSchemeChoice;
}