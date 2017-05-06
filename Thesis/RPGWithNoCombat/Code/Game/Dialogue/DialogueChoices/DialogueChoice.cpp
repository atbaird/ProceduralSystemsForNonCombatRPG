#include "Game/Dialogue/DialogueChoices/DialogueChoice.hpp"
#include "Engine/Core/EngineXMLParser.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Engine/Renderer/3DModelling/Mesh.hpp"
#include "Engine/Math/Vertex_PCT.hpp"
#include "Engine/Renderer/3DModelling/Material.hpp"
#include "Engine/Font/Font.hpp"
#include "Game/Dialogue/DialogueActions/DialogueTrigger.hpp"
#include "Game/Dialogue/DialogueRequirements/DialogueRequirement.hpp"
#include "Game/Dialogue/DialogueRequirements/DialogueRequirementRegistrationHelper.hpp"
#include "Game/Dialogue/DialogueActions/DialogueActionRegistrationHelper.hpp"
#include "Game/Common/GameDebuggingCommons.hpp"
#include "Engine/Math/MathToStringUtils.hpp"

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------
//Static Variables
STATIC const DialogueChoice DialogueChoice::s_EmptyChoice;

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------
//Constructors
DialogueChoice::DialogueChoice(const std::string& dialogueChoice,
	const std::string& dialogueSegmentToOpenAfterSelected)
	: m_dialogueChoice(dialogueChoice),
	m_dialogueSegmentToOpenAfterSelected(dialogueSegmentToOpenAfterSelected)
{
	if (m_dialogueChoice.length() > s_DialogueChoiceCharacterLimit)
	{
		m_dialogueChoice = m_dialogueChoice.substr(0, s_DialogueChoiceCharacterLimit);
	}
}

DialogueChoice::DialogueChoice(const XMLNode& node)
{
	/*
	<Decision>
	  <Yes DialogueToOpen="LongDialogueExampleWithChoiceSelectionYes"/>
	  <No DialogueToOpen="LongDialogueExampleWithChoiceSelectionNo"/>
	</Decision>
	*/
	m_dialogueChoice = node.getName();
	ReadXMLAttributes(node);
	ReadXMLChildren(node);
}

DialogueChoice::DialogueChoice(const DialogueChoice& other)
	: m_vertices(other.m_vertices),
	m_indices(other.m_indices),
	m_choiceColor(other.m_choiceColor),
	m_dialogueChoice(other.m_dialogueChoice),
	m_dialogueSegmentToOpenAfterSelected(other.m_dialogueSegmentToOpenAfterSelected)
{
	for (size_t requirementIdx = 0; requirementIdx < other.m_choiceRequirements.size(); requirementIdx++)
	{
		DialogueRequirement* req = other.m_choiceRequirements[requirementIdx];
		if (req != nullptr)
		{
			m_choiceRequirements.push_back(req->Clone());
		}
	}

	for (size_t actionIdx = 0; actionIdx < other.m_choiceActions.size(); actionIdx++)
	{
		DialogueTrigger* req = other.m_choiceActions[actionIdx];
		if (req != nullptr)
		{
			m_choiceActions.push_back(req->Clone());
		}
	}


}

DialogueChoice::~DialogueChoice()
{
	for (size_t actionIdx = 0; actionIdx < m_choiceActions.size(); actionIdx++)
	{
		DialogueTrigger* action = m_choiceActions[actionIdx];
		if (action == nullptr)
		{
			continue;
		}
		delete action;
	}
	m_choiceActions.clear();

	for (size_t requirementIdx = 0; requirementIdx < m_choiceRequirements.size(); requirementIdx++)
	{
		DialogueRequirement* req = m_choiceRequirements[requirementIdx];
		if (req == nullptr)
		{
			continue;
		}

		delete req;
	}
	m_choiceRequirements.clear();

	for (size_t idx = 0; idx < m_textMeshes.size(); idx++)
	{
		Mesh* mesh = m_textMeshes[idx];
		if (mesh != nullptr)
		{
			delete mesh;
		}
	}

	m_textMeshes.clear();
}


void DialogueChoice::ReadXMLAttributes(const XMLNode& node)
{
	for (int attrIdx = 0; attrIdx < node.nAttribute(); attrIdx++)
	{
		XMLAttribute attr = node.getAttribute(attrIdx);
		std::string attrName = MakeLower(attr.lpszName);
		if (attrName == "dialoguetoopen"
			|| attrName == "dialogue"
			|| attrName == "dialoguesegment")
		{
			m_dialogueSegmentToOpenAfterSelected = attr.lpszValue;
		}
		else if (attrName == "actualdialoguechoice"
			|| attrName == "dialoguechoice")
		{
			m_dialogueChoice = std::string(attr.lpszValue);
		}
	}
}

void DialogueChoice::ReadXMLChildren(const XMLNode& node)
{
	for (int childIdx = 0; childIdx < node.nChildNode(); childIdx++)
	{
		XMLNode child = node.getChildNode(childIdx);
		std::string childName = MakeLower(child.getName());
		if (childName == "requirements"
			|| childName == "requirement")
		{
			ReadChildForDialogueRequirements(child);
		}
		else if (childName == "actions"
			|| childName == "action"
			|| childName == "triggers"
			|| childName == "trigger")
		{
			ReadChildForDialogueActions(child);
		}
	}
}

void DialogueChoice::ReadChildForDialogueActions(const XMLNode& node)
{
	for (int childIdx = 0; childIdx < node.nChildNode(); childIdx++)
	{
		XMLNode child = node.getChildNode(childIdx);
		std::string childName = child.getName();
		DialogueTrigger* action = DialogueActionRegistrationHelper::CreateDialogueActionByName(childName, child);
		if (action != nullptr)
		{
			m_choiceActions.push_back(action);
		}
	}
}

