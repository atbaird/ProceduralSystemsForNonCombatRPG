#pragma once

#ifndef DIALOGUECHOICE_HPP
#define DIALOGUECHOICE_HPP
#include "Engine/Renderer/Rgba.hpp"
#include <string>
#include <vector>

class DialogueRequirement;
class DialogueTrigger;
class Mesh;
class Material;
class Font;
struct XMLNode;
struct Vertex_PCT;
class DialogueChoice
{
private:
protected:
	//Static variables
	static constexpr int s_DialogueChoiceCharacterLimit = 5;
	static constexpr float s_DialogueChoiceFontSize = 0.013f;
public:
	static const DialogueChoice s_EmptyChoice;

protected:

	//Variables
	std::vector<DialogueRequirement*> m_choiceRequirements;
	std::vector<DialogueTrigger*> m_choiceActions;
	std::vector<Mesh*> m_textMeshes;
	std::vector<std::vector<Vertex_PCT>> m_vertices;
	std::vector<std::vector<unsigned int>> m_indices;
	Rgba m_choiceColor = Rgba(0.f, 0.f, 0.f, 1.f);
	std::string m_dialogueChoice = "";
	std::string m_dialogueSegmentToOpenAfterSelected = "";
public:
	//Constructors
	DialogueChoice(const std::string& dialogueChoice = "", 
		const std::string& dialogueSegmentToOpenAfterSelected = "");
	DialogueChoice(const XMLNode& node);
	DialogueChoice(const DialogueChoice& other);
	virtual ~DialogueChoice();

protected:
	void ReadXMLAttributes(const XMLNode& node);
	void ReadXMLChildren(const XMLNode& node);
	void ReadChildForDialogueActions(const XMLNode& node);
	void ReadChildForDialogueRequirements(const XMLNode& node);

public:

	//Renders and Updates
	void Render(const Vector2& position,const std::vector<Material>& materials) const;

	//Setters
	void SetDialogueChoice(const std::string& choice);
	void SetDialogueSegmentToOpenAfterSelected(const std::string segment);
	void BuildMeshFromFont(Font* font);

	//Getters
	const std::string& GetDialogueChoice() const;
	const std::string& GetDialogueSegmentToOpenAfterSelected() const;
	bool CalculateShouldThisChoiceBeAvailable() const;

	//Operations
	void TriggerUniqueEvents();
	void WriteDialogueChoiceToString(std::string& str, int indentationAmt) const;
};

#endif