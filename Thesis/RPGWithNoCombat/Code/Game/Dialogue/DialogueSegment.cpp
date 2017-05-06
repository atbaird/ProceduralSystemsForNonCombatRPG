#include "Game/Dialogue/DialogueSegment.hpp"
#include "Engine/Core/FileUtils.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Engine/2D/SpriteResourceHandler.hpp"
#include "Game/Dialogue/DialogueRequirements/DialogueRequirement.hpp"
#include "Game/Dialogue/DialogueActions/DialogueTrigger.hpp"
#include "Game/Dialogue/DialogueRequirements/DialogueRequirementRegistrationHelper.hpp"
#include "Game/Dialogue/DialogueActions/DialogueActionRegistrationHelper.hpp"
#include "Engine/Core/Performance/Performance.hpp"
#include "Game/TheGame.hpp"
#include "Game/Common/GameDebuggingCommons.hpp"
#include "Engine/Math/MathToStringUtils.hpp"

//-------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------
//Static Variables
STATIC Matrix44 DialogueSegment::s_UpArrowMatrix;
STATIC Matrix44 DialogueSegment::s_DownArrowMatrix;
STATIC Matrix44 DialogueSegment::s_SelectionMatrix;
STATIC Mesh DialogueSegment::s_UpArrowMesh;
STATIC Mesh DialogueSegment::s_DownArrowMesh;
STATIC Mesh DialogueSegment::s_SelectionMesh;
STATIC const Vector2 DialogueSegment::s_UpArrowChoicePosition = Vector2(5.3f, 3.2f);
STATIC Vector2 DialogueSegment::s_SelectionPosition = Vector2(5.3f, 3.2f);
STATIC const Vector2 DialogueSegment::s_Choice1ArrowChoicePosition = Vector2(5.3f, 2.1f);
STATIC const Vector2 DialogueSegment::s_Choice2ArrowChoicePosition = Vector2(5.3f, 1.1f);
STATIC const Vector2 DialogueSegment::s_Choice3ArrowChoicePosition = Vector2(5.3f, 0.1f);
STATIC const Vector2 DialogueSegment::s_DownArrowChoicePosition = Vector2(5.3f, -0.5f);
STATIC std::vector<DialogueGroupings>* DialogueSegment::s_SpecialDialogueGroupings = nullptr;
STATIC std::vector<DialogueSegment>* DialogueSegment::s_DialogueSegments = nullptr;
STATIC DialogueSegmentAndGroupingLoadData* DialogueSegment::s_loadingData = nullptr;
STATIC const SpeechBubble* DialogueSegment::s_speechBubble = nullptr;
STATIC Font* DialogueSegment::s_defaultFont = nullptr;
STATIC Material* DialogueSegment::s_UpArrowMaterial = nullptr;
STATIC Material* DialogueSegment::s_DownArrowMaterial = nullptr;
STATIC Material* DialogueSegment::s_SelectionMaterial = nullptr;
STATIC ShaderProgram* DialogueSegment::s_TextShader = nullptr;
STATIC int DialogueSegment::s_MaxCharactersPerLine = 30;
STATIC int DialogueSegment::s_MaxLinesPerDialogueBox = 2;

//-------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------
//Constructors
DialogueSegment::DialogueSegment()
{
	m_speechBubble = s_speechBubble;
	m_font = s_defaultFont;
	if (m_font == nullptr)
	{
		m_textMats.clear();
	}
	m_generalMatrix.MatrixMakeIdentity();
	Matrix44::MakeEulerRotation(m_generalMatrix, 0.f, 0.f, 0.f, Vector2(0.f, 0.f));
}

DialogueSegment::DialogueSegment(const XMLNode& node)
{
	m_speechBubble = s_speechBubble;
	m_font = s_defaultFont;
	m_DialogueSegmentName = node.getName();
	ReadXMLAttributes(node);
	ReadXMLChildren(node);
	

	if (m_font == nullptr)
	{
		m_textMats.clear();
	}
	else
	{
		m_textMats.resize(m_font->GetPageCount());
	}
	m_generalMatrix.MatrixMakeIdentity();
	Matrix44::MakeEulerRotation(m_generalMatrix, 0.f, 0.f, 0.f, Vector2(0.f, 0.f));
	ParseText();
	TextPiece::CompileStringFromGroupOfTextPieces(m_dialoguePiecesPerThisDialogueSegment,
		s_MaxCharactersPerLine,
		s_MaxLinesPerDialogueBox,
		m_textLinesInGroupsOfLinesPerPage);
	UpdateMeshWithCurrentTextSegment();
	RecalculateAvailableDialogueChoices();
}

DialogueSegment::DialogueSegment(const DialogueSegment& other)
	: m_generalMatrix(other.m_generalMatrix),
	m_textMats(other.m_textMats),
	m_DialogueSegmentName(other.m_DialogueSegmentName),
	m_FullUnparsedText(other.m_FullUnparsedText),
	m_CurrentTextForFont(other.m_CurrentTextForFont),
	m_SpecialGrouping(other.m_SpecialGrouping),
	m_lowerCaseGroupingName(other.m_lowerCaseGroupingName),
	m_fontColor(other.m_fontColor),
	m_speechBubble(other.m_speechBubble),
	m_dialoguePiecesPerThisDialogueSegment(other.m_dialoguePiecesPerThisDialogueSegment),
	m_vertices(other.m_vertices),
	m_indices(other.m_indices),
	m_dialogueChoices(other.m_dialogueChoices),
	m_ActiveDialogueChoices(other.m_ActiveDialogueChoices),
	m_font(other.m_font),
	m_CurrentTextSegmentToRender(other.m_CurrentTextSegmentToRender),
	m_currentPlayerSelection(other.m_currentPlayerSelection),
	m_choicePage(other.m_choicePage),
	m_numberOfChoicePages(other.m_numberOfChoicePages),
	m_numberOfActiveChoicePages(other.m_numberOfActiveChoicePages),
	m_renderChoices(other.m_renderChoices),
	m_finished(other.m_finished),
	m_dialogueSegmentToOpenNext(other.m_dialogueSegmentToOpenNext)
{
	for (size_t requirementIdx = 0; requirementIdx < other.m_DialogueRequirementsForIfInADialogueGroup.size();
		requirementIdx++)
	{
		DialogueRequirement* requirement = other.m_DialogueRequirementsForIfInADialogueGroup[requirementIdx];
		if (requirement != nullptr)
		{
			DialogueRequirement* copy = requirement->Clone();
			m_DialogueRequirementsForIfInADialogueGroup.push_back(copy);
		}
	}

	for (size_t actionIdx = 0; actionIdx < other.m_actionsForWhenDialogueClosed.size(); actionIdx++)
	{
		DialogueTrigger* action = other.m_actionsForWhenDialogueClosed[actionIdx];
		if (action != nullptr)
		{
			DialogueTrigger* copy = action->Clone();
			m_actionsForWhenDialogueClosed.push_back(copy);
		}
	}

	m_textMeshes.resize(other.m_textMeshes.size(), nullptr);
	for (size_t i = 0; i < other.m_textMeshes.size(); i++)
	{
		if (m_textMeshes[i] == nullptr)
		{
			m_textMeshes[i] = new Mesh();
		}
	}
}

