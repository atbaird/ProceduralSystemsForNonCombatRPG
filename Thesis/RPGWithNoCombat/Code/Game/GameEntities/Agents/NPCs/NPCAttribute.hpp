#pragma once

#ifndef NPCATTRIBUTE_HPP
#define NPCATTRIBUTE_HPP
#include <string>

class NPCAttribute
{
private:
protected:
	//Variables
	std::string m_NPCAttributeName = "";
	std::string m_Setting = "";
public:
	static const NPCAttribute s_EmptyAttribute;
	//Constructors
	NPCAttribute();
	NPCAttribute(const std::string& npcAttributeName, const std::string& setting = "");
	virtual ~NPCAttribute();

	//Setters
	void SetNPCAttributeName(const std::string& name);
	void SetSetting(const std::string& setting);
	void SetSettingByOwningAttributeContainerIdx(int idx);
	void SetSettingByOwningAttributeViaRandom();

	//Getters
	const std::string& GetNPCAttributeName() const;
	const std::string& GetSetting() const;

	//Operations
	void WriteOutEntityImportantDataToStr(std::string& str, int currentIndentationLevel = 0) const;
};

#endif