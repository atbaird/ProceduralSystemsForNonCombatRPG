#include "Game/GameEntities/BaseGameEntity.hpp"
#include "Game/Map/Map.hpp"
#include "Engine/Math/MathToStringUtils.hpp"
#include "Game/Common/GameDebuggingCommons.hpp"
#include "Game/GameEntities/Agents/Player/Player.hpp"
#include "Game/Common/GameCommonVars.hpp"
#include "Game/Quest/Quest.hpp"

//-----------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------
//Static variables
BaseGameEntity* BaseGameEntity::s_Player = (nullptr);
STATIC std::vector<std::vector<BaseGameEntity*>>* BaseGameEntity::s_BaseGameEntitiesByType = (nullptr);
STATIC std::vector<BaseGameEntity*>* BaseGameEntity::s_AllNPCs = (nullptr);
STATIC std::vector<BaseGameEntity*>* BaseGameEntity::s_AllFeatures = nullptr;
STATIC const int BaseGameEntity::s_MaxColorChoiceOptions = 7; 
STATIC bool BaseGameEntity::s_tilePlayerHasTopLayer = false;

//-----------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------
//Constructors
BaseGameEntity::BaseGameEntity(GameEntityType type, const SpriteResource* resource, 
	Directions4 defaultDirection)
	: m_currentSprite(resource),
	m_entityStatPack(defaultDirection, type)
{
	Directions4 dir = m_entityStatPack.GetCurrentAnimationDirectionIdx();
	m_entityStatPack.SetAngleEntityFacing(Directions::GetAngleForDirectionEnum(dir));
	m_entityStatPack.SetAngleDegreesEntityFacing(Directions::GetAngleDegreesForDirectionEnum(dir));
	m_mesh.CreateVAOID();
	m_mesh.PregenerateVBOAndIBO();
	m_material.SetProgram(ShaderProgram::GetDefaultShaderProgram());
	LoadInDefaultData();
	m_sequences.resize(NUMBER_OF_DIRECTIONS4);

	for (size_t i = 0; i < NUMBER_OF_DIRECTIONS4; i++)
	{
		m_sequences[i].resize(NUMBER_OF_ANIMATIONIDXES);
		for (size_t j = 0; j < NUMBER_OF_ANIMATIONIDXES; j++)
		{
			m_sequences[i][j] = nullptr;
		}
	}
}

BaseGameEntity::~BaseGameEntity()
{
}

void BaseGameEntity::CommonConstructorSetup(bool registerThis)
{
	GameEntityType type = m_entityStatPack.GetGameEntityType();
	if (type == INVALID_ENTITY || registerThis == false)
	{
		return;
	}
	else
	{
		Map* map = Map::GetEditableMap();
		map->RegisterGameEntity(this);
	}
}


void BaseGameEntity::LoadInDefaultData()
{
	if (m_Model.size() != 4)
	{
		SpriteVert vert;
		m_Model.clear();
		m_Model.reserve(4);
		m_Model.push_back(vert);
		m_Model.push_back(vert);
		m_Model.push_back(vert);
		m_Model.push_back(vert);
	}

	const Vector2 topRight = Vector2(0.5f, 0.5f);
	const Vector2 bottomLeft = Vector2(-0.5f, -0.5f);
	AABB2 textureCoords = AABB2::ZERO_TO_ONE;
	if (m_currentSprite != nullptr)
	{
		textureCoords = m_currentSprite->GetTextureCoords();
	}
	SpriteVert vert;
	vert.m_color = Rgba::s_White;

	//BL Position
	vert.m_pos = Vector2(bottomLeft.x, bottomLeft.y);
	vert.m_texCoords = Vector2(textureCoords.mins.x, textureCoords.mins.y);
	m_Model[0] = (vert);

	//BR Position
	vert.m_pos = Vector2(topRight.x, bottomLeft.y);
	vert.m_texCoords = Vector2(textureCoords.maxs.x, textureCoords.mins.y);
	m_Model[1] = (vert);

	//TR Position
	vert.m_pos = Vector2(topRight.x, topRight.y);
	vert.m_texCoords = Vector2(textureCoords.maxs.x, textureCoords.maxs.y);
	m_Model[2] = (vert);

	//TL Position
	vert.m_pos = Vector2(bottomLeft.x, topRight.y);
	vert.m_texCoords = Vector2(textureCoords.mins.x, textureCoords.maxs.y);
	m_Model[3] = (vert);
	m_mesh.setVBO(m_Model);
	
	if (m_indices.size() != 6)
	{
		m_indices.clear();
		m_indices.reserve(6);
		m_indices.push_back(0);
		m_indices.push_back(1);
		m_indices.push_back(2);
		m_indices.push_back(0);
		m_indices.push_back(2);
		m_indices.push_back(3);
		m_mesh.setIBO(m_indices);
	}
}