DialogueSegment::~DialogueSegment()
{
	for (size_t requirementIdx = 0; requirementIdx < m_DialogueRequirementsForIfInADialogueGroup.size(); requirementIdx++)
	{
		DialogueRequirement* req = m_DialogueRequirementsForIfInADialogueGroup[requirementIdx];
		if (req != nullptr)
		{
			delete req;
		}
	}
	m_DialogueRequirementsForIfInADialogueGroup.clear();

	for (size_t actionIdx = 0; actionIdx < m_actionsForWhenDialogueClosed.size(); actionIdx++)
	{
		DialogueTrigger* action = m_actionsForWhenDialogueClosed[actionIdx];
		if (action != nullptr)
		{
			delete action;
		}
	}
	m_actionsForWhenDialogueClosed.clear();

	for (size_t i = 0; i < m_textMeshes.size(); i++)
	{
		Mesh* mesh = m_textMeshes[i];
		if (mesh != nullptr)
		{
			delete mesh;
		}
	}

	m_textMeshes.clear();
	m_dialogueChoices.clear();
}

void DialogueSegment::ReadXMLAttributes(const XMLNode& node)
{
	for (int attrIdx = 0; attrIdx < node.nAttribute(); attrIdx++)
	{
		XMLAttribute attr = node.getAttribute(attrIdx);
		std::string attrName = MakeLower(attr.lpszName);
		if (attrName == "text")
		{
			m_FullUnparsedText = attr.lpszValue;
		}
		else if (attrName == "font")
		{
			//Make this better.
			std::string val = (attr.lpszValue);
			Font* font = Font::CreateOrGetFont(val);
			if (font != nullptr)
			{
				m_font = font;
			}
		}
		else if (attrName == "speechbubble")
		{
			std::string val = MakeLower(attr.lpszValue);
			m_speechBubble = SpeechBubble::GetSpeechBubbleByName(val);
		}
		else if (attrName == "grouping"
			|| attrName == "uniquegrouping"
			|| attrName == "group"
			|| attrName == "uniquegroup")
		{
			m_SpecialGrouping = std::string(attr.lpszValue);
			m_lowerCaseGroupingName = MakeLower(m_SpecialGrouping);
			RegisterDialogueSegmentNameIntoSpecialGrouping(m_SpecialGrouping, m_DialogueSegmentName);
		}
		else if (attrName == "dialoguetoopennext"
			|| attrName == "dialoguesegmenttoopennext"
			|| attrName == "segmenttoopennext")
		{
			m_dialogueSegmentToOpenNext = std::string(attr.lpszValue);
		}
	}
}

void DialogueSegment::ReadXMLChildren(const XMLNode& node)
{
	for (int childIdx = 0; childIdx < node.nChildNode(); childIdx++)
	{
		XMLNode child = node.getChildNode(childIdx);
		std::string childName = MakeLower(child.getName());
		if (childName == "textblock"
			|| childName == "text")
		{
			for (int attrIdx = 0; attrIdx < child.nAttribute(); attrIdx++)
			{
				XMLAttribute attr = child.getAttribute(attrIdx);
				std::string attrName = MakeLower(attr.lpszName);
				if (attrName == "value"
					|| attrName == "val"
					|| attrName == "text")
				{
					m_FullUnparsedText += std::string(attr.lpszValue);
				}
			}
		}
		else if (childName == "decision"
			|| childName == "choice")
		{
			AddXMLChoices(child);
		}
		else if (childName == "requirement"
			|| childName == "requirements")
		{
			AddXMLRequirementsForWhenInGroup(child);
		}
		else if (childName == "actions"
			|| childName == "action"
			|| childName == "triggers"
			|| childName == "trigger")
		{
			ReadChildForDialogueActionsWhenDialogueCloses(child);
		}
	}
}

void DialogueSegment::AddXMLChoices(const XMLNode& node)
{
	for (int childIdx = 0; childIdx < node.nChildNode(); childIdx++)
	{
		XMLNode child = node.getChildNode(childIdx);
		m_dialogueChoices.push_back(DialogueChoice(child));
	}
	for (size_t index = 0; index < m_dialogueChoices.size(); index++)
	{
		m_dialogueChoices[index].BuildMeshFromFont(m_font);
	}
	m_numberOfChoicePages = (m_dialogueChoices.size() / s_ChoicesPerPage);
	if (m_dialogueChoices.size() % s_ChoicesPerPage != 0)
	{
		m_numberOfChoicePages += 1;
	}
}


void DialogueSegment::AddXMLRequirementsForWhenInGroup(const XMLNode& node)
{
	for (int childIdx = 0; childIdx < node.nChildNode(); childIdx++)
	{
		XMLNode child = node.getChildNode(childIdx);
		std::string childName = child.getName();
		DialogueRequirement* req =
			DialogueRequirementRegistrationHelper::CreateDialogueRequirementByName(childName, child);
		if (req != nullptr)
		{
			m_DialogueRequirementsForIfInADialogueGroup.push_back(req);
		}
	}
}

void DialogueSegment::ReadChildForDialogueActionsWhenDialogueCloses(const XMLNode& node)
{
	for (int childIdx = 0; childIdx < node.nChildNode(); childIdx++)
	{
		XMLNode child = node.getChildNode(childIdx);
		std::string childName = child.getName();
		DialogueTrigger* action = DialogueActionRegistrationHelper::CreateDialogueActionByName(childName, child);
		if (action != nullptr)
		{
			m_actionsForWhenDialogueClosed.push_back(action);
		}
	}
}


//-------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------
//Updates and Render
void DialogueSegment::Render() const
{
	if (m_speechBubble != nullptr)
	{
		m_speechBubble->Render();
		if (m_renderChoices)
		{
			m_speechBubble->RenderChoiceBubble();
		}
	}
	if (m_font != nullptr)
	{
		for (int pageIdx = 0; pageIdx < m_font->GetPageCount(); pageIdx++)
		{
			m_textMats[pageIdx].SetModelMatrix(m_generalMatrix);
			m_textMeshes[pageIdx]->DrawWith(&m_textMats[pageIdx]);
		}
	}

	if (m_renderChoices)
	{
		s_SelectionMaterial->SetModelMatrix(s_SelectionMatrix);
		s_SelectionMesh.DrawWith(s_SelectionMaterial);


		s_UpArrowMaterial->SetModelMatrix(s_UpArrowMatrix);
		s_UpArrowMesh.DrawWith(s_UpArrowMaterial);

		int choice1 = GetIndexOfActiveChoice(1);
		int choice2 = GetIndexOfActiveChoice(2);
		int choice3 = GetIndexOfActiveChoice(3);

		if (choice1 != -1)
		{
			m_dialogueChoices[choice1].Render(s_Choice1ArrowChoicePosition, m_textMats);
		}
		if (choice2 != -1)
		{
			m_dialogueChoices[choice2].Render(s_Choice2ArrowChoicePosition, m_textMats);
		}
		if (choice3 != -1)
		{
			m_dialogueChoices[choice3].Render(s_Choice3ArrowChoicePosition, m_textMats);
		}

		s_DownArrowMaterial->SetModelMatrix(s_DownArrowMatrix);
		s_DownArrowMesh.DrawWith(s_DownArrowMaterial);
	}
}

