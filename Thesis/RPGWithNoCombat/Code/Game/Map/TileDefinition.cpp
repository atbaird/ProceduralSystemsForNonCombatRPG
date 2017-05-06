#include "Game/Map/TileDefinition.hpp"
#include "Game/TheGame.hpp"
#include "Game/Common/MapCommonVars.hpp"
#include "Engine/Math/Vector3.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/2D/SpriteResourceHandler.hpp"
#include "Game/Map/TileEvent/TileEventFunctions.hpp"
#include "Engine/Core/FileUtils.hpp"
#include "Engine/Math/ExtendedMath.hpp"
#include "Engine/Core/Performance/Performance.hpp"
#include "Engine/Math/MathToStringUtils.hpp"
#include "Game/Common/GameDebuggingCommons.hpp"

//------------------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------------------
//static variables
STATIC std::vector<TileDefinition>* TileDefinition::s_definitions = nullptr;
STATIC TileDefinitionLoadingData* TileDefinition::s_loadingData = nullptr;
STATIC int TileDefinition::s_SolidMask = 1 << IS_SOLID;
STATIC int TileDefinition::s_LiquidMask = 1 << IS_LIQUID;
//------------------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------------------
//Constructors
TileDefinition::TileDefinition()
{
	ClearDebugName();
	m_name = "default";
}


TileDefinition::TileDefinition(const XMLNode& node)
{
	bool nameSet = false;
	ClearDebugName();
	ReadAttributesFromTileDefNode(node, nameSet);
	if (nameSet == false)
	{
		m_name = "default";
	}
	ReadChildrenFromTileDefNode(node);
}

TileDefinition::~TileDefinition()
{
}