//-----------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------
//Updates
void BaseGameEntity::Update(const float& dt)
{
	AnimationUpdate(dt);
	MeshUpdate(dt);

	m_modelMatrix.MatrixMakeIdentity();
	Matrix44::MakeEulerRotation(m_modelMatrix, 0.f, 0.f, 0.f, 
		m_entityStatPack.GetPosition() + m_entityStatPack.GetRenderOffset());
}

void BaseGameEntity::AnimationUpdate(const float& dt)
{
	if (m_overridingAnimation != nullptr)
	{
		m_currentAnimation = m_overridingAnimation;
		{
			if (m_DebugSpriteIdxInAnimationOverride == -1)
			{
				m_currentAnimationPlayTime = 
					m_currentAnimation->UpdateTime(m_currentAnimationPlayTime, 
						dt, m_entityStatPack.GetCurrentAnimationPlaySpeed());
				int currentAnimationFrameIdx = m_entityStatPack.GetCurrentAnimationFrameIdx();
				const SpriteResource* res =
					m_currentAnimation->GetFrame(m_currentAnimationPlayTime, currentAnimationFrameIdx);
				m_entityStatPack.SetCurrentAnimationFrameIdx(currentAnimationFrameIdx);
				if (res != m_currentSprite)
				{
					m_currentSprite = res;
					m_UpdateMesh = true;
				}
			}
			else
			{
				{
					const SpriteResource* res = 
						m_currentAnimation->GetFrame(m_DebugSpriteIdxInAnimationOverride);
					if (res != m_currentSprite)
					{
						m_currentSprite = res;
						m_UpdateMesh = true;
					}
				}
			}
		}
	}
	else if (m_overridingSprite != nullptr)
	{
		if (m_currentSprite != m_overridingSprite)
		{
			m_currentSprite = m_overridingSprite;
			m_UpdateMesh = true;
		}
	}
	else
	{
		Directions4 currentAnimationDirectionIdx = m_entityStatPack.GetCurrentAnimationDirectionIdx();
		if (m_DebugSpriteIdxInAnimationOverride == -1)
		{
			if ((size_t)currentAnimationDirectionIdx >= 0 
				&& (size_t)currentAnimationDirectionIdx < m_sequences.size())
			{
				m_currentAnimation = 
					m_sequences[currentAnimationDirectionIdx][m_entityStatPack.GetCurrentAnimationIdx()];
				if (m_currentAnimation != nullptr)
				{
					m_currentAnimationPlayTime = 
						m_currentAnimation->UpdateTime(m_currentAnimationPlayTime, 
							dt, m_entityStatPack.GetCurrentAnimationPlaySpeed());
					int currentAnimationFrameIdx = 
						m_entityStatPack.GetCurrentAnimationFrameIdx();
					const SpriteResource* res = 
						m_currentAnimation->GetFrame(m_currentAnimationPlayTime, currentAnimationFrameIdx);
					m_entityStatPack.SetCurrentAnimationFrameIdx(currentAnimationFrameIdx);
					if (res != m_currentSprite)
					{
						m_currentSprite = res;
						m_UpdateMesh = true;
					}
				}
				else
				{
					m_currentAnimationPlayTime = 0.f;
				}
			}
			else
			{
				m_currentAnimationPlayTime = 0.f;
			}
		}
		else
		{
			m_currentAnimation =
				m_sequences[currentAnimationDirectionIdx][m_entityStatPack.GetCurrentAnimationIdx()];
			if (m_currentAnimation != nullptr)
			{
				const SpriteResource* res = 
					m_currentAnimation->GetFrame(m_DebugSpriteIdxInAnimationOverride);
				if (res != m_currentSprite)
				{
					m_currentSprite = res;
					m_UpdateMesh = true;
				}
			}

		}
	}
}

void BaseGameEntity::MeshUpdate(const float& dt)
{
	dt;
	if (m_UpdateMesh)
	{
		if (m_currentSprite != nullptr)
		{
			if (m_material.SetTexture(m_currentSprite->GetSpriteSheet(), 0, "gDiffuseTex", 0) == false)
			{
				m_material.AddTexture(m_currentSprite->GetSpriteSheet(), 0, "gDiffuseTex");
			}
		}
		LoadInDefaultData();
		m_UpdateMesh = false;
	}
}