//-------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------
//Operations
void DialogueSegment::TriggerClosingActions()
{
	for (size_t actionIdx = 0; actionIdx < m_actionsForWhenDialogueClosed.size(); actionIdx++)
	{
		DialogueTrigger* action = m_actionsForWhenDialogueClosed[actionIdx];
		if (action != nullptr)
		{
			action->TriggerAction();
		}
	}
	if (m_lowerCaseGroupingName != ""
		&& s_SpecialDialogueGroupings != nullptr)
	{
		for (size_t groupingIdx = 0; groupingIdx < s_SpecialDialogueGroupings->size(); groupingIdx++)
		{
			DialogueGroupings& grouping = s_SpecialDialogueGroupings->at(groupingIdx);
			if (grouping.GetLowerCaseGroupingName() == m_lowerCaseGroupingName)
			{
				grouping.TriggerActionsForAfterMemberOfGroupingCloses();
			}
		}
	}
}

void DialogueSegment::MoveSelectionUp()
{
	m_currentPlayerSelection--;
	if (m_currentPlayerSelection < 0)
	{
		m_currentPlayerSelection = 0;
	}
	SetPositionOfSelected();
}

void DialogueSegment::MoveSelectionDown()
{
	m_currentPlayerSelection++;
	if (m_currentPlayerSelection > s_ChoicesPerPage + 1)
	{
		m_currentPlayerSelection = s_ChoicesPerPage + 1;
	}
	SetPositionOfSelected();
}

void DialogueSegment::ResetCurrentTextSegmentToRender()
{
	m_finished = false;
	m_renderChoices = false;
	m_CurrentTextSegmentToRender = 0;
	m_currentPlayerSelection = 0;
	m_choicePage = 0; 
	TextPiece::CompileStringFromGroupOfTextPieces(m_dialoguePiecesPerThisDialogueSegment,
		s_MaxCharactersPerLine,
		s_MaxLinesPerDialogueBox,
		m_textLinesInGroupsOfLinesPerPage);
	UpdateMeshWithCurrentTextSegment();
	SetPositionOfSelected();
	RecalculateAvailableDialogueChoices();
}

void DialogueSegment::RecalculateAvailableDialogueChoices()
{
	m_numberOfActiveChoicePages = 0;
	m_ActiveDialogueChoices.clear();
	if (m_dialogueChoices.size() == 0.f)
	{
		return;
	}
	m_ActiveDialogueChoices.reserve(m_dialogueChoices.size());
	for (size_t dialogueChoiceIdx = 0; dialogueChoiceIdx < m_dialogueChoices.size(); dialogueChoiceIdx++)
	{
		if (m_dialogueChoices[dialogueChoiceIdx].CalculateShouldThisChoiceBeAvailable() == true)
		{
			m_ActiveDialogueChoices.push_back(dialogueChoiceIdx);
		}
	}

	m_numberOfActiveChoicePages = (m_ActiveDialogueChoices.size() / s_ChoicesPerPage);
	if (m_ActiveDialogueChoices.size() % s_ChoicesPerPage != 0)
	{
		m_numberOfActiveChoicePages += 1;
	}
}

const DialogueChoice DialogueSegment::IncrementCurrentTextSegmentToRender()
{
	if ((size_t)m_CurrentTextSegmentToRender < m_textLinesInGroupsOfLinesPerPage.size())
	{
		m_CurrentTextSegmentToRender++;
	}
	return UpdateMeshWithCurrentTextSegment();
}

const DialogueChoice DialogueSegment::UpdateMeshWithCurrentTextSegment()
{
	for (size_t i = 0; i < m_dialogueChoices.size(); i++)
	{
		m_dialogueChoices[i].BuildMeshFromFont(m_font);
	}

	if ((size_t)m_CurrentTextSegmentToRender >= m_textLinesInGroupsOfLinesPerPage.size())
	{
		if (m_dialogueChoices.size() == 0)
		{
			m_finished = true;
			TriggerClosingActions();
			return DialogueChoice::s_EmptyChoice;
		}
		else
		{
			if (m_renderChoices)
			{
				if (m_currentPlayerSelection == 0)
				{
					m_choicePage--;
					if (m_choicePage < 0)
					{
						m_choicePage = m_numberOfActiveChoicePages - 1;
					}
				}
				else if (m_currentPlayerSelection == s_ChoicesPerPage + 1)
				{
					m_choicePage++;
					if (m_choicePage >= m_numberOfActiveChoicePages)
					{
						m_choicePage = 0;
					}
				}
				else
				{
					m_finished = true;
					TriggerClosingActions();
					int choice = GetIndexOfActiveChoice(m_currentPlayerSelection);
					m_dialogueChoices[choice].TriggerUniqueEvents();
					return m_dialogueChoices[choice];
				}

				return "";
			}
			else
			{
				m_renderChoices = true;
				return DialogueChoice::s_EmptyChoice;
			}
		}
	}
	m_CurrentTextForFont = m_textLinesInGroupsOfLinesPerPage[m_CurrentTextSegmentToRender];
	Vector2 centralBubblePosition = SpeechBubble::GetCentralBubblePosition();
	Vector2 screenSize = Vector2(g_TheGame->GetWindowWidth(), g_TheGame->GetWindowHeight());
	Vector2 halfScreenSize = screenSize * 0.5f;
	Vector2 sixthScreenSize = screenSize * (1.f / 6.f);
	Vector2 topLeft = (centralBubblePosition + Vector2(- halfScreenSize.x + 1.8f, sixthScreenSize.y - 1.2f));
	m_font->GetTextToRender(m_vertices, m_indices, topLeft , m_CurrentTextForFont, s_DialogueSegmentFontSize,
		Vector3(1.f, 0.f), Vector3(0.f, 1.f),
		m_fontColor, Font_LeftHand_Aligned, true, 0.5f);

	//Update Meshes
	int pageCount = m_font->GetPageCount();
	m_textMats.resize(pageCount);
	m_textMeshes.resize(pageCount, nullptr);
	for (int pageIdx = 0; pageIdx < pageCount; pageIdx++)
	{
		if (m_textMeshes[pageIdx] == nullptr)
		{
			m_textMeshes[pageIdx] = new Mesh();
		}

		m_textMeshes[pageIdx]->setVBO(m_vertices[pageIdx]);
		m_textMeshes[pageIdx]->setIBO(m_indices[pageIdx]);
		m_textMats[pageIdx].SetProgram(s_TextShader);
		const Texture* fontPage = m_font->GetPageForIdx(pageIdx);
		m_textMats[pageIdx].SetOrAddTexture(fontPage, 0, "gTextureDiffuse", 0);
	}
	return DialogueChoice::s_EmptyChoice;
}

