#pragma once

#ifndef TILEEVENTPACK_HPP
#define TILEEVENTPACK_HPP
#include "Engine/Math/IntVector2.hpp"
#include "Game/Map/TileEvent/TileEvent.hpp"

#define TILE_COORDS IntVector2

class TileEventPack
{
private:
protected:
	//Variables
	std::vector<StrXMLAttr> m_OnInteractParams;
	std::vector<StrXMLAttr> m_OnTouchParams;
	std::vector<StrXMLAttr> m_OnEnterParams;
	std::vector<StrXMLAttr> m_OnExitParams;
	TILE_COORDS m_coordsToEffect = TILE_COORDS::ivec2_zeros;
	int m_tileIdx = 0;
	TileEvent* m_onInteract = nullptr;
	TileEvent* m_onTouch = nullptr;
	TileEvent* m_onEnter = nullptr;
	TileEvent* m_onExit = nullptr;

	std::string m_OnInteractEventName = "";
	std::string m_OnTouchEventName = "";
	std::string m_OnEnterEventName = "";
	std::string m_onExitEventName = "";
public:
	//Constructors
	TileEventPack();
	TileEventPack(TileEvent* onInteract, TileEvent* onTouch, TileEvent* onEnter, TileEvent* onExit,
		const std::vector<StrXMLAttr>& onInteractParams, 
		const std::vector<StrXMLAttr>& onTouchParams,
		const std::vector<StrXMLAttr>& onEnterParams,
		const std::vector<StrXMLAttr>& onExitParams,
		const std::string& OnInteractEventName,
		const std::string& OnTouchEventName,
		const std::string& OnEnterEventName,
		const std::string& OnExitEventName,
		const TILE_COORDS& coords = TILE_COORDS::ivec2_zeros,
		int tileIdx = 0);
	virtual ~TileEventPack();

	//Setters
	void SetTileCoords(const TILE_COORDS& tileCoords);
	void SetTileIdx(int tileIdx);
	void SetOnInteractEvent(TileEvent* tileEvent, const std::string& eventName);
	void SetOnTouchEvent(TileEvent* tileEvent, const std::string& eventName);
	void SetOnEnterEvent(TileEvent* tileEvent, const std::string& eventName);
	void SetOnExitEvent(TileEvent* tileEvent, const std::string& eventName);
	void SetOnInteractParams(const std::vector<StrXMLAttr>& params);
	void SetOnTouchParams(const std::vector<StrXMLAttr>& params);
	void SetOnEnterParams(const std::vector<StrXMLAttr>& params);
	void SetOnExitParams(const std::vector<StrXMLAttr>& params);

	//Getters
	const TILE_COORDS& GetTileCoords() const;
	const int GetTileIdx() const;
	const TileEvent* GetOnInteractEvent() const;
	const TileEvent* GetOnTouchEvent() const;
	const TileEvent* GetOnEnterEvent() const;
	const TileEvent* GetOnExitEvent() const;
	TileEvent* GetEditableOnInteractEvent() const;
	TileEvent* GetEditableOnTouchEvent() const;
	TileEvent* GetEditableOnEnterEvent() const;
	TileEvent* GetEditableOnExitEvent() const;
	const std::vector<StrXMLAttr>& GetOnInteractParams() const;
	const std::vector<StrXMLAttr>& GetOnTouchParams() const;
	const std::vector<StrXMLAttr>& GetOnEnterParams() const;
	const std::vector<StrXMLAttr>& GetOnExitParams() const;

	void WriteTileEventPackToString(std::string& str, int indentationAmt, bool writeTileCoords = true) const;
};

#endif