void TileDefinition::ReadAttributesFromTileDefNode(const XMLNode& node, bool& nameSet)
{
	for (int childIdx = 0; childIdx < node.nAttribute(); childIdx++)
	{
		XMLAttribute attr = node.getAttribute(childIdx);
		std::string attrName = MakeLower(attr.lpszName);
		if ((attrName == CommonMapVars::s_debugNameVar)
			|| attrName == "name")
		{
			m_name = MakeLower(attr.lpszValue);
			nameSet = true;
		}
		else if ((attrName == CommonMapVars::s_tiledefIdxVar))
		{
			m_tileDefIdx = (unsigned int)atoi(attr.lpszValue);
			m_tileIdxSet = true;
		}
		else if ((attrName == CommonMapVars::s_MapCodeVar))
		{
			std::vector<std::string> vars = ParseString(std::string(attr.lpszValue), ',');
			if (vars.size() > 0)
			{
				m_mapImageCode.x = (float)atof(vars[0].c_str());
			}
			if (vars.size() > 1)
			{
				m_mapImageCode.y = (float)atof(vars[1].c_str());
			}
			if (vars.size() > 2)
			{
				m_mapImageCode.z = (float)atof(vars[2].c_str());
			}
		}
		else if ((attrName == CommonMapVars::s_SolidVar))
		{
			if (SimpleStrCmpLower(attr.lpszValue, "true")
				|| SimpleStrCmpLower(attr.lpszValue, "t")
				|| SimpleStrCmpLower(attr.lpszValue, "1"))
			{
				m_solid = true;
			}
		}
		else if ( (attrName == CommonMapVars::s_liquidVar))
		{
			if (SimpleStrCmpLower(attr.lpszValue, "true")
				|| SimpleStrCmpLower(attr.lpszValue, "t")
				|| SimpleStrCmpLower(attr.lpszValue, "1"))
			{
				m_liquid = true;
			}
		}
		else if ( (attrName ==  "toplayercandisappear")
			|| (attrName == "toplayerdisappears"))
		{
			std::string var = MakeLower(attr.lpszValue);
			if (var == "true"
				|| var == "t")
			{
				m_topLayerDisappearsWhenPlayerUnderneath = true;
			}
		}
		else if ( (attrName == "toplayer2candisappear")
			|| (attrName == "toplayer2disappears"))
		{
			std::string var = MakeLower(attr.lpszValue);
			if (var == "true"
				|| var == "t")
			{
				m_topLayer2DisappearsWhenPlayerUnderneath = true;
			}
		}
		else if ( (attrName == CommonMapVars::s_BottomResourceName)
			|| attrName == "bottomresource")
		{
			m_BottomResourceName = std::string(attr.lpszValue);
			m_BottomResource = SpriteResourceHandler::GetSpriteResourceByName(m_BottomResourceName);
		}
		else if ( (attrName == CommonMapVars::s_ResourceName)
			|| attrName == "resource")
		{
			m_ResourceName = std::string(attr.lpszValue);
			m_resource = SpriteResourceHandler::GetSpriteResourceByName(m_ResourceName);
		}
		else if ((attrName ==CommonMapVars::s_TopResourceName)
			|| attrName == "topresource")
		{
			m_TopResourceName = std::string(attr.lpszValue);
			m_TopResource = SpriteResourceHandler::GetSpriteResourceByName(m_TopResourceName);
		}
		else if ((attrName == CommonMapVars::s_TopResource2Name)
			|| attrName == "topresource2")
		{
			m_TopResource2Name = std::string(attr.lpszValue);
			m_TopResource2 = SpriteResourceHandler::GetSpriteResourceByName(m_TopResource2Name);
		}
		else if ( (attrName==  "altbottomresourcename"))
		{
			std::string bottomResourceName = std::string(attr.lpszValue);
			m_AltBottomResource = SpriteResourceHandler::GetSpriteResourceByName(bottomResourceName);
		}
		else if ((attrName == "altresourcename"))
		{
			std::string resourceName = std::string(attr.lpszValue);
			m_AltResource = SpriteResourceHandler::GetSpriteResourceByName(resourceName);
		}
		else if ((attrName == "alttopresourcename"))
		{
			std::string resourceName = std::string(attr.lpszValue);
			m_AltTopResource = SpriteResourceHandler::GetSpriteResourceByName(resourceName);
		}
		else if ((attrName == "alttop2resourcename"))
		{
			std::string resourceName = std::string(attr.lpszValue);
			m_AltTopResource2 = SpriteResourceHandler::GetSpriteResourceByName(resourceName);
		}
		else if (attrName == CommonMapVars::s_testTintVar)
		{
			std::vector<std::string> vars = ParseString(attr.lpszValue, ',');
			if (vars.size() > 0)
			{
				m_tint.Red = (float)atof(vars.at(0).c_str());
			}
			if (vars.size() > 1)
			{
				m_tint.Green = (float)atof(vars.at(1).c_str());
			}
			if (vars.size() > 2)
			{
				m_tint.Blue = (float)atof(vars.at(2).c_str());
			}
			if (vars.size() > 3)
			{
				m_tint.Alpha = (float)atof(vars.at(3).c_str());
			}
		}
		else if ((attrName == "altbottomresourcechance"))
		{
			m_altBottomResourceChance = (float)atof(attr.lpszValue);
		}
		else if ((attrName =="altresourcechance"))
		{
			m_altResourceChance = (float)atof(attr.lpszValue);
		}
		else if ((attrName==  "alttopresourcechance"))
		{
			m_altTopResourceChance = (float)atof(attr.lpszValue);
		}
		else if ((attrName == "alttopresource2chance"))
		{
			m_altTop2ResourceChance = (float)atof(attr.lpszValue);
		}
	}
}