void DialogueSegment::SetPositionOfSelected()
{
	switch (m_currentPlayerSelection)
	{
	case 0:
		s_SelectionPosition = s_UpArrowChoicePosition;
		break;
	case 1:
		s_SelectionPosition = s_Choice1ArrowChoicePosition;
		break;
	case 2:
		s_SelectionPosition = s_Choice2ArrowChoicePosition;
		break;
	case 3:
		s_SelectionPosition = s_Choice3ArrowChoicePosition;
		break;
	case s_ChoicesPerPage + 1:
		s_SelectionPosition = s_DownArrowChoicePosition;
		break;
	default:
		s_SelectionPosition = s_UpArrowChoicePosition;
		break;
	}

	RecalcSelectionMatrix();
}


//-------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------
//Setters
void DialogueSegment::MarkSegmentFinished()
{
	m_finished = true;
}

void DialogueSegment::SetDialogueSegmentName(const std::string& dialogueSegmentName)
{
	m_DialogueSegmentName = dialogueSegmentName;
}

void DialogueSegment::SetFullUnparsedTextAndParseIt(const std::string& fullUnparsedText)
{
	m_FullUnparsedText = fullUnparsedText;
	ParseText();
}

void DialogueSegment::SetSpeechBubble(const SpeechBubble* bubble)
{
	m_speechBubble = bubble;
}


void DialogueSegment::ParseText()
{
	m_dialoguePiecesPerThisDialogueSegment.clear();

	std::string currentLine = "";
	std::string currentWord = "";
	int lineCnt = 0;
	unsigned char car = ' ';
	unsigned char nextCar = ' ';
	bool hollaDollahMode = false; //Basically just means we have encountered an odd number of dollar signs.
	for (size_t charIdx = 0; charIdx < m_FullUnparsedText.length(); charIdx++)
	{
		bool addChar = true;
		car = m_FullUnparsedText[charIdx];
		if (charIdx != m_FullUnparsedText.size() - 1)
		{
			nextCar = m_FullUnparsedText[charIdx + 1];
		}
		else
		{
			nextCar = ' ';
		}

		if (charIdx == m_FullUnparsedText.length() - 1)
		{
			if (strcmp("" + car, ".") == 0)
			{
				size_t wordNum = (m_dialoguePiecesPerThisDialogueSegment.size() - 1);
				m_dialoguePiecesPerThisDialogueSegment[wordNum].SetHasAPeriodAtEnd(true);
				currentWord = "";
				break;
			}
			else if (strcmp("" + car, ",") == 0)
			{
				size_t wordNum = (m_dialoguePiecesPerThisDialogueSegment.size() - 1);
				m_dialoguePiecesPerThisDialogueSegment[wordNum].SetHasACommaAtEnd(true);
				currentWord = "";
				break;
			}
		}

		bool newLineDeclared = ((car == '\\' || car == '/') && nextCar == 'n');

		if (car == '$')
		{
			if (hollaDollahMode)
			{
				{
					currentWord += std::string(1, car);
				}
				car = nextCar;
				charIdx++;
				if (charIdx < m_FullUnparsedText.size() - 1)
				{
					nextCar = m_FullUnparsedText[charIdx + 1];
				}
				else
				{
					nextCar = ' ';
				}
				LoadInAPiece(currentWord, currentLine, charIdx, lineCnt, car, nextCar, hollaDollahMode);
				currentWord = "";
				if (charIdx != m_FullUnparsedText.length() - 1)
				{
					std::string str = m_FullUnparsedText.substr(charIdx, 1);
					if (str == "." || str == ",")
					{
						size_t wordNum = (m_dialoguePiecesPerThisDialogueSegment.size() - 1);
						if (str == ".")
						{
							m_dialoguePiecesPerThisDialogueSegment[wordNum].SetHasAPeriodAtEnd(true);
						}
						else if (str == ",")
						{
							m_dialoguePiecesPerThisDialogueSegment[wordNum].SetHasACommaAtEnd(true);
						}
						addChar = false;
					}
				}
				else
				{
					currentWord += std::string(1, car);
				}
				hollaDollahMode = false;
			}
			else
			{
				LoadInAPiece(currentWord, currentLine, charIdx, lineCnt, car, nextCar, hollaDollahMode);
				hollaDollahMode = true;
				currentWord += car;
			}
		}
		else if (hollaDollahMode == false && (car == ' ' || newLineDeclared))
		{
			LoadInAPiece(currentWord, currentLine, charIdx, lineCnt, car,  nextCar, hollaDollahMode);
		}
		else if(addChar)
		{
			currentWord += std::string(1, car);
		}
	}
	if (currentWord != "")
	{
		if (currentWord == ".")
		{
			size_t wordNum = (m_dialoguePiecesPerThisDialogueSegment.size() - 1);
			m_dialoguePiecesPerThisDialogueSegment[wordNum].SetHasAPeriodAtEnd(true);
			currentWord = "";
		}
		else if (currentWord == ",")
		{
			size_t wordNum = (m_dialoguePiecesPerThisDialogueSegment.size() - 1);
			m_dialoguePiecesPerThisDialogueSegment[wordNum].SetHasACommaAtEnd(true);
			currentWord = "";
		}
		else if (currentWord == " ")
		{
			size_t wordNum = (m_dialoguePiecesPerThisDialogueSegment.size() - 1);
			m_dialoguePiecesPerThisDialogueSegment[wordNum].SetTextProceededBy(SPACE);
			currentWord = "";
		}
		else
		{
			bool lineStartedANew = false;
			TextPiece piece(currentWord);
			{
				if ((currentLine.length() + piece.GetExpectedStringLength()) <= (size_t)s_MaxCharactersPerLine)
				{
					piece.SetTextProceededBy(NOTHING);
				}
				else
				{
					currentLine = "";
					lineStartedANew = true;
					piece.SetTextProceededBy(SPACE);
				}
			}
			if (lineStartedANew)
			{
				lineCnt++;
				if (lineCnt == s_MaxLinesPerDialogueBox)
				{
					lineCnt = 0;
				}
			}
			m_dialoguePiecesPerThisDialogueSegment.push_back(piece);

			for (size_t i = 0; i < piece.GetExpectedStringLength(); i++)
			{
				currentLine += ".";
			}
			currentWord = "";
		}
		
	}
}


