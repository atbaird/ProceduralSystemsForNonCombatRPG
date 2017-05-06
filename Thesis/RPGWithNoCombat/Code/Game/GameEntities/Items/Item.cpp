#include "Game/GameEntities/Items/Item.hpp"
#include "Game/GameEntities/Agents/Agent.hpp"
#include "Engine/2D/SpriteResourceHandler.hpp"
#include "Game/GameEntities/Items/ItemGenerator.hpp"
#include "Engine/Math/ExtendedMath.hpp"
#include "Game/Common/GameDebuggingCommons.hpp"


//---------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------
//Static Varaibles
STATIC std::vector<Item*>* Item::s_Items = nullptr;
STATIC Item* Item::s_DefaultMaleClothing = nullptr;
STATIC Item* Item::s_DefaultFemaleClothing = nullptr;

//---------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------
//Constructors
Item::Item(ItemType itemType, const SpriteResource* resource, Directions4 defaultDirection)
	: BaseGameEntity(ITEM_ENTITY, resource, defaultDirection),
	m_itemType(itemType)
{
	ItemConstructorCommons();
}

Item::Item(ItemType itemType, const XMLNode& node)
	: BaseGameEntity(ITEM_ENTITY, nullptr, SOUTH),
	m_itemType(itemType)
{
	ItemConstructorCommons();
	for (int attrIdx = 0; attrIdx < node.nAttribute(); attrIdx++)
	{
		XMLAttribute attr = node.getAttribute(attrIdx);
		std::string lowerCaseName = MakeLower(attr.lpszName);
		if (lowerCaseName == "name")
		{
			m_name = attr.lpszValue;
		}
		else if (lowerCaseName == "defaultimage")
		{
			m_currentSprite = SpriteResourceHandler::GetSpriteResourceByName(attr.lpszValue);
		}
		else if (lowerCaseName == "genderspecific")
		{
			std::string lowerCaseVal = MakeLower(attr.lpszValue);
			if (lowerCaseVal == "true"
				|| lowerCaseVal == "t")
			{
				m_genderSpecific = true;
			}
			else
			{
				m_genderSpecific = false;
			}
		}
		else if (lowerCaseName == "amountallowedtostackupto")
		{
			m_amtAllowedToStackUpTo = atoi(attr.lpszValue);
		}
	}

	for (int childIdx = 0; childIdx < node.nChildNode(); childIdx++)
	{
		XMLNode child = node.getChildNode(childIdx);
		std::string lowerCaseName = MakeLower(child.getName());
		if (lowerCaseName == "animations")
		{
			ReadXMLForAnimations(child, false);
		}
		else if (lowerCaseName == "alternativeanimations"
			|| lowerCaseName == "altanimations")
		{
			ReadXMLForAnimations(child, true);
		}
	}
}

Item::~Item()
{
	BaseGameEntity::~BaseGameEntity();
}

void Item::ItemConstructorCommons()
{
	m_entityStatPack.SetColorSchemeChoice(GetRandomIntLessThan(BaseGameEntity::s_MaxColorChoiceOptions));
	m_altSequences.resize(NUMBER_OF_DIRECTIONS4);

	for (size_t i = 0; i < NUMBER_OF_DIRECTIONS4; i++)
	{
		m_altSequences[i].resize(NUMBER_OF_ANIMATIONIDXES);
		for (size_t j = 0; j < NUMBER_OF_ANIMATIONIDXES; j++)
		{
			m_altSequences[i][j] = nullptr;
		}
	}
}

void Item::ReadXMLForAnimations(const XMLNode& child, bool altAnims)
{
	for (int animIdx = 0; animIdx < child.nChildNode(); animIdx++)
	{
		XMLNode anim = child.getChildNode(animIdx);
		Directions4 dir = INVALID_DIR4;
		int animIdxVal = -1;
		SpriteAnimationSequence* sequence = nullptr;
		for (int attrIdx = 0; attrIdx < anim.nAttribute(); attrIdx++)
		{
			XMLAttribute attr = anim.getAttribute(attrIdx);
			if (SimpleStrCmpLower(attr.lpszName, "animationName"))
			{
				sequence = SpriteResourceHandler::GetSpriteAnimationSequenceByName(attr.lpszValue);
			}
			else if (SimpleStrCmpLower(attr.lpszName, "direction"))
			{
				int val = atoi(attr.lpszValue);
				if (val != 0 || (val == 0 && SimpleStrCmp(attr.lpszValue, "0")))
				{
					dir = (Directions4)(val);
				}
				else
				{
					if (SimpleStrCmpLower(attr.lpszValue, "north"))
					{
						dir = NORTH;
					}
					else if (SimpleStrCmpLower(attr.lpszValue, "west"))
					{
						dir = WEST;
					}
					else if (SimpleStrCmpLower(attr.lpszValue, "south"))
					{
						dir = SOUTH;
					}
					else if (SimpleStrCmpLower(attr.lpszValue, "east"))
					{
						dir = EAST;
					}
				}
			}
			else if (SimpleStrCmpLower(attr.lpszName, "animIdx"))
			{
				int val = atoi(attr.lpszValue);
				if (val != 0 || (val == 0 && SimpleStrCmp(attr.lpszValue, "0")))
				{
					animIdxVal = val;
				}
				else
				{
					if (SimpleStrCmpLower(attr.lpszValue, "walk"))
					{
						animIdxVal = WALK;
					}
				}
			}
		}
		if (altAnims)
		{
			AddAltAnimationSequence(sequence, dir, animIdxVal);
		}
		else
		{
			AddAnimationSequence(sequence, dir, animIdxVal);
		}
	}
}

