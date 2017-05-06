#pragma once

#ifndef HAIRTEMPLATE_HPP
#define HAIRTEMPLATE_HPP
#include "Engine/Core/EngineXMLParser.hpp"
#include "Game/Enums/Directions4.hpp"
#include <string>

class SpriteAnimationSequence;
struct HairColorNameValuePair;
class Hair;
class HairTemplate
{
private:
protected:
	XMLNode m_hairNode;
	std::vector<HairColorNameValuePair> m_hairColors;
	std::vector<std::vector<SpriteAnimationSequence*>> m_sequences;
	std::vector<std::vector<std::string>> m_SequenceNames;
	std::string m_hairName = "";
	std::string m_dialogueHairName = "";
	bool m_isDefaultHair = false;
public:
	//Constructors
	HairTemplate();
	virtual ~HairTemplate();


	//Operations
	Hair* SpawnHair() const;
	Hair* SpawnHair(const std::vector<std::string>& colorChoiceLimits) const;
protected:
	void GetValidColorChoicesBasedOffLimit(const std::vector<std::string>& colorChoiceLimits,
		std::vector<HairColorNameValuePair>& outColorChoices) const;

public:
	//Setters
	void SetAndReadXMLNode(const XMLNode& node);
protected:
	void ReadXMLNodeAttributes(const XMLNode& node);
	void ReadXMLNodeChildren(const XMLNode& node);
	void ReadColorsXMLNode(const XMLNode& node);
	void ReadAnimationXMLNode(const XMLNode& node);
public:
	void SetHairName(const std::string& name);
	void AddSpriteAnimationSequence(SpriteAnimationSequence* sequence, Directions4 dir, int idx,
		const std::string& str = "");

	//Getters
	void WriteGeneratorToString(std::string& str, int indentationLevel) const;
	const XMLNode& GetXMLNode() const;
	const std::string& GetHairName() const;
	bool IsDefaultHair() const;
};
#endif