void DialogueSegment::LoadInAPiece(std::string& currentWord, std::string& currentLine, size_t& charIdx,
	int& lineCnt, unsigned char& car, unsigned char& nextCar, bool hollahDollahModeCurrentlyOn)
{
	if (currentWord == "")
	{
		if (m_dialoguePiecesPerThisDialogueSegment.size() > 0)
		{
			if (car == ' ')
			{
				m_dialoguePiecesPerThisDialogueSegment[
					m_dialoguePiecesPerThisDialogueSegment.size() - 1].SetTextProceededBy(SPACE);
			}
			else
			{
				TextPiece piece(currentWord);
				m_dialoguePiecesPerThisDialogueSegment.push_back(piece);
			}
		}
		return;
	}
	lineCnt;
	bool usedCurrentChar = false;
	bool lineStartedANew = false;

	TextPiece piece(currentWord);

	if(!hollahDollahModeCurrentlyOn)
	{
		if (car == ' ' || car == '$')
		{
			usedCurrentChar = true;

			{
				if (car == ' ' && charIdx != m_FullUnparsedText.size() - 1)
				{
					piece.SetTextProceededBy(SPACE);
					currentLine += " ";
				}
				else
				{
					//piece.SetTextProceededBy(SPACE);
					
				}
			}
		}
		else if((car == '\\' || car == '/') && nextCar == 'n')
		{
			usedCurrentChar = true;
			currentLine = "";
			lineStartedANew = true;
			piece.SetTextProceededBy(NEW_LINE);
			charIdx++;
		}
	}
	else
	{
		if (car == ' ')
		{
			usedCurrentChar = true;
			{
				if (car == ' ' && charIdx != m_FullUnparsedText.size() - 1)
				{
					piece.SetTextProceededBy(SPACE);
					currentLine += " ";
				}
				else
				{
					piece.SetTextProceededBy(SPACE);
				}
			}
		}
		else if ((car == '\\' || car == '/') && nextCar == 'n')
		{
			usedCurrentChar = true;
			currentLine = "";
			lineStartedANew = true;
			piece.SetTextProceededBy(NEW_LINE);
			charIdx++;
		}
	}
	m_dialoguePiecesPerThisDialogueSegment.push_back(piece);
	currentLine += currentWord;
	currentWord = "";
	if (!usedCurrentChar)
	{
		currentWord += car;
	}
	else
	{
		currentWord = "";
	}
}

//-------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------
//Getters
const std::string& DialogueSegment::GetDialogueToOpenNext() const
{
	return m_dialogueSegmentToOpenNext;
}

const std::string& DialogueSegment::GetDialogueSegmentName() const
{
	return m_DialogueSegmentName;
}

const std::string& DialogueSegment::GetFullUnparsedText() const
{
	return m_FullUnparsedText;
}

const SpeechBubble* DialogueSegment::GetSpeechBubble() const
{
	return m_speechBubble;
}

const bool DialogueSegment::GetIsFinished() const
{
	return m_finished;
}

bool DialogueSegment::GetHasDialogueChoice() const
{
	return m_dialogueChoices.size() > 0;
}

int DialogueSegment::GetIndexOfChoice(int index) const
{
	return GetIndexOfChoiceWithPage(index, m_choicePage);
}

int DialogueSegment::GetIndexOfActiveChoice(int index) const
{
	return GetIndexOfActiveChoiceWithPage(index, m_choicePage);
}

int DialogueSegment::GetIndexOfChoiceWithPage(int index, int page) const
{
	if (index == 0 || index == s_ChoicesPerPage + 1)
	{
		return -1;
	}
	int calcedChoice = (index - 1) + (page * s_ChoicesPerPage);
	if ((size_t)calcedChoice >= m_dialogueChoices.size()
		|| calcedChoice < 0)
	{
		return -1;
	}

	return calcedChoice;
}

int DialogueSegment::GetIndexOfActiveChoiceWithPage(int index, int page) const
{
	if (index == 0 || index == s_ChoicesPerPage + 1)
	{
		return -1;
	}
	int calcedChoice = (index - 1) + (page * s_ChoicesPerPage);
	if ((size_t)calcedChoice >= m_ActiveDialogueChoices.size()
		|| calcedChoice < 0)
	{
		return -1;
	}

	return m_ActiveDialogueChoices[calcedChoice];
}

bool DialogueSegment::DetermineDialogueRequirementsMet() const
{
	for (size_t requirementIdx = 0; requirementIdx < m_DialogueRequirementsForIfInADialogueGroup.size(); requirementIdx++)
	{
		DialogueRequirement* requirement = m_DialogueRequirementsForIfInADialogueGroup[requirementIdx];
		if (requirement != nullptr && requirement->CheckIfRequirementIsMet() == false)
		{
			return false;
		}
	}

	return true;
}

void DialogueSegment::WriteDialogueSegmentToString(std::string& str, int indentationAmt) const
{
	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Dialogue Segment Name: " + m_DialogueSegmentName + "\n";
	indentationAmt += 3;

	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Full Unparsed Text: " + m_FullUnparsedText + "\n";

	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Dialogue To Open Next: " + m_dialogueSegmentToOpenNext + "\n";

	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Font Color: " + MathToStringUtils::ToString(m_fontColor) + "\n";

	str += "Speech Bubble: ";
	if (m_speechBubble == nullptr)
	{
		str += "NULL\n";
	}
	else
	{
		str += m_speechBubble->GetBubbleName() + "\n";
	}

	int nextLevel = indentationAmt + 3;
	if (m_DialogueRequirementsForIfInADialogueGroup.size() != 0)
	{
		GameDebuggingCommons::AddIndentation(str, indentationAmt);
		str += "Dialogue Requirements if in Group:\n";
		for (size_t i = 0; i < m_DialogueRequirementsForIfInADialogueGroup.size(); i++)
		{
			const DialogueRequirement* req = m_DialogueRequirementsForIfInADialogueGroup[i];
			if (req != nullptr)
			{
				req->WriteDialogueTriggerToString(str, nextLevel);
			}
			else
			{
				GameDebuggingCommons::AddIndentation(str, nextLevel);
				str += "NULL DIALOGUE REQUIREMENT\n";
			}
		}
	}

	if (m_actionsForWhenDialogueClosed.size() != 0)
	{
		GameDebuggingCommons::AddIndentation(str, indentationAmt);
		str += "Dialogue Triggers to perform when this segment closes:\n";
		for (size_t i = 0; i < m_actionsForWhenDialogueClosed.size(); i++)
		{
			const DialogueTrigger* trigger = m_actionsForWhenDialogueClosed[i];
			if (trigger != nullptr)
			{
				trigger->WriteDialogueTriggerToString(str, nextLevel);
			}
			else
			{
				GameDebuggingCommons::AddIndentation(str, nextLevel);
				str += "NULL DIALOGUE TRIGGER\n";
			}
		}
	}

	if (m_dialogueChoices.size() != 0)
	{
		GameDebuggingCommons::AddIndentation(str, indentationAmt);
		str += "Dialogue Choices: \n";
		for (size_t i = 0; i < m_dialogueChoices.size(); i++)
		{
			const DialogueChoice& choice = m_dialogueChoices[i];
			choice.WriteDialogueChoiceToString(str, nextLevel);
		}
	}

	//std::vector<DialogueChoice> m_dialogueChoices;
}

//-------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------
//Static Methods
STATIC void DialogueSegment::WriteAllDialogueGroupsToString(std::string& str, int indentationAmt)
{
	if (s_SpecialDialogueGroupings == nullptr)
	{
		return;
	}
	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Number of Dialogue Groups: " + std::to_string(s_SpecialDialogueGroupings->size()) + "\n";

	for (size_t i = 0; i < s_SpecialDialogueGroupings->size(); i++)
	{
		const DialogueGroupings& group = s_SpecialDialogueGroupings->at(i);
		group.WriteDialogueGroupToString(str, indentationAmt);
	}
}

