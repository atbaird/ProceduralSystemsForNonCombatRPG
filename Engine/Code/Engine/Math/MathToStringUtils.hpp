#pragma once

#ifndef MATHTOSTRINGUTILS_HPP
#define MATHTOSTRINGUTILS_HPP
#include <string>
#define STATIC
class Vector2;
class Vector3;
class Vector4;
class IntVector2;
class IntVector3;
class UnsignedIntVector4;
class AABB2;
class Rgba;

struct MathToStringUtils
{
	static const std::string ToString(const Vector2& vec2);
	static const std::string ToString(const Vector3& vec3);
	static const std::string ToString(const Vector4& vec4);
	static const std::string ToString(const IntVector2& vec2);
	static const std::string ToString(const IntVector3& vec3);
	static const std::string ToString(const UnsignedIntVector4& vec4);
	static const std::string ToString(const AABB2& aabb2);
	static const std::string ToString(const Rgba& rgba);
};
#endif