void BaseGameEntity::Render() const
{
	if (m_currentSprite == nullptr)
	{
		return;
	}
	m_material.SetModelMatrix(m_modelMatrix);
	ShaderProgram* prog = m_material.GetProgram();
	int colorSchemeChoice = m_entityStatPack.GetColorSchemeChoice();
	prog->SetProgramValueInt("gColorChoice", &colorSchemeChoice, 1);
	m_mesh.DrawWith(&m_material);
}

//-----------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------
//Debug Controls
void BaseGameEntity::GetEntityInfoForDeveloperDebug(std::vector<DeveloperDebugInfoBox>& infoBoxes) const
{
	DeveloperDebugInfoBox info = DeveloperDebugInfoBox();
	info.SetBoxColor(Rgba(0.5f, 0.f, 0.f, 0.7f));
	WriteDeveloperDebugInfo(info);
	infoBoxes.push_back(info);
}

void BaseGameEntity::WriteDeveloperDebugInfo(DeveloperDebugInfoBox& info) const
{
	DeveloperDebugInfoTextBlurb nameBlurb = DeveloperDebugInfoTextBlurb(
		"Name: " + m_name, Rgba::s_White);
	info.AddTextBlurb(nameBlurb);

	DeveloperDebugInfoTextBlurb colorChoiceBlurb = DeveloperDebugInfoTextBlurb(
		"Color Choice: " + std::to_string(m_entityStatPack.GetColorSchemeChoice()), Rgba::s_White);
	info.AddTextBlurb(colorChoiceBlurb);

	DeveloperDebugInfoTextBlurb generatorUsedBlurb = DeveloperDebugInfoTextBlurb(
		"Generator: " + m_generatorUsed, Rgba::s_White);
	info.AddTextBlurb(generatorUsedBlurb);

	std::vector<Quest*> questReferences;
	GetListOfQuestsThatAllReferenceGivenEntity(this, questReferences);
	DeveloperDebugInfoTextBlurb numberOfQuestsApartOf = DeveloperDebugInfoTextBlurb(
		"Num Quests Referencing this: " + std::to_string(questReferences.size()), Rgba::s_White);

	if (questReferences.size() > 0)
	{
		DeveloperDebugInfoTextBlurb questsBlurb =
			DeveloperDebugInfoTextBlurb("Quests:", Rgba::s_White);
		questsBlurb.SetGuaranteeAloneOnLine(true);
		info.AddTextBlurb(questsBlurb);

		for (size_t i = 0; i < questReferences.size(); i++)
		{
			Quest* quest = questReferences[i];
			if (quest != nullptr)
			{
				DeveloperDebugInfoTextBlurb questBlurb = DeveloperDebugInfoTextBlurb(
					"     " + quest->GetQuestTitle(), Rgba::s_White);
				questBlurb.SetGuaranteeAloneOnLine(true);
				info.AddTextBlurb(questBlurb);
			}
		}
	}


}

void BaseGameEntity::SetDebugSpriteIdxInAnimationOverride(int overrid)
{
	m_DebugSpriteIdxInAnimationOverride = overrid;
}

void BaseGameEntity::DebugCycleThroughWalkingDirections()
{
	Directions4 currentAnimationDirectionIdx = 
		(Directions4)(m_entityStatPack.GetCurrentAnimationDirectionIdx() + 1);
	if (currentAnimationDirectionIdx >= NUMBER_OF_DIRECTIONS4)
	{
		currentAnimationDirectionIdx = (Directions4)0;
	}
	m_entityStatPack.SetCurrentAnimationDirectionIdx(currentAnimationDirectionIdx);
	m_DebugSpriteIdxInAnimationOverride = 0;
}

void BaseGameEntity::DebugCycleThroughAnimationIdx()
{
	int currentAnimationIdx = m_entityStatPack.GetCurrentAnimationIdx();
	currentAnimationIdx++;
	Directions4 currentAnimationDirectionIdx = m_entityStatPack.GetCurrentAnimationDirectionIdx();
	if ((size_t)currentAnimationIdx >= m_sequences[currentAnimationDirectionIdx].size())
	{
		currentAnimationIdx = 0;
	}
	m_entityStatPack.SetCurrentAnimationIdx(currentAnimationIdx);
	m_DebugSpriteIdxInAnimationOverride = 0;
}

void BaseGameEntity::DebugCycleThroughSpriteIdxInAnimation()
{
	m_DebugSpriteIdxInAnimationOverride++;
	if (m_currentAnimation == nullptr
		|| m_DebugSpriteIdxInAnimationOverride < 0
		|| m_DebugSpriteIdxInAnimationOverride >= (int)m_currentAnimation->GetNumberOfFrames())
	{
		m_DebugSpriteIdxInAnimationOverride = 0;
	}
}

