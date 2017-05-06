#include "Game/GameEntities/Agents/NPCs/NPCAttribute.hpp"
#include "Game/GameEntities/Agents/NPCs/NPCAttributes/NPCAttributeContainer.hpp"
#include "Game/Common/GameDebuggingCommons.hpp"

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//Static Variables
STATIC const NPCAttribute NPCAttribute::s_EmptyAttribute = NPCAttribute();

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//Constructors
NPCAttribute::NPCAttribute()
{

}

NPCAttribute::NPCAttribute(const std::string& npcAttributeName, const std::string& setting)
	: m_NPCAttributeName(npcAttributeName),
	m_Setting(setting)
{

}

NPCAttribute::~NPCAttribute()
{

}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//Setters
void NPCAttribute::SetNPCAttributeName(const std::string& name)
{
	m_NPCAttributeName = name;
}

void NPCAttribute::SetSetting(const std::string& setting)
{
	m_Setting = setting;
}

void NPCAttribute::SetSettingByOwningAttributeContainerIdx(int idx)
{
	m_Setting = NPCAttributeContainer::GetNPCAttributeValueByIndex(m_NPCAttributeName, idx);
}

void NPCAttribute::SetSettingByOwningAttributeViaRandom()
{
	m_Setting = NPCAttributeContainer::GetRandomNPCAttributeValue(m_NPCAttributeName);
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//Getters
const std::string& NPCAttribute::GetNPCAttributeName() const
{
	return m_NPCAttributeName;
}

const std::string& NPCAttribute::GetSetting() const
{
	return m_Setting;
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//Operations
void NPCAttribute::WriteOutEntityImportantDataToStr(std::string& str, int currentIndentationLevel) const
{
	GameDebuggingCommons::AddIndentation(str, currentIndentationLevel);
	str += m_NPCAttributeName + ": " + m_Setting + "\n";
}