void TileDefinition::ReadChildrenFromTileDefNode(const XMLNode& node)
{
	TileEvent* OnExitEvent = nullptr;
	TileEvent* OnTouchEvent = nullptr;
	TileEvent* OnEnterEvent = nullptr;
	TileEvent* OnInteractEvent = nullptr;
	std::vector<StrXMLAttr> OnExitParams;
	std::vector<StrXMLAttr> OnTouchParams;
	std::vector<StrXMLAttr> OnEnterParams;
	std::vector<StrXMLAttr> OnInteractParams;
	std::string onExitName = "";
	std::string onEnterName = "";
	std::string onTouchName = "";
	std::string onInteractName = "";
	for (int childIdx = 0; childIdx < node.nChildNode(); childIdx++)
	{
		XMLNode child = node.getChildNode(childIdx);
		std::string lowerCaseName = MakeLower(child.getName());
		if (lowerCaseName == CommonMapVars::s_OnTouch)
		{
			ReadTileEventDataFromChild(child, OnTouchEvent, OnTouchParams, onTouchName);
		}
		else if (lowerCaseName == CommonMapVars::s_OnEnter)
		{
			ReadTileEventDataFromChild(child, OnEnterEvent, OnEnterParams, onEnterName);
		}
		else if (lowerCaseName == CommonMapVars::s_OnExit)
		{
			ReadTileEventDataFromChild(child, OnExitEvent, OnExitParams, onExitName);
		}
		else if (lowerCaseName == CommonMapVars::s_OnInteract)
		{
			ReadTileEventDataFromChild(child, OnInteractEvent, OnInteractParams, onInteractName);
		}
		else if (lowerCaseName == "specialcase"
			|| lowerCaseName == "special")
		{
			TileDefinitionSpecialCase specialCase = TileDefinitionSpecialCase(child);
			m_specialCases.push_back(specialCase);
		}
		else if (lowerCaseName == "tiledefinitionstopretendarethesame"
			|| lowerCaseName == "tiledefinitiontopretendarethesame"
			|| lowerCaseName =="pretendarethesame")
		{
			ReadTileDefinitionsToIgnore(child);
		}
	}
	m_eventPack = TileEventPack(OnInteractEvent, OnTouchEvent, OnEnterEvent, OnExitEvent,
		OnInteractParams, OnTouchParams, OnEnterParams, OnExitParams,
		onInteractName, onTouchName, onEnterName, onExitName);
}


void TileDefinition::ReadTileEventDataFromChild(const XMLNode& child,
	TileEvent*& EventPtr, std::vector<StrXMLAttr>& EventParams,
	std::string& name)
{
	std::vector<StrXMLAttr> attributes = EngineXMLParser::GetAttributesOffOfNode(child);
	for (size_t j = 0; j < attributes.size(); j++)
	{
		StrXMLAttr attr = attributes[j];
		if (SimpleStrCmpLower(attr.name, CommonMapVars::s_Function))
		{
			name = std::string(attr.val);
			EventPtr = TileEvent::GetTileEventForName(name);
			if (EventPtr != nullptr)
			{
				m_interactable = true;
			}
			else
			{
				m_interactable = false;
			}
		}
		else
		{
			EventParams.push_back(attr);
		}
	}
}


void TileDefinition::ReadTileDefinitionsToIgnore(const XMLNode& node)
{
	for (int childIdx = 0; childIdx < node.nChildNode(); childIdx++)
	{
		XMLNode child = node.getChildNode(childIdx);
		for (int attrIdx = 0; attrIdx < child.nAttribute(); attrIdx++)
		{
			XMLAttribute attr = child.getAttribute(attrIdx);
			std::string attrName = MakeLower(attr.lpszName);
			if (attrName == "name")
			{
				m_tileDefinitionNamesToPretendAreTheSame.push_back(MakeLower(attr.lpszValue));
			}
		}
	}
}

//------------------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------------------
//Setters
void TileDefinition::SetMapImageCode(Vector3 code)
{
	m_mapImageCode = code;
}

void TileDefinition::SetIsSolid(bool solid)
{
	m_solid = solid;
}

void TileDefinition::SetIsLiquid(bool liquid)
{
	m_liquid = liquid;
}

void TileDefinition::SetIsInteractable(bool interactable)
{
	m_interactable = interactable;
}

