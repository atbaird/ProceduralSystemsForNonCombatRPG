#include "Engine/2D/SpriteVertWithMultiSprites.hpp"

STATIC const SpriteVertWithMultiSprites SpriteVertWithMultiSprites::s_EmptyVert 
    = SpriteVertWithMultiSprites(Vector2(0.f,0.f),
	Rgba(0.f, 0.f, 0.f, 1.f), Vector2(0.f, 0.f), 0);

//--------------------------------------------------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------
//Constructors
SpriteVertWithMultiSprites::SpriteVertWithMultiSprites(const Vector2& pos,
	const Rgba& tint,
	const Vector2& uv,
	unsigned int spriteIdx)
	: m_pos(pos),
	m_color(tint),
	m_texCoords(uv),
	m_spriteIdx(spriteIdx)
{
}

//--------------------------------------------------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------
//Operators
const bool SpriteVertWithMultiSprites::operator==(const SpriteVertWithMultiSprites& other) const
{
	if (other.m_color == m_color
		&& other.m_pos == m_pos
		&& other.m_texCoords == m_texCoords
		&& other.m_spriteIdx == m_spriteIdx)
	{
		return true;
	}
	return false;
}

const bool SpriteVertWithMultiSprites::operator!=(const SpriteVertWithMultiSprites& other) const
{
	if (other.m_color != m_color
		|| other.m_pos != m_pos
		|| other.m_texCoords != m_texCoords
		|| other.m_spriteIdx != m_spriteIdx)
	{
		return true;
	}
	return false;
}