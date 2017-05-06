#pragma once

#ifndef TILEDEFINITION_HPP
#define TILEDEFINITION_HPP
#include "Engine/Math/Vector3.hpp"
#include "Engine/2D/SpriteResource.hpp"
#include "Game/Map/TileEvent/TileEvent.hpp"
#include "Engine/Core/EngineXMLParser.hpp"
#include "Game/Map/TileEvent/TileEventPack.hpp"
#include "Game/Map/TileDefinitionSpecialCase/BitsPerBool.hpp"
#include "Game/Map/TileDefinitionSpecialCase/TileDefinitionSpecialCase.hpp"
#include <vector>
#include <string>

#define STATIC
struct XMLNode;
class TileEventPack;

struct TileDefinitionLoadingData
{
	std::vector<std::string> m_files;
	std::string m_tileDefinitionXMLFolderLocation = "Data/XML/TileDefinitions/";
	std::string m_tileDefinitionXMLRootNodeName = "TileDefinitions";
	std::string m_tileDefinitionXMLNodeName = "TileDefinition";
	double m_amountOfTimeToTakeToReadNodesPerFrame = 0.25;
	int m_fileIdx = 0;
	bool m_receivedFiles = false;
};

#define MaxDebugNameLengthTileDef 32

class TileDefinition
{
private:
protected:
	//static variables
	static std::vector<TileDefinition>* s_definitions;
	static TileDefinitionLoadingData* s_loadingData;
	static int s_SolidMask;
	static int s_LiquidMask;

	//variables
	std::vector<TileDefinitionSpecialCase> m_specialCases;
	std::vector<std::string> m_tileDefinitionNamesToPretendAreTheSame;
	std::string m_name = "default";
	const SpriteResource* m_BottomResource = nullptr;
	const SpriteResource* m_resource = nullptr;
	const SpriteResource* m_TopResource = nullptr;
	const SpriteResource* m_TopResource2 = nullptr;
	const SpriteResource* m_AltBottomResource = nullptr;
	const SpriteResource* m_AltResource = nullptr;
	const SpriteResource* m_AltTopResource = nullptr;
	const SpriteResource* m_AltTopResource2 = nullptr;
	TileEventPack m_eventPack;
	std::string m_BottomResourceName = "";
	std::string m_ResourceName = "";
	std::string m_TopResourceName = "";
	std::string m_TopResource2Name = "";
	Rgba m_tint = Rgba::s_White;
	Vector3 m_mapImageCode = Vector3::vec3_zeros;
	float m_altBottomResourceChance = 0.f;
	float m_altResourceChance = 0.f;
	float m_altTopResourceChance = 0.f;
	float m_altTop2ResourceChance = 0.f;
	mutable unsigned int m_tileDefIdx = 0;
	bool m_solid = false;
	bool m_liquid = false;
	bool m_interactable = false;
	bool m_triggerVolume = false;
	bool m_tileIdxSet = false;
	bool m_topLayerDisappearsWhenPlayerUnderneath = false;
	bool m_topLayer2DisappearsWhenPlayerUnderneath = false;
public:
	//Constructors
	TileDefinition();
	TileDefinition(const XMLNode& node);
	virtual ~TileDefinition();

protected:
	void ReadAttributesFromTileDefNode(const XMLNode& node, bool& nameSet);
	void ReadChildrenFromTileDefNode(const XMLNode& node);
	void ReadTileEventDataFromChild(const XMLNode& child, 
		TileEvent*& OnInteractEvent, std::vector<StrXMLAttr>& OnInteractParams,
		std::string& name);
	void ReadTileDefinitionsToIgnore(const XMLNode& child);
public:

	//Setters
	void SetMapImageCode(Vector3 code);
	void SetIsSolid(bool solid);
	void SetIsLiquid(bool liquid);
	void SetIsInteractable(bool interactable);
	void SetIsTriggerVolume(bool triggerVolume);
	void SetResourceName(std::string resourceName);
	void SetTint(Rgba tint);
	void ClearDebugName();

	//Getters
	bool IsTileDefinitionNameOneToIgnore(const std::string& name) const;
	TileEvent* GetOnTouchEvent() const;
	TileEvent* GetOnExitEvent() const;
	TileEvent* GetOnEnterEvent() const;
	TileEvent* GetOnInteractEvent() const;
	const std::vector<StrXMLAttr>& GetOnTouchParams() const;
	const std::vector<StrXMLAttr>& GetOnExitParams() const;
	const std::vector<StrXMLAttr>& GetOnEnterParams() const;
	const std::vector<StrXMLAttr>& GetOnInteractParams() const;
	std::vector<TileDefinitionSpecialCase> GetCopyOfSpecialCases() const;
	std::vector<TileDefinitionSpecialCase>& GetSpecialCasesEditablePtr();
	const std::vector<TileDefinitionSpecialCase>& GetSpecialCasesPtr() const;
	const Vector3& GetMapImageCode() const;
	bool GetIsSolid() const;
	bool GetIsLiquid() const;
	bool GetIsInteractable() const;
	bool GetIsTriggerVolume() const;
	bool GetTopLayerDisappearsIfPlayerUnderneath() const;
	bool GetTopLayer2DisappearsIfPlayerUnderneath() const;
	unsigned int GetTileDefIdx() const;
	int GetMask() const;
	const std::string& GetResourceName() const;
	const std::string& GetDebugName() const;
	const SpriteResource* GetBottomSpriteResource() const;
	const SpriteResource* GetSpriteResource() const;
	const SpriteResource* GetTopSpriteResource() const;
	const SpriteResource* GetTopSprite2Resource() const;
	const SpriteResource* GetAltBottomSpriteResource() const;
	const SpriteResource* GetAltSpriteResource() const;
	const SpriteResource* GetAltTopSpriteResource() const;
	const SpriteResource* GetAltTopSprite2Resource() const;
	const SpriteResource* GetRandBottomSpriteResource() const;
	const SpriteResource* GetRandSpriteResource() const;
	const SpriteResource* GetRandTopSpriteResource() const;
	const SpriteResource* GetRandTopSpriteResource2() const;
	const Rgba& GetTint() const;
	void WriteTileDefinitionToString(std::string& str, int indentationAmt) const;

	//Static Methods
	static void WriteAllTileDefinitionsToString(std::string& str, int indentationAmt);
	static void RegisterTileDefinition(unsigned int index, const TileDefinition& def);
	static void PushbackTileDefinition(const TileDefinition& def);
	static TileDefinition* GetTileDefinitionAtIndex(unsigned int index, bool& found);
	static TileDefinition* GetTileDefinitionByName(const std::string& tileDefinitionName, bool& found);
	static TileDefinition* GetTileDefinitionForImageCode(const Vector3& imageCode, bool& found);
	static int GetTileDefinitionMaskAtIndex(unsigned int index, bool& found);
	static void LoadTileDefinitionsFromCommonXMLFolderAllAtOnce();
	static bool OnUpdateLoadTileDefinitionsFromCommonXMLFolder();
protected:
	static void LoadDataFromXMLNodeFileLocation(const std::string& fileStr,
		const std::string& tileDefinitionXMLRootNodeName,
		const std::string& tileDefinitionXMLNodeName);
public:
	static void LoadTileDefinitionDataFromXMLNode(const XMLNode& tileDef, const std::string& nodeNameLookingFor);
	static void ClearTileDefinitions();

	static bool GetIsSolidFromMask(int mask) { return (mask & s_SolidMask) != 0; };
	static bool GetIsLiquidFromMask(int mask) { return (mask & s_LiquidMask) != 0; };
};
#endif