void TileDefinition::SetIsTriggerVolume(bool triggerVolume)
{
	m_triggerVolume = triggerVolume;
}

void TileDefinition::SetResourceName(std::string resourceName)
{
	m_ResourceName = resourceName;
}

void TileDefinition::SetTint(Rgba tint)
{
	m_tint = tint;
}

void TileDefinition::ClearDebugName()
{
	m_name = "";
}

//------------------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------------------
//Getters
bool TileDefinition::IsTileDefinitionNameOneToIgnore(const std::string& name) const
{
	for (size_t i = 0; i < m_tileDefinitionNamesToPretendAreTheSame.size(); i++)
	{
		const std::string& nameTest = MakeLower(m_tileDefinitionNamesToPretendAreTheSame[i]);
		if (nameTest == name)
		{
			return true;
		}
	}
	return false;
}

TileEvent* TileDefinition::GetOnTouchEvent() const
{
	return m_eventPack.GetEditableOnTouchEvent();
}

TileEvent* TileDefinition::GetOnExitEvent() const
{
	return m_eventPack.GetEditableOnExitEvent();
}

TileEvent* TileDefinition::GetOnEnterEvent() const
{
	return m_eventPack.GetEditableOnEnterEvent();
}

TileEvent* TileDefinition::GetOnInteractEvent() const
{
	return m_eventPack.GetEditableOnInteractEvent();
}


const std::vector<StrXMLAttr>& TileDefinition::GetOnTouchParams() const
{
	return m_eventPack.GetOnTouchParams();
}

const std::vector<StrXMLAttr>& TileDefinition::GetOnExitParams() const
{
	return  m_eventPack.GetOnExitParams();
}

const std::vector<StrXMLAttr>& TileDefinition::GetOnEnterParams() const
{
	return m_eventPack.GetOnEnterParams();
}

const std::vector<StrXMLAttr>& TileDefinition::GetOnInteractParams() const
{
	return m_eventPack.GetOnInteractParams();
}

std::vector<TileDefinitionSpecialCase> TileDefinition::GetCopyOfSpecialCases() const
{
	return m_specialCases;
}

std::vector<TileDefinitionSpecialCase>& TileDefinition::GetSpecialCasesEditablePtr()
{
	return m_specialCases;
}

const std::vector<TileDefinitionSpecialCase>& TileDefinition::GetSpecialCasesPtr() const
{
	return m_specialCases;
}

const Vector3& TileDefinition::GetMapImageCode() const
{
	return m_mapImageCode;
}

bool TileDefinition::GetIsSolid() const
{
	return m_solid;
}

bool TileDefinition::GetIsLiquid() const
{
	return m_liquid;
}

bool TileDefinition::GetIsInteractable() const
{
	return m_interactable;
}

bool TileDefinition::GetIsTriggerVolume() const
{
	return m_triggerVolume;
}

bool TileDefinition::GetTopLayerDisappearsIfPlayerUnderneath() const
{
	return m_topLayerDisappearsWhenPlayerUnderneath;
}
bool TileDefinition::GetTopLayer2DisappearsIfPlayerUnderneath() const
{
	return m_topLayer2DisappearsWhenPlayerUnderneath;
}

unsigned int TileDefinition::GetTileDefIdx() const
{
	return m_tileDefIdx;
}

int TileDefinition::GetMask() const
{
	int mask = 0;

	mask |= (m_solid << IS_SOLID);
	mask |= (m_liquid << IS_LIQUID);

	return mask;
}

const std::string& TileDefinition::GetResourceName() const
{
	return m_ResourceName;
}

const std::string& TileDefinition::GetDebugName() const
{
	return m_name;
}

const SpriteResource* TileDefinition::GetBottomSpriteResource() const
{
	return m_BottomResource;
}

const SpriteResource* TileDefinition::GetSpriteResource() const
{
	return m_resource;
}

const SpriteResource* TileDefinition::GetTopSpriteResource() const
{
	return m_TopResource;
}

