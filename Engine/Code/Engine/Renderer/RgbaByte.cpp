#include "Engine/Renderer/RgbaByte.hpp"

//----------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------
//Static Variables
STATIC const RgbaByte RgbaByte::s_White = RgbaByte(255, 255, 255, 255);
STATIC const RgbaByte RgbaByte::s_Black = RgbaByte(0, 0, 0, 255);
STATIC const RgbaByte RgbaByte::s_Blue = RgbaByte(0,0,255,255);
STATIC const RgbaByte RgbaByte::s_Gray = RgbaByte(127, 127, 127,255);
STATIC const RgbaByte RgbaByte::s_Gray6 = RgbaByte(153, 153, 153,255);
STATIC const RgbaByte RgbaByte::s_lightGray = RgbaByte(204, 204, 204,255);
STATIC const RgbaByte RgbaByte::s_Red = RgbaByte(255,0,0,255);
STATIC const RgbaByte RgbaByte::s_Green = RgbaByte(0,255,0,255);
STATIC const RgbaByte RgbaByte::s_Yellow = RgbaByte(255,255,0,255);

//----------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------
//constructors
RgbaByte::RgbaByte(const unsigned char& red, const unsigned char& green, const unsigned char& blue, const unsigned char& alpha)
{
	Red = (unsigned char)InternalColorClamp((unsigned int)red);
	Green = (unsigned char)InternalColorClamp((unsigned int)green);
	Blue = (unsigned char)InternalColorClamp((unsigned int)blue);
	Alpha = (unsigned char)InternalColorClamp((unsigned int)alpha);
}

RgbaByte::RgbaByte(const IntVector3& rgb, const unsigned char& alpha)
{
	Red = (unsigned char)InternalColorClamp(rgb.x);
	Green = (unsigned char)InternalColorClamp(rgb.y);
	Blue = (unsigned char)InternalColorClamp(rgb.z);
	Alpha = (unsigned char)InternalColorClamp((unsigned int)alpha);
}

unsigned int RgbaByte::InternalColorClamp(unsigned int col)
{
	if (col > 255)
	{
		return 255;
	}
	return col;
}

//----------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------
//setters
void RgbaByte::SetRGBA(const unsigned char& in_r, const unsigned char& in_g, const unsigned char& in_b, const unsigned char& in_a)
{
	Red = (unsigned char)InternalColorClamp((unsigned int)in_r);
	Green = (unsigned char)InternalColorClamp((unsigned int)in_g);
	Blue = (unsigned char)InternalColorClamp((unsigned int)in_b);
	Alpha = (unsigned char)InternalColorClamp((unsigned int)in_a);
}

void RgbaByte::SetRGB(const unsigned char& in_r, const unsigned char& in_g, const unsigned char& in_b)
{
	Red = (unsigned char)InternalColorClamp((unsigned int)in_r);
	Green = (unsigned char)InternalColorClamp((unsigned int)in_g);
	Blue = (unsigned char)InternalColorClamp((unsigned int)in_b);
}

void RgbaByte::setAlpha(const unsigned char& in_a)
{
	Alpha = (unsigned char)InternalColorClamp((unsigned int)in_a);
}

//----------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------
//getters
void RgbaByte::GetRGBA(unsigned char& out_r, unsigned char& out_g, unsigned char& out_b, unsigned char& out_a) const
{
	out_r = Red;
	out_g = Green;
	out_b = Blue;
	out_a = Alpha;
}

const IntVector3 RgbaByte::GetRGBByte() const
{
	return IntVector3(Red, Green, Blue);
}

//----------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------
//operators
const bool RgbaByte::operator==(const RgbaByte& other) const
{
	if (other.Red == Red
		&& other.Green == Green
		&& other.Blue == Blue
		&& other.Alpha == Alpha)
	{
		return true;
	}
	return false;
}

const bool RgbaByte::operator!=(const RgbaByte& other) const
{
	if (other.Red != Red
		|| other.Green != Green
		|| other.Blue != Blue
		|| other.Alpha != Alpha)
	{
		return true;
	}
	return false;
}

const RgbaByte RgbaByte::operator-(const RgbaByte& other) const
{
	return RgbaByte(Red - other.Red, Green - other.Green, Blue - other.Blue, Alpha - other.Alpha);
}

const RgbaByte RgbaByte::operator+(const RgbaByte& other) const
{
	return RgbaByte(Red + other.Red, Green + other.Green, Blue + other.Blue, Alpha + other.Alpha);
}

const RgbaByte RgbaByte::operator*(const unsigned char& mult) const
{
	return RgbaByte(Red * mult, Green * mult, Blue * mult, Alpha * mult);
}

const RgbaByte RgbaByte::operator/(const unsigned char& div) const
{
	return RgbaByte(Red / div, Green / div, Blue / div, Alpha / div);
}