//---------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------
//Updates and Render
void Item::Update(const float& dt)
{
	if (m_equipped)
	{
		m_overridingAnimation = nullptr;
		if (m_AgentEquippedTo != nullptr)
		{
			Directions4 dir = m_AgentEquippedTo->GetCurrentAnimationsIdx();
			m_entityStatPack.SetCurrentAnimationDirectionIdx(dir);
			bool agentEquippedToIsMale = m_AgentEquippedTo->GetIsMale();

			if (m_genderSpecific && agentEquippedToIsMale == false)
			{
				SpriteAnimationSequence* sequence = m_altSequences[dir][m_entityStatPack.GetCurrentAnimationIdx()];
				SetOveridingSequence(sequence);
				//SetAnimationSequencePtr(&m_altSequences);
			}
			else
			{
				ResetOveridingSequence();
			}
		}
	}
	else
	{
		m_overridingAnimation = m_spriteAnimationToRenderWhenOnGround;
	}

	BaseGameEntity::Update(dt);
}

void Item::EquippedRender(const Matrix44& position) const
{ 
	position;
	//m_material.SetModelMatrix(position);
	ShaderProgram* prog = m_material.GetProgram();
	int colorSchemeChoice = m_entityStatPack.GetColorSchemeChoice();
	prog->SetProgramValueInt("gColorChoice", &colorSchemeChoice, 1);
	m_mesh.DrawWith(&m_material);
}

//---------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------
//Setters
void Item::SetItemCount(int itemCnt)
{
	m_itemCount = itemCnt;
}

void Item::SetAmountAllowedToStackUpTo(int amtAllowedToStackUpTo)
{
	m_amtAllowedToStackUpTo = amtAllowedToStackUpTo;
}

void Item::SetAgentEquippedTo(Agent* agent)
{
	m_AgentEquippedTo = agent;
	if (m_AgentEquippedTo == nullptr)
	{
		m_equipped = false;
	}
}

void Item::UnequipItem()
{
	m_AgentEquippedTo = nullptr;
	m_equipped = false;
}

void Item::SetOnGround(bool onGround)
{
	m_onGround = onGround;
}

void Item::SetEquipped(bool equipped)
{
	m_equipped = equipped;
}
void Item::SetAbleToBeEquipped(bool ableToBeEquipped)
{
	m_ableToBeEquipped = ableToBeEquipped;
}

void Item::SetGenderSpecific(bool genderSpecific)
{
	m_genderSpecific = genderSpecific;
}

void Item::AddAltAnimationSequence(SpriteAnimationSequence* sequence, Directions4 dir, int idx)
{
	if (sequence == nullptr
		|| dir < 0
		|| idx < 0
		|| (size_t)dir >= m_altSequences.size())
	{
		return;
	}
	if ((size_t)idx >= m_altSequences[dir].size())
	{
		m_altSequences[dir].resize(idx + 1);
	}

	m_altSequences[dir][idx] = sequence;
}


//---------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------
//Getters


//---------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------
//Static Functions
STATIC void Item::AbsoluteClearAllItems() //Includes Defaults.
{
	ClearAllItems();
	if (s_DefaultMaleClothing != nullptr)
	{
		if (s_DefaultFemaleClothing == s_DefaultMaleClothing)
		{
			s_DefaultFemaleClothing = nullptr;
		}
		delete s_DefaultMaleClothing;
		s_DefaultMaleClothing = nullptr;
	}
	if (s_DefaultFemaleClothing != nullptr)
	{
		delete s_DefaultFemaleClothing;
		s_DefaultFemaleClothing = nullptr;
	}
}

STATIC void Item::ClearAllItems()
{
	if (s_Items == nullptr)
	{
		return;
	}
	{
		for (size_t i = 0; i < s_Items->size(); i++)
		{
			Item* item = s_Items->at(i);
			if (item != nullptr)
			{
				delete item;
			}
		}
		delete s_Items;
		s_Items = nullptr;
	}
}

STATIC void Item::RegisterItem(Item* item)
{
	if (item == nullptr)
	{
		return;
	}
	if (s_Items == nullptr)
	{
		s_Items = new std::vector<Item *>();
	}
	s_Items->push_back(item);
}

STATIC void Item::UpdateItems(float dt)
{
	if (s_DefaultMaleClothing == nullptr)
	{

	}
	if (s_DefaultFemaleClothing != nullptr
		&& s_DefaultMaleClothing != s_DefaultFemaleClothing)
	{
		s_DefaultFemaleClothing->Update(dt);
	}

	if (s_Items == nullptr)
	{
		return;
	}
}

STATIC void Item::SetDefaultMaleClothes(Item* item)
{
	s_DefaultMaleClothing = item;
}

STATIC void Item::SetDefaultFemaleClothes(Item* item)
{
	s_DefaultFemaleClothing = item;
}

STATIC Item* Item::GetDefaultMaleClothes()
{
	return s_DefaultMaleClothing;
}

STATIC Item* Item::GetDefaultFemaleClothes()
{
	return s_DefaultFemaleClothing;
}

STATIC Item* Item::GetCopyDefaultMaleClothes()
{
	if (s_DefaultMaleClothing == nullptr)
	{
		return nullptr;
	}
	BaseGameEntity* entity = ItemGenerator::SpawnItemByItemName(s_DefaultMaleClothing->GetName().c_str(), Vector2::vec2_zeros);
	Item* clothes = dynamic_cast<Item*>(entity);
	return clothes;
}

STATIC Item* Item::GetCopyDefaultFemaleClothes()
{
	if (s_DefaultMaleClothing == nullptr)
	{
		return nullptr;
	}
	BaseGameEntity* entity = ItemGenerator::SpawnItemByItemName(s_DefaultFemaleClothing->GetName().c_str(), Vector2::vec2_zeros);
	Item* clothes = dynamic_cast<Item*>(entity);
	return clothes;
}