STATIC void DialogueSegment::WriteAllDialogueSegmentsToString(std::string& str, int indentationAmt)
{
	if (s_DialogueSegments == nullptr)
	{
		return;
	}
	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Number of Dialogue Segments: " + std::to_string(s_DialogueSegments->size()) + "\n";

	for (size_t i = 0; i < s_DialogueSegments->size(); i++)
	{
		const DialogueSegment& segment = s_DialogueSegments->at(i);
		segment.WriteDialogueSegmentToString(str, indentationAmt);
	}

}

STATIC void DialogueSegment::ReadInAllDialogueSegmentsFromDefaultFolderAllAtOnce()
{
	ReadInAllDialogueGroupingsFromDefaultFolder();
	SetUpCommonMeshes();

	std::string commonDialogueSegmentsXMLFile = "Data/XML/DialogueSegments/";
	std::string dialogueSegmentsRootXMLName = "DialogueSegments";
	GameSettings* settings = TheGame::GetGameSettings();
	if (settings != nullptr)
	{
		TextPiece::SetAddThreeDotsAtTheEndOfDialogueSegment(settings->m_AddThreeDotsAfterPushFullDialogue);
		s_MaxCharactersPerLine = settings->m_numberOfCharactersPerLine;
		commonDialogueSegmentsXMLFile = settings->m_commonDialogueSegmentsXMLFolder;
		dialogueSegmentsRootXMLName = settings->m_dialogueSegmentsRootXML;
		s_MaxCharactersPerLine = settings->m_numberOfCharactersPerLine;
	}

	std::vector<std::string> files = FileUtils::EnumerateFilesInDirectory(commonDialogueSegmentsXMLFile, "*", true);

	for (size_t fileIdx = 0; fileIdx < files.size(); fileIdx++)
	{
		std::string fileStr = files.at(fileIdx);
		ReadDialogueSegmentFromXMLFileLocation(fileStr, dialogueSegmentsRootXMLName);
	}
}

STATIC bool DialogueSegment::OnUpdateReadInAllDialogueSegmentsFromDefaultFolder()
{
	if (s_loadingData == nullptr)
	{
		s_loadingData = new DialogueSegmentAndGroupingLoadData();
		SetUpCommonMeshes();
	}
	if (s_loadingData->m_readingDialogueGroupings)
	{
		if (OnUpdateReadInAllDialogueGroupingsFromDefaultFolder())
		{
			s_loadingData->m_readingDialogueGroupings = false;
			s_loadingData->m_index = 0;
		}
		return false;
	}
	else
	{
		return ActuallyOnUpdateReadInAllDialogueSegmentsFromDefaultFolder();
	}
}

STATIC bool DialogueSegment::ActuallyOnUpdateReadInAllDialogueSegmentsFromDefaultFolder()
{
	if (s_loadingData == nullptr)
	{
		//Can't do anything.
		return true;
	}
	GameSettings* settings = TheGame::GetGameSettings();
	if (!s_loadingData->m_readDialogueSegmentFiles)
	{
		if (settings != nullptr)
		{
			TextPiece::SetAddThreeDotsAtTheEndOfDialogueSegment(settings->m_AddThreeDotsAfterPushFullDialogue);
			s_MaxCharactersPerLine = settings->m_numberOfCharactersPerLine;
			s_loadingData->m_commonDialogueSegmentsXMLFile = settings->m_commonDialogueSegmentsXMLFolder;
			s_loadingData->m_dialogueSegmentsRootXMLName = settings->m_dialogueSegmentsRootXML;
			s_loadingData->m_amountOfTimeToTakeReadingDialogueSegmentsPerFrame = 
				settings->m_amountOfTimeLoadingDialogueSegmentsPerFrame;
		}
		s_loadingData->m_readDialogueSegmentFiles = true;
		s_loadingData->m_index = 0;
		s_loadingData->m_dialogueSegmentFiles =
			FileUtils::EnumerateFilesInDirectory(s_loadingData->m_commonDialogueSegmentsXMLFile, "*", true);
		if (s_loadingData->m_dialogueSegmentFiles.size() == 0)
		{
			return true;
		}
		return false;
	}
	if (s_loadingData->m_amountOfTimeToTakeReadingDialogueSegmentsPerFrame <= 0)
	{
		return true;
	}
	int numberOfSegmentsReadThisFrame = 0;
	while ((size_t)s_loadingData->m_index < s_loadingData->m_dialogueSegmentFiles.size())
	{
		std::string fileStr = s_loadingData->m_dialogueSegmentFiles[s_loadingData->m_index];
		ReadDialogueSegmentFromXMLFileLocation(fileStr, s_loadingData->m_dialogueSegmentsRootXMLName);
		s_loadingData->m_index++;
		numberOfSegmentsReadThisFrame++;
		uint64_t timeDiff = Performance::GetCurrentPerformanceCount() - g_TheGame->GetFrameStartTime();
		double timeDiffDouble = Performance::PerformanceCountToSeconds(timeDiff);
		if (timeDiffDouble >= s_loadingData->m_amountOfTimeToTakeReadingDialogueSegmentsPerFrame)
		{
			return false;
		}
	}
	if ((size_t)s_loadingData->m_index >= s_loadingData->m_dialogueSegmentFiles.size())
	{
		return true;
	}
	return false;
}

STATIC void DialogueSegment::ReadDialogueSegmentFromXMLFileLocation(const std::string& fileStr,
	const std::string& dialogueSegmentsRootXMLName)
{
	std::string last3Letters = fileStr.substr(fileStr.size() - 3, 3);
	if (SimpleStrCmpLower(last3Letters, "xml") == false)
	{
		return;
	}

	bool errorOccurred = false;
	XMLNode root = EngineXMLParser::ParseXMLFile(fileStr, dialogueSegmentsRootXMLName.c_str(), errorOccurred);
	if (errorOccurred == true)
	{
		return;
	}

	for (int attrIdx = 0; attrIdx < root.nAttribute(); attrIdx++)
	{
		XMLAttribute attr = root.getAttribute(attrIdx);
		std::string attrName = MakeLower(attr.lpszName);
		if (attrName == "speechbubble")
		{
			s_speechBubble = SpeechBubble::GetSpeechBubbleByName(attr.lpszValue);
		}
	}
	for (int childIdx = 0; childIdx < root.nChildNode(); childIdx++)
	{
		XMLNode child = root.getChildNode(childIdx);
		{
			if (s_DialogueSegments == nullptr)
			{
				s_DialogueSegments = new std::vector<DialogueSegment>();
			}
			s_DialogueSegments->push_back(DialogueSegment(child));

		}
	}
}

