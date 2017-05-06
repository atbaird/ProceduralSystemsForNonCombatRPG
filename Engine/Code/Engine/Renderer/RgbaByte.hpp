#pragma once

#ifndef RGBABYTE_HPP
#define RGBABYTE_HPP
#include "Engine/Math/IntVector3.hpp"
#include "Engine/Math/Vector3.hpp"
#include <string>

#define STATIC
class RgbaByte
{
public:
	//Static Variables
	static const RgbaByte s_White;
	static const RgbaByte s_Black;
	static const RgbaByte s_Blue;
	static const RgbaByte s_Gray;
	static const RgbaByte s_Gray6;
	static const RgbaByte s_lightGray;
	static const RgbaByte s_Red;
	static const RgbaByte s_Green;
	static const RgbaByte s_Yellow;

	//variables
	unsigned char Red = 0;
	unsigned char Green = 0;
	unsigned char Blue = 0;
	unsigned char Alpha = 255;

	//constructors
	RgbaByte(const unsigned char& red = 0, const unsigned char& green = 0, const unsigned char& blue = 0, 
		const unsigned char& alpha = 255);
	RgbaByte(const IntVector3& rgb, const unsigned char& alpha = 255);
protected:
	unsigned int InternalColorClamp(unsigned int col);
public:

	//Setters
	void SetRGBA(const unsigned char& in_r, const unsigned char& in_g, const unsigned char& in_b, const unsigned char& in_a);
	void SetRGB(const unsigned char& in_r, const unsigned char& in_g, const unsigned char& in_b);
	void setAlpha(const unsigned char& in_a);

	//Getters
	void GetRGBA(unsigned char& out_r, unsigned char& out_g, unsigned char& out_b, unsigned char& out_a) const;
	const IntVector3 GetRGBByte() const;


	//operators
	const bool operator==(const RgbaByte& other) const;
	const bool operator!=(const RgbaByte& other) const;
	const RgbaByte operator-(const RgbaByte& other) const;
	const RgbaByte operator+(const RgbaByte& other) const;
	const RgbaByte operator*(const unsigned char& mult) const;
	const RgbaByte operator/(const unsigned char& div) const;

};

#endif