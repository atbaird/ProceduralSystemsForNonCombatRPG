#include "Game/Collision/Collision.hpp"
#include "Game/GameEntities/BaseGameEntity.hpp"


//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//Constructors
Collision::Collision(CollisionType type)
	: m_type(type)
{

}

Collision::~Collision()
{

}

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//Setters
void Collision::SetGameEntity(BaseGameEntity* entity)
{
	m_entity = entity;
}



void Collision::ClearIntersectingEntities()
{
	m_allIntersectingEntities.clear();
}

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//Getters
CollisionType Collision::GetCollisionType() const
{
	return m_type;
}


const BaseGameEntity* Collision::GetEntity() const
{
	return m_entity;
}

BaseGameEntity* Collision::GetEditableEntity() const
{
	return m_entity;
}


const std::vector<const BaseGameEntity*>* Collision::GetAllIntersectingEntities() const
{
	return (std::vector<const BaseGameEntity*>*)&m_allIntersectingEntities;
}

std::vector<BaseGameEntity*>* Collision::GetEditableAllIntersectingEntities() const
{
	return &m_allIntersectingEntities;
}