//-----------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------
//Setters
void BaseGameEntity::SetGeneratorUsed(const std::string& generatorUsed)
{
	m_generatorUsed = generatorUsed;
}

void BaseGameEntity::SetPosition(const Vector2& position)
{
	const Map* map = Map::GetMap();
	SetPosition(position, map);
}

void BaseGameEntity::SetPosition(const Vector2& position, const Map* map)
{
	m_entityStatPack.SetPosition(position);

	if (map != nullptr)
	{
		m_entityStatPack.SetBLTileCoords(map->ConvertPositionToTileCoords(position));
	}
	else
	{
		//This should not happen; why is the map null?
		m_entityStatPack.SetBLTileCoords(TILE_COORDS((int)floor(position.x), (int)floor(position.y)));
	}
}

void BaseGameEntity::SetTint(const Rgba& tint)
{
	for (size_t i = 0; i < m_Model.size(); i++)
	{
		m_Model.at(i).m_color = tint;
	}
	m_UpdateMesh = true;
}

void BaseGameEntity::SetOveridingSequence(SpriteAnimationSequence* sequence)
{
	m_overridingAnimation = sequence;
}

void BaseGameEntity::SetColorSchemeChoice(int colorChoice)
{
	m_entityStatPack.SetColorSchemeChoice(colorChoice);
}

void BaseGameEntity::ResetOveridingSequence()
{
	m_overridingAnimation = nullptr;
}

void BaseGameEntity::AddAnimationSequence(SpriteAnimationSequence* sequence, Directions4 dir, int idx)
{
	if (sequence == nullptr
		|| dir < 0
		|| idx < 0
		|| (size_t)dir >= m_sequences.size())
	{
		return;
	}
	if ((size_t)idx >= m_sequences[dir].size())
	{
		m_sequences[dir].resize(idx + 1);
	}

	m_sequences[dir][idx] = sequence;
}


void BaseGameEntity::ChangeSpriteResource(const SpriteResource* resource)
{
	if (resource != m_currentSprite)
	{
		m_currentSprite = resource;
		m_UpdateMesh = true;
		
	}
}

void BaseGameEntity::ChangeCurrentAnimationPlaying(Directions4 animationDirIdx, int animationIdx)
{
	Directions4 currentAnimationDirectionIdx = m_entityStatPack.GetCurrentAnimationDirectionIdx();
	if (currentAnimationDirectionIdx != animationDirIdx
		|| m_entityStatPack.GetCurrentAnimationIdx() != animationIdx)
	{
		m_entityStatPack.SetCurrentAnimationDirectionIdx(animationDirIdx);
		m_entityStatPack.SetCurrentAnimationIdx(animationIdx);
		m_entityStatPack.SetAngleEntityFacing(Directions::GetAngleForDirectionEnum(animationDirIdx));
		m_entityStatPack.SetAngleDegreesEntityFacing(Directions::GetAngleDegreesForDirectionEnum(animationDirIdx));
		m_currentAnimationPlayTime = 0.f;
	}
}

void BaseGameEntity::SetAnimationPlaySpeed(float playSpeed)
{
	m_entityStatPack.SetCurrentAnimationPlaySpeed(playSpeed);
}

void BaseGameEntity::SetAnimationPlayTime(float playTime)
{
	m_currentAnimationPlayTime = playTime;
}

//-----------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------
//Getters
std::string BaseGameEntity::GetEntityTypeAsString() const
{
	GameEntityType type = GetEntityType();
	switch (type)
	{
	case INVALID_ENTITY:
		return "INVALID";
	case FEATURE_ENTITY:
		return "FEATURE";
	case HAIR_ENTITY:
		return "HAIR";
	case ITEM_ENTITY:
		return "ITEM";
	case AGENT_ENTITY:
		return "AGENT";
	default:
		return "";
	}
}


//-----------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------
//Operations
void BaseGameEntity::WriteOutEntityImportantDataToStr(std::string& str, int currentIndentationLevel) const
{
	str += "\n";
	GameDebuggingCommons::AddIndentation(str, currentIndentationLevel);
	str += "Entity Name: " + GetName() + "\n";
	GameDebuggingCommons::AddIndentation(str, currentIndentationLevel);
	str += "Entity Type: " + GetEntityTypeAsString() + "\n";
	GameDebuggingCommons::AddIndentation(str, currentIndentationLevel);
	str += "Tile Coordinates: " + MathToStringUtils::ToString(m_entityStatPack.GetBLTileCoords()) + "\n";
	GameDebuggingCommons::AddIndentation(str, currentIndentationLevel);
	str += "Actual Position: " + MathToStringUtils::ToString(m_entityStatPack.GetPosition()) + "\n";
	GameDebuggingCommons::AddIndentation(str, currentIndentationLevel);
	str += "Color Choice: " + std::to_string(m_entityStatPack.GetColorSchemeChoice()) + "\n";

}

