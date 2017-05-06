#include "Engine/Math/MathToStringUtils.hpp"
#include "Engine/Math/Vector2.hpp"
#include "Engine/Math/Vector3.hpp"
#include "Engine/Math/Vector4.hpp"
#include "Engine/Math/IntVector2.hpp"
#include "Engine/Math/IntVector3.hpp"
#include "Engine/Math/UnsignedIntVector4.hpp"
#include "Engine/Math/AABB2.hpp"
#include "Engine/Renderer/Rgba.hpp"


STATIC const std::string MathToStringUtils::ToString(const Vector2& vec2)
{
	std::string str = std::to_string(vec2.x) + ","+ std::to_string(vec2.y);
	return str;
}

STATIC const std::string MathToStringUtils::ToString(const Vector3& vec3)
{
	std::string str = std::to_string(vec3.x) + "," + std::to_string(vec3.y) + "," + std::to_string(vec3.z);
	return str;
}

STATIC const std::string MathToStringUtils::ToString(const Vector4& vec4)
{
	std::string str = std::to_string(vec4.x) + "," + std::to_string(vec4.y) 
		+ "," + std::to_string(vec4.z) + "," + std::to_string(vec4.w);
	return str;
}

STATIC const std::string MathToStringUtils::ToString(const IntVector2& vec2)
{
	std::string str = std::to_string(vec2.x) + "," + std::to_string(vec2.y);
	return str;
}

STATIC const std::string MathToStringUtils::ToString(const IntVector3& vec3)
{
	std::string str = std::to_string(vec3.x) + "," + std::to_string(vec3.y) + "," + std::to_string(vec3.z);
	return str;
}

STATIC const std::string MathToStringUtils::ToString(const UnsignedIntVector4& vec4)
{
	std::string str = std::to_string(vec4.x) + "," + std::to_string(vec4.y)
		+ "," + std::to_string(vec4.z) + "," + std::to_string(vec4.w);
	return str;
}

STATIC const std::string MathToStringUtils::ToString(const AABB2& aabb2)
{
	std::string str = ToString(aabb2.mins) + "," + ToString(aabb2.maxs);
	return str;
}

STATIC const std::string MathToStringUtils::ToString(const Rgba& rgba)
{
	std::string str = std::to_string(rgba.Red) + "," + std::to_string(rgba.Green)
		+ "," + std::to_string(rgba.Blue) + "," + std::to_string(rgba.Alpha);
	return str;
}