const SpriteResource* TileDefinition::GetTopSprite2Resource() const
{
	return m_TopResource2;
}

const SpriteResource* TileDefinition::GetAltBottomSpriteResource() const
{
	return m_AltBottomResource;
}

const SpriteResource* TileDefinition::GetAltSpriteResource() const
{
	return m_AltResource;
}

const SpriteResource* TileDefinition::GetAltTopSpriteResource() const
{
	return m_AltTopResource;
}

const SpriteResource* TileDefinition::GetAltTopSprite2Resource() const
{
	return m_AltTopResource2;
}

const SpriteResource* TileDefinition::GetRandBottomSpriteResource() const
{
	if (m_BottomResource == nullptr
		|| (m_AltBottomResource != nullptr
		&& GetRandomFloatZeroToOne() <= m_altBottomResourceChance))
	{
		return m_AltBottomResource;
	}
	return m_BottomResource;
}

const SpriteResource* TileDefinition::GetRandSpriteResource() const
{
	if (m_resource == nullptr
		|| (m_AltResource != nullptr && GetRandomFloatZeroToOne() <= m_altResourceChance))
	{
		return m_AltResource;
	}
	return m_resource;
}

const SpriteResource* TileDefinition::GetRandTopSpriteResource() const
{
	if (m_TopResource == nullptr
		|| (m_AltTopResource != nullptr && GetRandomFloatZeroToOne() <= m_altTopResourceChance))
	{
		return m_AltTopResource;
	}
	return m_TopResource;
}

const SpriteResource* TileDefinition::GetRandTopSpriteResource2() const
{
	if (m_TopResource2 == nullptr
		|| (m_AltTopResource2 != nullptr && GetRandomFloatZeroToOne() <= m_altTop2ResourceChance))
	{
		return m_AltTopResource2;
	}
	return m_TopResource2;
}

const Rgba& TileDefinition::GetTint() const
{
	return m_tint;
}

