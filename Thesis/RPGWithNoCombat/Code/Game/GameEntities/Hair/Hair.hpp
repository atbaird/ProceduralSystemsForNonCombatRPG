#pragma once

#ifndef HAIR_HPP
#define HAIR_HPP
#include "Game/GameEntities/BaseGameEntity.hpp"

struct XMLNode;

class Hair : public BaseGameEntity
{
private:
protected:
	std::string m_hairColorName = "default";
	std::string m_dialogueHairName = "default";
public:
	//Constructor
	Hair(std::string hairName = "", const SpriteResource* resource = nullptr, Directions4 defaultDirection = SOUTH);
	Hair(std::string hairName, const XMLNode& node);
	virtual ~Hair();

protected:

public:

	//Setters
	void SetHairColorName(const std::string& hairColorName);
	void SetDialogueHairName(const std::string& hairName);

	//Getters
	const std::string& GetHairColorName() const;
	const std::string& GetDeialogueHairName() const;

	//Operations
	virtual void WriteOutEntityImportantDataToStr(std::string& str, int currentIndentationLevel = 0) const override;

	//Updates and Render
	virtual void Update(const float& dt) override;
};

#endif