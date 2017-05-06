#pragma once

#ifndef FEATURETEMPLATE_HPP
#define FEATURETEMPLATE_HPP
#include "Engine/2D/SpriteResource.hpp"
#include "Engine/2D/SpriteAnimationSequence.hpp"
#include "Game/Enums/Directions4.hpp"
#include "Game/Enums/AnimationsIdxes.hpp"
#include "Game/GameEntities/Features/FeatureBehavior/FeatureBehavior.hpp"
#include "Game/Enums/DefinedFeatureBehaviorIdxes.hpp"

class Feature;
struct XMLNode;
class FeatureTemplate
{
private:
protected:
	std::string m_defaultImageName = "default";
	const SpriteResource* m_defaultImage = nullptr;
	std::vector<std::vector<SpriteAnimationSequence*>> m_sequences;
	std::vector<std::vector<FeatureBehavior*>> m_FeatureBehaviors;
	std::vector<std::vector<std::string>> m_sequenceNames;
	Vector2 m_renderOffset = Vector2::vec2_zeros;
	bool m_solid = false;
	bool m_overwriteTileOnSolid = false;
public:
	//Constructors
	FeatureTemplate();
	FeatureTemplate(const FeatureTemplate& other);
	virtual ~FeatureTemplate();

	//Operations
	void CopyDataOntoFeature(Feature* feature);
	void CopyDataOntoFeature(Feature* feature, const XMLNode& node);

	//Setters
	void SetRenderOffset(const Vector2& renderOffset);
	void SetDefaultImage(const SpriteResource* image, const std::string& imageName = "");
	void AddSpriteAnimationSequence(SpriteAnimationSequence* sequence, Directions4 dir, int idx,
		const std::string& animName = "");
	void AddFeatureBehavior(size_t bevType, FeatureBehavior* bev);
	void SetIsSolid(bool solid, bool overwrite = false);


	//Getters
	const Vector2& GetRenderOffset() const;
	bool GetOverwriteIsTileOnSolid() const;
	bool GetIsSolid() const;
	size_t GetNumberOfFeatureBehaviors() const;
	const SpriteResource* GetDefaultImage() const;
	const std::vector<std::vector<SpriteAnimationSequence*>>& GetAnimationSequences() const;
	const std::vector<std::vector<SpriteAnimationSequence*>>* GetAnimationSequencesPtr() const;
	std::vector<std::vector<SpriteAnimationSequence*>>* GetEditableAnimationSequencesPtr();
	const std::vector<std::vector<FeatureBehavior*>>& GetFeatureBehaviors() const;
	const std::vector<std::vector<FeatureBehavior*>>* GetFeatureBehaviorsPtr() const;
	std::vector<std::vector<FeatureBehavior*>>* GetEditableFeatureBehaviorsPtr();

	void WriteFeatureTemplateToString(std::string& str, int indentationLvl) const;
};
#endif