void DialogueChoice::ReadChildForDialogueRequirements(const XMLNode& node)
{
	for (int childIdx = 0; childIdx < node.nChildNode(); childIdx++)
	{
		XMLNode child = node.getChildNode(childIdx);
		std::string childName = child.getName();
		DialogueRequirement* req =
			DialogueRequirementRegistrationHelper::CreateDialogueRequirementByName(childName, child);
		if (req != nullptr)
		{
			m_choiceRequirements.push_back(req);
		}
	}
}


//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------
//Renders and Updates
void DialogueChoice::Render(const Vector2& position, const std::vector<Material>& materials) const
{
	Matrix44 matrix;
	matrix.MatrixMakeIdentity();
	Matrix44::MakeEulerRotation(matrix, 0.f, 0.f, 0.f, position);
	for (size_t pageIdx = 0; pageIdx < materials.size() && pageIdx < m_textMeshes.size(); pageIdx++)
	{
		materials[pageIdx].SetModelMatrix(matrix);
		m_textMeshes[pageIdx]->DrawWith(&materials[pageIdx]);
	}
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------
//Setters
void DialogueChoice::SetDialogueChoice(const std::string& choice)
{
	m_dialogueChoice = choice;
	if (m_dialogueChoice.length() > s_DialogueChoiceCharacterLimit)
	{
		m_dialogueChoice = m_dialogueChoice.substr(0, s_DialogueChoiceCharacterLimit);
	}
}

void DialogueChoice::SetDialogueSegmentToOpenAfterSelected(const std::string segment)
{
	m_dialogueSegmentToOpenAfterSelected = segment;
}

void DialogueChoice::BuildMeshFromFont(Font* font)
{
	m_vertices.clear();
	m_indices.clear();
	if (font == nullptr)
	{
		return;
	}

	float dialogueSize = s_DialogueChoiceFontSize;

	//TODO, add in alternating dialogue sizes based off the number of chars.

	font->GetTextToRender(m_vertices, m_indices, Vector2(0.f, -0.3f), m_dialogueChoice,
		dialogueSize,
		Vector3(1.f, 0.f), Vector3(0.f, 1.f),
		m_choiceColor, Font_Center_Aligned, true, 0.5f);
	int pageCount = font->GetPageCount();
	for (size_t pageIdx = 0; pageIdx < (size_t)pageCount; pageIdx++)
	{
		if (m_textMeshes.size() < (size_t)pageCount)
		{
			m_textMeshes.push_back(new Mesh());
		}
		m_textMeshes[pageIdx]->CreateVAOID();
		m_textMeshes[pageIdx]->setVBO(m_vertices[pageIdx]);
		m_textMeshes[pageIdx]->setIBO(m_indices[pageIdx]);
	}

}

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------
//Getters
const std::string& DialogueChoice::GetDialogueChoice() const
{
	return m_dialogueChoice;
}

const std::string& DialogueChoice::GetDialogueSegmentToOpenAfterSelected() const
{
	return m_dialogueSegmentToOpenAfterSelected;
}

bool DialogueChoice::CalculateShouldThisChoiceBeAvailable() const
{
	for (size_t requirementIdx = 0; requirementIdx < m_choiceRequirements.size(); requirementIdx++)
	{
		DialogueRequirement* requirement = m_choiceRequirements[requirementIdx];
		if (requirement != nullptr && requirement->CheckIfRequirementIsMet() == false)
		{
			return false;
		}
	}
	return true;
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------
//Operations
void DialogueChoice::TriggerUniqueEvents()
{
	for (size_t idx = 0; idx < m_choiceActions.size(); idx++)
	{
		DialogueTrigger* action = m_choiceActions[idx];
		if (action == nullptr)
			continue;

		action->TriggerAction();
	}
}

void DialogueChoice::WriteDialogueChoiceToString(std::string& str, int indentationAmt) const
{
	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Dialogue Choice: " + m_dialogueChoice + "\n";
	indentationAmt += 3;

	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Dialogue Segment to Open: " + m_dialogueSegmentToOpenAfterSelected + "\n";

	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Dialogue Choice Color: " + MathToStringUtils::ToString(m_choiceColor) + "\n";

	int nextLevel = indentationAmt + 3;
	if (m_choiceRequirements.size() != 0)
	{
		GameDebuggingCommons::AddIndentation(str, indentationAmt);
		str += "Dialogue Choice Requirements: \n";
		for (size_t i = 0; i < m_choiceRequirements.size(); i++)
		{
			const DialogueRequirement* req = m_choiceRequirements[i];
			if (req == nullptr)
			{
				GameDebuggingCommons::AddIndentation(str, nextLevel);
				str += "NULL DIALOGUE REQUIREMENT\n";
			}
			else
			{
				req->WriteDialogueTriggerToString(str, nextLevel);
			}
		}
	}

	if (m_choiceActions.size() != 0)
	{
		GameDebuggingCommons::AddIndentation(str, indentationAmt);
		str += "Dialogue Choice Triggers: \n";

		for (size_t i = 0; i < m_choiceActions.size(); i++)
		{
			const DialogueTrigger* trigger = m_choiceActions[i];
			if (trigger == nullptr)
			{
				GameDebuggingCommons::AddIndentation(str, nextLevel);
				str += "NULL DIALOGUE TRIGGER\n";
			}
			else
			{
				trigger->WriteDialogueTriggerToString(str, nextLevel);
			}
		}
	}
}