void BaseGameEntity::SignUpForEvents()
{

}

void BaseGameEntity::UnsignUpForEvents()
{
	EventSystem::GlobalUnregisterFromAllEvents(this);
}

STATIC void BaseGameEntity::WriteBaseGameEntityToNamedProperties(NamedProperties& params,
	BaseGameEntity* entity)
{
	if (entity == nullptr)
	{
		//you fudged up.
		return;
	}
	params.Set("entityType", entity->GetEntityType());
	params.Set("entity", entity);
}

STATIC BaseGameEntity* BaseGameEntity::ReadBaseGameEntityFromNameProperties(NamedProperties& params)
{
	BaseGameEntity* entity = nullptr;
	params.Get("entity", entity, (BaseGameEntity*)nullptr);

	return entity;
}

STATIC BaseGameEntity* BaseGameEntity::ReadBaseGameEntityFromNameProperties(NamedProperties& params,
	GameEntityType& type)
{
	params.Get("entityType", type, INVALID_ENTITY);
	return ReadBaseGameEntityFromNameProperties(params);
}

//-----------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------
//Static methods
STATIC void BaseGameEntity::GetListOfQuestsThatAllReferenceGivenEntity(const BaseGameEntity* entity,
	std::vector<Quest*>& questReferences)
{
	questReferences.clear();
	if (entity == nullptr)
	{
		return;
	}
	for (std::map<std::string, Map*>::const_iterator it = GameCommons::s_AllLoadedMaps.begin();
		it != GameCommons::s_AllLoadedMaps.end(); it++)
	{
		if (it->second != nullptr)
		{
			it->second->AddAllQuestsThatReferenceEntityToAList(entity, questReferences);
		}
	}


	const BaseGameEntity* playerEntity = GetPointerToPlayer();
	const Player* player = dynamic_cast<const Player*>(playerEntity);
	if (player != nullptr)
	{
		const PlayerQuestManager& questManager = player->GetPlayerQuestManager();
		questManager.AddAllQuestsThatReferenceGivenEntityToList(entity, questReferences);
	}

}

STATIC void BaseGameEntity::TotalClearGameEntities()
{
	s_Player = nullptr;
}

STATIC void BaseGameEntity::SetPointerToPlayer(BaseGameEntity* player)
{
	s_Player = player;
}

STATIC const BaseGameEntity* BaseGameEntity::GetPointerToPlayer()
{
	return s_Player;
}

STATIC BaseGameEntity* BaseGameEntity::GetEditablePointerToPlayer()
{
	return s_Player;
}

STATIC void BaseGameEntity::SetBaseGameEntitiesPointer(std::vector<std::vector<BaseGameEntity*>>* baseEntitiesByType)
{
	s_BaseGameEntitiesByType = baseEntitiesByType;
}

STATIC void BaseGameEntity::SetAllNPCsPointer(std::vector<BaseGameEntity*>* baseEntitiesByType)
{
	s_AllNPCs = baseEntitiesByType;
}

STATIC void BaseGameEntity::SetAllFeaturesPointer(std::vector<BaseGameEntity*>* baseEntitiesByType)
{
	s_AllFeatures = baseEntitiesByType;
}

STATIC void BaseGameEntity::TestIfPlayerIsOnTileWithTransparentTop()
{
	if (s_Player == nullptr)
	{
		s_tilePlayerHasTopLayer = false;
		return;
	}

	Map* map = Map::GetEditableMap();
	if (map == nullptr)
	{
		s_tilePlayerHasTopLayer = false;
		return;
	}

	const Tile* tile = map->GetTileAtCoords(s_Player->GetTileCoords());
	if (tile == nullptr)
	{
		s_tilePlayerHasTopLayer = false;
		return;
	}
	
	s_tilePlayerHasTopLayer = (tile->GetCurrentTopSpriteResource() != nullptr);
	return;
}

STATIC bool BaseGameEntity::GetIfPlayerIsInTileWithTransparentTop()
{
	return s_tilePlayerHasTopLayer;
}

STATIC const std::vector<BaseGameEntity*>* BaseGameEntity::GetAllFeaturesPtr()
{
	return s_AllFeatures;
}

STATIC const std::vector<BaseGameEntity*>* BaseGameEntity::GetAllNPCsPtr()
{
	return s_AllNPCs;
}