STATIC void DialogueSegment::ReadInAllDialogueGroupingsFromDefaultFolder()
{
	std::string dialogueGroupingsCommonFolder = "Data/XML/DialogueGroupings/";
	std::string dialogueGroupingsRootXMLName = "DialogueGroupings";
	GameSettings* settings = TheGame::GetGameSettings();
	if (settings != nullptr)
	{
		dialogueGroupingsCommonFolder = settings->m_commonDialogueGroupingsXMLFolder;
		dialogueGroupingsRootXMLName = settings->m_dialogueGroupingsRootXML;
	}

	std::vector<std::string> files = FileUtils::EnumerateFilesInDirectory(dialogueGroupingsCommonFolder, "*", true);
	for (size_t fileIdx = 0; fileIdx < files.size(); fileIdx++)
	{
		std::string fileStr = files.at(fileIdx);
		ReadDialogueGroupFromXMLFileLocation(fileStr, dialogueGroupingsRootXMLName);
	}
}

STATIC bool DialogueSegment::OnUpdateReadInAllDialogueGroupingsFromDefaultFolder()
{
	if (s_loadingData == nullptr)
	{
		//Can't do anything.
		return true;
	}
	GameSettings* settings = TheGame::GetGameSettings();
	if (!s_loadingData->m_readDialogueGroupingFiles)
	{
		if (settings != nullptr)
		{
			TextPiece::SetAddThreeDotsAtTheEndOfDialogueSegment(settings->m_AddThreeDotsAfterPushFullDialogue);
			s_loadingData->m_commonDialogueGroupingsXMLFile = settings->m_commonDialogueGroupingsXMLFolder;
			s_loadingData->m_dialogueGroupingsRootXMLName = settings->m_dialogueGroupingsRootXML;
			s_loadingData->m_amountOfTimeToTakeReadingDialogueGroupingsPerFrame 
				= settings->m_amountOfTimeLoadingDialogueGroupingsPerFrame;
		}
		s_loadingData->m_readDialogueGroupingFiles = true;
		s_loadingData->m_index = 0;
		s_loadingData->m_dialogueGroupingFiles =
			FileUtils::EnumerateFilesInDirectory(s_loadingData->m_commonDialogueGroupingsXMLFile, "*", true);
		if (s_loadingData->m_dialogueGroupingFiles.size() == 0)
		{
			return true;
		}
		return false;
	}
	if (s_loadingData->m_amountOfTimeToTakeReadingDialogueGroupingsPerFrame <= 0)
	{
		return true;
	}
	int numberOfGroupingsReadThisFrame = 0;
	while (numberOfGroupingsReadThisFrame < s_loadingData->m_amountOfTimeToTakeReadingDialogueGroupingsPerFrame
		&& (size_t)s_loadingData->m_index < s_loadingData->m_dialogueGroupingFiles.size())
	{
		std::string fileLocation = s_loadingData->m_dialogueGroupingFiles[s_loadingData->m_index];
		ReadDialogueGroupFromXMLFileLocation(fileLocation, s_loadingData->m_dialogueGroupingsRootXMLName);
		s_loadingData->m_index++;
		numberOfGroupingsReadThisFrame++;
		uint64_t timeDiff = Performance::GetCurrentPerformanceCount() - g_TheGame->GetFrameStartTime();
		double timeDiffDouble = Performance::PerformanceCountToSeconds(timeDiff);
		if (timeDiffDouble >= s_loadingData->m_amountOfTimeToTakeReadingDialogueGroupingsPerFrame)
		{
			return false;
		}
	}
	if ((size_t)s_loadingData->m_index >= s_loadingData->m_dialogueGroupingFiles.size())
	{
		return true;
	}
	return false;
}

STATIC void DialogueSegment::ReadDialogueGroupFromXMLFileLocation(const std::string& fileStr,
	const std::string& dialogueGroupingsRootXMLName)
{
	std::string last3Letters = fileStr.substr(fileStr.size() - 3, 3);
	if (SimpleStrCmpLower(last3Letters, "xml") == false)
	{
		return;
	}
	bool errorOccurred = false;
	XMLNode root = EngineXMLParser::ParseXMLFile(fileStr, dialogueGroupingsRootXMLName.c_str(), errorOccurred);
	if (errorOccurred == true)
	{
		return;
	}
	for (int childIdx = 0; childIdx < root.nChildNode(); childIdx++)
	{
		XMLNode child = root.getChildNode(childIdx);
		DialogueGroupings grouping = DialogueGroupings(child);
		if (s_SpecialDialogueGroupings == nullptr)
		{
			s_SpecialDialogueGroupings = new std::vector<DialogueGroupings>();
		}
		s_SpecialDialogueGroupings->push_back(grouping);
	}
}

STATIC void DialogueSegment::ClearAllDialogueSegments()
{
	if (s_loadingData != nullptr)
	{
		delete s_loadingData;
		s_loadingData = nullptr;
	}
	if (s_SelectionMaterial != nullptr)
	{
		delete s_SelectionMaterial;
		s_SelectionMaterial = nullptr;
	}

	if (s_DownArrowMaterial != nullptr)
	{
		delete s_DownArrowMaterial;
		s_DownArrowMaterial = nullptr;
	}
	if (s_UpArrowMaterial != nullptr)
	{
		delete s_UpArrowMaterial;
		s_UpArrowMaterial = nullptr;
	}
	if (s_SpecialDialogueGroupings != nullptr)
	{
		delete s_SpecialDialogueGroupings;
		s_SpecialDialogueGroupings = nullptr;
	}
	if (s_DialogueSegments == nullptr)
	{
		return;
	}
	delete s_DialogueSegments;
	s_DialogueSegments = nullptr;
}

STATIC void DialogueSegment::SetDefaultFont(Font* font)
{
	s_defaultFont = font;
}

STATIC Font* DialogueSegment::GetDefaultFont()
{
	return s_defaultFont;
}

STATIC DialogueSegment* DialogueSegment::GetDialogueSegmentByName(const std::string& name)
{
	std::string nameCopy = MakeLower(name);
	if (s_DialogueSegments == nullptr)
	{
		return nullptr;
	}
	//
	{
		std::string testName = MakeLower(name);
		bool foundInGroupings = false;

		if (s_SpecialDialogueGroupings != nullptr)
		{
			for (size_t idx = 0; idx < s_SpecialDialogueGroupings->size(); idx++)
			{
				DialogueGroupings& grouping = s_SpecialDialogueGroupings->at(idx);
				if (grouping.m_lowerCaseGroupingName == testName)
				{
					foundInGroupings = true;
					nameCopy = MakeLower(grouping.GetRandomNameFromListingWithListeningForRequirements());
					break;
				}
			}
		}
	}
	

	for (size_t i = 0; i < s_DialogueSegments->size(); i++)
	{
		DialogueSegment* segment = &s_DialogueSegments->at(i);
		if (segment != nullptr && MakeLower(segment->GetDialogueSegmentName()) == nameCopy)
		{
			return segment;
		}
	}
	return nullptr;
}