void TileDefinition::WriteTileDefinitionToString(std::string& str, int indentationAmt) const
{
	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Tile Definition Name: " + m_name + "\n";
	indentationAmt += 3;

	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Tile Color Code: "
		+ std::to_string((int)m_mapImageCode.x) + ","
		+ std::to_string((int)m_mapImageCode.y) + ","
		+ std::to_string((int)m_mapImageCode.z)
		+ "\n";

	std::string tintAsString = MathToStringUtils::ToString(m_tint);
	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Tint: " + tintAsString + "\n";

	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Tiles are Solid: ";
	GameDebuggingCommons::WriteBoolOntoString(str, m_solid);

	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Tiles are interactable: ";
	GameDebuggingCommons::WriteBoolOntoString(str, m_interactable);

	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Tiles are Trigger Volume: ";
	GameDebuggingCommons::WriteBoolOntoString(str, m_triggerVolume);


	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Top Layer vanishes when Player underneath: ";
	GameDebuggingCommons::WriteBoolOntoString(str, m_topLayerDisappearsWhenPlayerUnderneath);


	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Top Layer 2 vanishes when Player underneath: ";
	GameDebuggingCommons::WriteBoolOntoString(str, m_topLayer2DisappearsWhenPlayerUnderneath);

	if (m_BottomResource != nullptr)
	{
		GameDebuggingCommons::AddIndentation(str, indentationAmt);
		str += "Bottom Resource Name: " + m_BottomResource->GetSpriteResourceName() + "\n";
	}

	if (m_resource != nullptr)
	{
		GameDebuggingCommons::AddIndentation(str, indentationAmt);
		str += "Resource Name: " + m_resource->GetSpriteResourceName() + "\n";
	}

	if (m_TopResource != nullptr)
	{
		GameDebuggingCommons::AddIndentation(str, indentationAmt);
		str += "Top Resource Name: " + m_TopResource->GetSpriteResourceName() + "\n";
	}

	if (m_TopResource2 != nullptr)
	{
		GameDebuggingCommons::AddIndentation(str, indentationAmt);
		str += "Top Resource 2 Name: " + m_TopResource2->GetSpriteResourceName() + "\n";
	}

	if (m_AltBottomResource != nullptr)
	{
		GameDebuggingCommons::AddIndentation(str, indentationAmt);
		str += "Alternative Bottom Resource Name: " + m_AltBottomResource->GetSpriteResourceName() + "\n";

		GameDebuggingCommons::AddIndentation(str, indentationAmt);
		str += "Alternative Bottom Resource Chance: " + std::to_string(m_altBottomResourceChance) + "\n";
	}

	if (m_AltResource != nullptr)
	{
		GameDebuggingCommons::AddIndentation(str, indentationAmt);
		str += "Alternative Resource Name: " + m_AltResource->GetSpriteResourceName() + "\n";

		GameDebuggingCommons::AddIndentation(str, indentationAmt);
		str += "Alternative Resource Chance: " + std::to_string(m_altResourceChance) + "\n";
	}

	if (m_AltTopResource != nullptr)
	{
		GameDebuggingCommons::AddIndentation(str, indentationAmt);
		str += "Alternative Top Resource Name: " + m_AltTopResource->GetSpriteResourceName() + "\n";

		GameDebuggingCommons::AddIndentation(str, indentationAmt);
		str += "Alternative Top Resource Chance: " + std::to_string(m_altTopResourceChance) + "\n";
	}

	if (m_AltTopResource2 != nullptr)
	{
		GameDebuggingCommons::AddIndentation(str, indentationAmt);
		str += "Alternative Top Resource 2 Name: " + m_AltTopResource2->GetSpriteResourceName() + "\n";

		GameDebuggingCommons::AddIndentation(str, indentationAmt);
		str += "Alternative Top Resource 2 Chance: " + std::to_string(m_altTop2ResourceChance) + "\n";
	}

	int nextLevel = indentationAmt + 3;
	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Tile Definitions to treat as the same as this Tile Definition:\n";
	for (size_t i = 0; i < m_tileDefinitionNamesToPretendAreTheSame.size(); i++)
	{
		GameDebuggingCommons::AddIndentation(str, nextLevel);
		str += m_tileDefinitionNamesToPretendAreTheSame[i] + "\n";
	}

	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Number of Special Cases: " + std::to_string(m_specialCases.size()) + "\n";
	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Special Cases: \n";
	for (size_t i = 0; i < m_specialCases.size(); i++)
	{
		m_specialCases[i].WriteSpecialCaseToString(str, nextLevel);
	}

	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Tile Event Data: \n";
	m_eventPack.WriteTileEventPackToString(str, nextLevel, false);
}


//------------------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------------------------
//Static Methods
STATIC void TileDefinition::WriteAllTileDefinitionsToString(std::string& str, int indentationAmt)
{
	if (s_definitions == nullptr)
	{
		return;
	}

	str += "Number of definitions: " + std::to_string(s_definitions->size()) + "\n";

	for (size_t i = 0; i < s_definitions->size(); i++)
	{
		const TileDefinition& def = s_definitions->at(i);
		def.WriteTileDefinitionToString(str, indentationAmt);
	}
}

STATIC void TileDefinition::RegisterTileDefinition(unsigned int index, const TileDefinition& def)
{
	if (s_definitions == nullptr)
	{
		s_definitions = new std::vector<TileDefinition>();
		unsigned int numberToReserve = 50;
		GameSettings* settings = TheGame::GetGameSettings();
		if (settings != nullptr)
		{
			numberToReserve = settings->m_preallocationAmountTileDefinitions;
		}
		s_definitions->reserve(numberToReserve);
	}

	if (index > s_definitions->size())
	{
		index = s_definitions->size();
	}
	def.m_tileDefIdx = index;

	for (size_t idx = index; idx < s_definitions->size(); idx++)
	{
		TileDefinition& definition = s_definitions->at(idx);
		definition.m_tileDefIdx = definition.m_tileDefIdx + 1;
	}
	s_definitions->insert(s_definitions->begin() + index, def);
}

