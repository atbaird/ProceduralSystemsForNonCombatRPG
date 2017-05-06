#pragma once

#ifndef ITEM_HPP
#define ITEM_HPP
#include "Game/GameEntities/BaseGameEntity.hpp"
#include "Game/GameEntities/Items/ItemTypeSpawnerRegistration.hpp"
#include "Game/Enums/ItemCareAboutGenderOrNot.hpp"
#include "Game/Enums/ItemType.hpp"

class Agent;

class Item : public BaseGameEntity
{
private:
protected:
	//Static Variables
	static std::vector<Item*>* s_Items;
	static Item* s_DefaultMaleClothing;
	static Item* s_DefaultFemaleClothing;

	//Variables
	std::vector<std::vector<SpriteAnimationSequence*>> m_altSequences;
	SpriteAnimationSequence* m_spriteAnimationToRenderWhenOnGround = nullptr;
	Agent* m_AgentEquippedTo = nullptr;
	ItemType m_itemType = INVALID_ITEM_TYPE;
	bool m_onGround = false;
	bool m_equipped = false;
	bool m_ableToBeEquipped = false;
	bool m_genderSpecific = false;
	int m_itemCount = 1;
	int m_amtAllowedToStackUpTo = 1;
public:
	//Constructors
	Item(ItemType itemType = INVALID_ITEM_TYPE, const SpriteResource* resource = nullptr,
		Directions4 defaultDirection = SOUTH);
	Item(ItemType itemType, const XMLNode& node);
	virtual ~Item();
protected:
	void ItemConstructorCommons();
	void ReadXMLForAnimations(const XMLNode& node, bool altAnims);

public:
	//Updates and Render
	virtual void Update(const float& dt) override;
	virtual void EquippedRender(const Matrix44& position) const;

	//Setters
	void SetItemCount(int itemCnt);
	void SetAmountAllowedToStackUpTo(int amtAllowedToStackUpTo);
	void SetAgentEquippedTo(Agent* agent);
	void UnequipItem();
	void SetOnGround(bool onGround);
	void SetEquipped(bool equipped);
	void SetAbleToBeEquipped(bool ableToBeEquipped);
	void SetGenderSpecific(bool genderSpecific);
	virtual void AddAltAnimationSequence(SpriteAnimationSequence* sequence, Directions4 dir, int idx);


	//Getters
	int GetItemCount() const { return m_itemCount; };
	int GetAmountAllowedToStackUpTo() const { return m_amtAllowedToStackUpTo; };
	ItemType GetItemType() const { return m_itemType; };
	bool GetOnGround() const { return m_onGround; };
	bool GetEquipped() const { return m_equipped; };
	bool GetAbleToBeEquipped() const { return m_ableToBeEquipped; };

	//Static Functions
	static void AbsoluteClearAllItems(); //Includes Defaults.
	static void ClearAllItems();
	static void RegisterItem(Item* item);
	static void UpdateItems(float dt);
	static void SetDefaultMaleClothes(Item* item);
	static void SetDefaultFemaleClothes(Item* item);
	static Item* GetDefaultMaleClothes();
	static Item* GetDefaultFemaleClothes();
	static Item* GetCopyDefaultMaleClothes();
	static Item* GetCopyDefaultFemaleClothes();
};

#endif