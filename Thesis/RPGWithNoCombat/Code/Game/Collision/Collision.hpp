#pragma once

#ifndef COLLISION_HPP
#define COLLISION_HPP
#include "Engine/Math/Vector2.hpp"
#include "Game/Collision/CollisionType.hpp"
#include <vector>


class BaseGameEntity;

class Collision
{
private:
protected:
	mutable std::vector<BaseGameEntity*> m_allIntersectingEntities;
	BaseGameEntity* m_entity = nullptr;
	CollisionType m_type = INVALID_COLLISION;
public:
	//Constructors
	Collision(CollisionType type);
	virtual ~Collision();

	//Setters
	virtual void SetGameEntity(BaseGameEntity* entity);
	virtual void ClearIntersectingEntities();

	//Getters
	virtual CollisionType GetCollisionType() const;
	virtual const BaseGameEntity* GetEntity() const;
	virtual BaseGameEntity* GetEditableEntity() const;
	virtual const std::vector<const BaseGameEntity*>* GetAllIntersectingEntities() const;
	virtual std::vector<BaseGameEntity*>* GetEditableAllIntersectingEntities() const;

	//operations
	virtual bool CheckIfPointInCollision(const Vector2& point, const Vector2& myPos) const = 0;
};
#endif