void TileDefinition::PushbackTileDefinition(const TileDefinition& def)
{
	if (s_definitions == nullptr)
	{
		s_definitions = new std::vector<TileDefinition>();
		unsigned int numberToReserve = 50;
		GameSettings* settings = TheGame::GetGameSettings();
		if (settings != nullptr)
		{
			numberToReserve = settings->m_preallocationAmountTileDefinitions;
		}
		s_definitions->reserve(numberToReserve);
	}
	def.m_tileDefIdx = s_definitions->size();
	s_definitions->push_back(def);
}

STATIC TileDefinition* TileDefinition::GetTileDefinitionAtIndex(unsigned int index, bool& found)
{
	if (s_definitions == nullptr || index >= s_definitions->size())
	{
		found = false;
		return nullptr;
	}

	found = true;
	return &s_definitions->at(index);
}

STATIC TileDefinition* TileDefinition::GetTileDefinitionByName(const std::string& tileDefinitionName, bool& found)
{
	if (s_definitions == nullptr)
	{
		found = false;
		return nullptr;
	}
	std::string lowerCase = MakeLower(tileDefinitionName);

	for (size_t i = 0; i < s_definitions->size(); i++)
	{
		TileDefinition* def = &s_definitions->at(i);
		if (def != nullptr && (def->GetDebugName()) == lowerCase)
		{
			found = true;
			return def;
		}
	}
	found = false;
	return nullptr;
}

STATIC TileDefinition* TileDefinition::GetTileDefinitionForImageCode(const Vector3& imageCode, bool& found)
{
	if (s_definitions == nullptr)
	{
		found = false;
		return nullptr;
	}

	for (size_t i = 0; i < s_definitions->size(); i++)
	{
		if (s_definitions->at(i).GetMapImageCode() == imageCode)
		{
			found = true;
			return &s_definitions->at(i);
		}
	}

	found = false;
	return nullptr;
}

STATIC int TileDefinition::GetTileDefinitionMaskAtIndex(unsigned int index, bool& found)
{
	if (s_definitions == nullptr || index >= s_definitions->size())
	{
		found = false;
		return 0;
	}

	found = true;
	return s_definitions->at(index).GetMask();
}


STATIC void TileDefinition::LoadTileDefinitionsFromCommonXMLFolderAllAtOnce()
{
	std::string tileDefinitionXMLFolderLocation = "Data/XML/TileDefinitions/";
	std::string tileDefinitionXMLRootNodeName = "TileDefinitions";
	std::string tileDefinitionXMLNodeName = "TileDefinition";

	GameSettings* settings = TheGame::GetGameSettings();
	if (settings != nullptr)
	{
		tileDefinitionXMLFolderLocation = settings->m_tileDefinitionXMLFolderLocaction;
		tileDefinitionXMLRootNodeName = settings->m_tileDefinitionsXMLRootName;
		tileDefinitionXMLNodeName = settings->m_tileDefinitionXMLNodeName;
	}

	std::vector<std::string> files = FileUtils::EnumerateFilesInDirectory(tileDefinitionXMLFolderLocation,
		"*", true);

	for (size_t fileIdx = 0; fileIdx < files.size(); fileIdx++)
	{
		std::string fileStr = files.at(fileIdx);
		LoadDataFromXMLNodeFileLocation(fileStr, 
			tileDefinitionXMLRootNodeName,
			tileDefinitionXMLNodeName);
	}
}