STATIC bool DialogueSegment::RegisterDialogueSegmentByName(const std::string& name, DialogueSegment& segment)
{
	if (segment.GetDialogueSegmentName() != name)
	{
		return false;
	}

	if (s_DialogueSegments == nullptr)
	{
		s_DialogueSegments = new std::vector<DialogueSegment>();
	}

	for (size_t DialogueSegmentIdx = 0; DialogueSegmentIdx < s_DialogueSegments->size(); DialogueSegmentIdx++)
	{
		DialogueSegment* seg = &s_DialogueSegments->at(DialogueSegmentIdx);
		if (seg != nullptr && seg->GetDialogueSegmentName() == name)
		{
			return false;
		}
	}
	segment.m_DialogueSegmentName = name;
	s_DialogueSegments->push_back(segment);

	return true;
}

STATIC bool DialogueSegment::RegisterDialogueSegmentNameIntoSpecialGrouping(
	const std::string& groupingName, const std::string& DialogueSegmentName)
{
	std::string val = MakeLower(groupingName);
	if (s_SpecialDialogueGroupings == nullptr)
	{
		s_SpecialDialogueGroupings = new std::vector<DialogueGroupings>();
	}
	bool found = false;
	for (size_t i = 0; i < s_SpecialDialogueGroupings->size(); i++)
	{
		DialogueGroupings& grouping = s_SpecialDialogueGroupings->at(i);
		if (grouping.m_lowerCaseGroupingName == val)
		{
			grouping.AddDialogueSegmentName(DialogueSegmentName);
			found = true;
			return true;
		}
	}
	if (found == false)
	{
		DialogueGroupings grouping(val);
		grouping.AddDialogueSegmentName(DialogueSegmentName);
		s_SpecialDialogueGroupings->push_back(grouping);
	}
	return true;
}

STATIC void DialogueSegment::AddArrowIndices(std::vector<unsigned int>& indices)
{
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);
	indices.push_back(0);
	indices.push_back(2);
	indices.push_back(3);
}

STATIC void DialogueSegment::AddArrowQuad(std::vector<SpriteVert>& vertices, const AABB2& textureCoords,
	const Vector2& size)
{
	Vector2 halfSize = size * 0.5f;
	SpriteVert vert;
	vert.m_color = Rgba::s_White;

	//BL Position
	vert.m_texCoords = Vector2(textureCoords.mins.x, textureCoords.mins.y);
	vert.m_pos = Vector2(-halfSize.x, -halfSize.y);
	vertices.push_back(vert);

	//BR Position
	vert.m_texCoords = Vector2(textureCoords.maxs.x, textureCoords.mins.y);
	vert.m_pos = Vector2(halfSize.x, -halfSize.y);
	vertices.push_back(vert);

	//TR Position
	vert.m_texCoords = Vector2(textureCoords.maxs.x, textureCoords.maxs.y);
	vert.m_pos = Vector2(halfSize.x, halfSize.y);
	vertices.push_back(vert);

	//TL Position
	vert.m_texCoords = Vector2(textureCoords.mins.x, textureCoords.maxs.y);
	vert.m_pos = Vector2(-halfSize.x, halfSize.y);
	vertices.push_back(vert);
}

STATIC void DialogueSegment::RecalcSelectionMatrix()
{
	s_SelectionMatrix.MatrixMakeIdentity();
	Matrix44::MakeEulerRotation(s_SelectionMatrix, 0.f, 0.f, 0.f, s_SelectionPosition);
}

STATIC void DialogueSegment::SetUpCommonMeshes()
{
	s_UpArrowMatrix.MatrixMakeIdentity();
	s_DownArrowMatrix.MatrixMakeIdentity();
	Matrix44::MakeEulerRotation(s_UpArrowMatrix, 0.f, 0.f, 0.f, s_UpArrowChoicePosition);
	Matrix44::MakeEulerRotation(s_DownArrowMatrix, 0.f, 0.f, 0.f, s_DownArrowChoicePosition);
	RecalcSelectionMatrix();

	std::vector<unsigned int> indices;
	std::vector<SpriteVert> commonMesh;
	indices.reserve(6);
	commonMesh.reserve(4);
	AddArrowIndices(indices);
	s_UpArrowMesh.setIBO(indices);
	s_DownArrowMesh.setIBO(indices);
	s_SelectionMesh.setIBO(indices);

	const SpriteResource* upArrow = SpriteResourceHandler::GetSpriteResourceByName("upArrow");
	const SpriteResource* downArrow = SpriteResourceHandler::GetSpriteResourceByName("downArrow");
	const SpriteResource* selection = SpriteResourceHandler::GetSpriteResourceByName("currentSelection");

	AABB2 upArrowCoords = AABB2::ZERO_TO_ONE;
	AABB2 downArrowCoords = AABB2::ZERO_TO_ONE;
	AABB2 selectionCoords = AABB2::ZERO_TO_ONE;

	if (upArrow != nullptr)
	{
		upArrowCoords = upArrow->GetTextureCoords();
	}
	if (downArrow != nullptr)
	{
		downArrowCoords = downArrow->GetTextureCoords();
	}
	if (selection != nullptr)
	{
		selectionCoords = selection->GetTextureCoords();
	}


	AddArrowQuad(commonMesh, upArrowCoords);
	s_UpArrowMesh.setVBO(commonMesh);
	commonMesh.clear();

	AddArrowQuad(commonMesh, downArrowCoords);
	s_DownArrowMesh.setVBO(commonMesh);
	commonMesh.clear();

	Vector2 selectionRenderSize = Vector2(2.4f, 0.8f);
	GameSettings* gameSettings = TheGame::GetGameSettings();
	if (gameSettings != nullptr)
	{
		selectionRenderSize = gameSettings->m_selectionRenderSize;
	}

	AddArrowQuad(commonMesh, selectionCoords, selectionRenderSize);
	s_SelectionMesh.setVBO(commonMesh);
	commonMesh.clear();

	ShaderProgram* defaultProg = ShaderProgram::GetDefaultShaderProgram();
	s_UpArrowMaterial = new Material();
	s_DownArrowMaterial = new Material();
	s_SelectionMaterial = new Material();
	s_UpArrowMaterial->SetProgram(defaultProg);
	s_DownArrowMaterial->SetProgram(defaultProg);
	s_SelectionMaterial->SetProgram(defaultProg);

	if (upArrow != nullptr)
	{
		s_UpArrowMaterial->SetOrAddTexture(upArrow->GetSpriteSheet(), 0, "gTextureDiffuse", 0);
	}
	if (downArrow != nullptr)
	{
		s_DownArrowMaterial->SetOrAddTexture(downArrow->GetSpriteSheet(), 0, "gTextureDiffuse", 0);
	}
	if (selection != nullptr)
	{
		s_SelectionMaterial->SetOrAddTexture(selection->GetSpriteSheet(), 0, "gTextureDiffuse", 0);
	}
}

STATIC void DialogueSegment::SetDefaultTextShader(ShaderProgram* prog)
{
	s_TextShader = prog;
}

STATIC ShaderProgram* DialogueSegment::GetTextShaderProgram()
{
	return s_TextShader;
}