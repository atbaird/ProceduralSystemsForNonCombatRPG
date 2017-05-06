#pragma once

#ifndef SPRITEVERTWITHMULTISPRITES_HPP
#define SPRITEVERTWITHMULTISPRITES_HPP
#include "Engine/Math/Vector2.hpp"
#include "Engine/Renderer/Rgba.hpp"

#define STATIC

struct SpriteVertWithMultiSprites
{
public:
	//Variables
	Vector2 m_pos;
	Vector2 m_texCoords;
	Rgba m_color; //uniform sampler2D gTexDiffuse
	unsigned int m_spriteIdx = 0;

	//Static Variables
	static const SpriteVertWithMultiSprites s_EmptyVert;

	//Constructors
	SpriteVertWithMultiSprites(const Vector2& pos = Vector2::vec2_zeros,
		const Rgba& tint = Rgba::s_White,
		const Vector2& uv = Vector2::vec2_zeros,
		unsigned int spriteIdx = 0);

	//Operators
	const bool operator==(const SpriteVertWithMultiSprites& other) const;
	const bool operator!=(const SpriteVertWithMultiSprites& other) const;
};

#endif