STATIC bool TileDefinition::OnUpdateLoadTileDefinitionsFromCommonXMLFolder()
{
	if (s_loadingData == nullptr)
	{
		s_loadingData = new TileDefinitionLoadingData();
	}

	GameSettings* settings = TheGame::GetGameSettings();
	if (!s_loadingData->m_receivedFiles)
	{
		if (settings != nullptr)
		{
			s_loadingData->m_tileDefinitionXMLFolderLocation = settings->m_tileDefinitionXMLFolderLocaction;
			s_loadingData->m_tileDefinitionXMLRootNodeName = settings->m_tileDefinitionsXMLRootName;
			s_loadingData->m_tileDefinitionXMLNodeName = settings->m_tileDefinitionXMLNodeName;
			s_loadingData->m_amountOfTimeToTakeToReadNodesPerFrame = settings->m_amountOfTimeLoadingTileDefinitionsPerFrame;
		}
		s_loadingData->m_fileIdx = 0;
		s_loadingData->m_receivedFiles = true;
		s_loadingData->m_files = FileUtils::EnumerateFilesInDirectory(
			s_loadingData->m_tileDefinitionXMLFolderLocation,
			"*", true);
		if (s_loadingData->m_files.size() == 0)
		{
			return true;
		}
		return false;
	}

	if (s_loadingData->m_amountOfTimeToTakeToReadNodesPerFrame <= 0)
	{
		return true;
	}
	int numberOfTileDefinitionsReadThisFrame = 0;

	while ((size_t)s_loadingData->m_fileIdx < s_loadingData->m_files.size())
	{
		std::string fileStr = s_loadingData->m_files[s_loadingData->m_fileIdx];
		LoadDataFromXMLNodeFileLocation(fileStr,
			s_loadingData->m_tileDefinitionXMLRootNodeName,
			s_loadingData->m_tileDefinitionXMLNodeName);

		s_loadingData->m_fileIdx++;
		numberOfTileDefinitionsReadThisFrame++;
		uint64_t timeDiff = Performance::GetCurrentPerformanceCount() - g_TheGame->GetFrameStartTime();
		double timeDiffDouble = Performance::PerformanceCountToSeconds(timeDiff);
		if (timeDiffDouble >= s_loadingData->m_amountOfTimeToTakeToReadNodesPerFrame)
		{
			return false;
		}
	}

	if ((size_t)s_loadingData->m_fileIdx >= s_loadingData->m_files.size())
	{
		return true;
	}
	return false;
}

STATIC void TileDefinition::LoadDataFromXMLNodeFileLocation(const std::string& fileStr,
	const std::string& tileDefinitionXMLRootNodeName,
	const std::string& tileDefinitionXMLNodeName)
{
	std::string last3Letters = fileStr.substr(fileStr.size() - 3, 3);
	if (SimpleStrCmpLower(last3Letters, "xml") == false)
	{
		return;
	}

	bool errorOccurred = false;
	XMLNode root = EngineXMLParser::ParseXMLFile(fileStr,
		tileDefinitionXMLRootNodeName.c_str(), errorOccurred);
	if (errorOccurred == true)
	{
		return;
	}

	for (int childIdx = 0; childIdx < root.nChildNode(); childIdx++)
	{
		XMLNode child = root.getChildNode(childIdx);
		LoadTileDefinitionDataFromXMLNode(child, tileDefinitionXMLNodeName);
	}
}

STATIC void TileDefinition::LoadTileDefinitionDataFromXMLNode(const XMLNode& tileDef, 
	const std::string& nodeNameLookingFor)
{
	if (SimpleStrCmpLower(tileDef.getName(), nodeNameLookingFor))
	{
		TileDefinition def = TileDefinition(tileDef);
		if (def.m_tileIdxSet == false)
		{
			if (s_definitions == nullptr)
			{
				def.m_tileDefIdx = 0;
			}
			else
			{
				def.m_tileDefIdx = s_definitions->size();
			}
		}
		PushbackTileDefinition(def);
	}
}

STATIC void TileDefinition::ClearTileDefinitions()
{
	if (s_loadingData != nullptr)
	{
		delete s_loadingData;
		s_loadingData = nullptr;
	}
	if (s_definitions == nullptr)
	{
		return;
	}
	delete s_definitions;
	s_definitions = nullptr;
}