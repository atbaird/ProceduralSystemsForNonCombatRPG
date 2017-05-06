#include "Game/GameEntities/Hair/HairTemplate.hpp"
#include "Game/GameEntities/Hair/Hair.hpp"
#include "Game/GameEntities/Hair/HairColorNameValuePair.hpp"
#include "Engine/Math/ExtendedMath.hpp"
#include "Engine/2D/SpriteResourceHandler.hpp"
#include "Game/Common/GameDebuggingCommons.hpp"

//--------------------------------------------------------------
//--------------------------------------------------------------
//--------------------------------------------------------------
//Constructors
HairTemplate::HairTemplate()
{
	m_sequences.resize(NUMBER_OF_DIRECTIONS4);
	m_SequenceNames.resize(NUMBER_OF_DIRECTIONS4);

	for (size_t dirIdx = 0; dirIdx < NUMBER_OF_DIRECTIONS4; dirIdx++)
	{
		m_sequences[dirIdx].resize(NUMBER_OF_ANIMATIONIDXES, nullptr);
		m_SequenceNames[dirIdx].resize(NUMBER_OF_ANIMATIONIDXES, "");
	}
}

HairTemplate::~HairTemplate()
{

}

//--------------------------------------------------------------
//--------------------------------------------------------------
//--------------------------------------------------------------
//Operations
Hair* HairTemplate::SpawnHair() const
{
	Hair* hair = new Hair(m_hairName, m_hairNode);
	if (m_hairColors.size() > 0)
	{
		int randomColor = GetRandomIntLessThan(m_hairColors.size());
		hair->SetColorSchemeChoice(m_hairColors[randomColor].m_ColorIndex);
		hair->SetHairColorName(m_hairColors[randomColor].m_ColorName);
	}
	hair->SetDialogueHairName(m_dialogueHairName);

	return hair;
}

Hair* HairTemplate::SpawnHair(const std::vector<std::string>& colorChoiceLimits) const
{
	Hair* hair = new Hair(m_hairName, m_hairNode);
	std::vector<HairColorNameValuePair> colorChoices;
	GetValidColorChoicesBasedOffLimit(colorChoiceLimits, colorChoices);

	if (colorChoices.size() > 0)
	{
		int randomColor = GetRandomIntLessThan(colorChoices.size());
		hair->SetColorSchemeChoice(colorChoices[randomColor].m_ColorIndex);
		hair->SetHairColorName(colorChoices[randomColor].m_ColorName);
	}
	hair->SetDialogueHairName(m_dialogueHairName);

	for (Directions4 dir = NORTH; dir < NUMBER_OF_DIRECTIONS4; dir = (Directions4)(dir + 1))
	{
		for (int animIdx = 0; (size_t)animIdx < m_sequences[dir].size(); animIdx++)
		{
			hair->AddAnimationSequence(m_sequences[dir][animIdx], dir, animIdx);
		}
	}

	return hair;
}

void HairTemplate::GetValidColorChoicesBasedOffLimit(
	const std::vector<std::string>& colorChoiceLimits,
	std::vector<HairColorNameValuePair>& outColorChoices) const
{
	if (colorChoiceLimits.size() == 0)
	{
		return;
	}

	for (size_t colorChoiceIdx = 0; colorChoiceIdx < colorChoiceLimits.size(); colorChoiceIdx++)
	{
		std::string currentColorWanted = MakeLower(colorChoiceLimits[colorChoiceIdx]);
		for (size_t hairColorIdx = 0; hairColorIdx < m_hairColors.size(); hairColorIdx++)
		{
			HairColorNameValuePair hairColor = m_hairColors[hairColorIdx];
			if (currentColorWanted == hairColor.m_LowerCaseColorName)
			{
				outColorChoices.push_back(hairColor);
				break;
			}
		}
	}
}


//--------------------------------------------------------------
//--------------------------------------------------------------
//--------------------------------------------------------------
//Setters
void HairTemplate::SetAndReadXMLNode(const XMLNode& node)
{
	m_hairNode = node;
	ReadXMLNodeAttributes(node);
	ReadXMLNodeChildren(node);
}

void HairTemplate::ReadXMLNodeAttributes(const XMLNode& node)
{
	for (int attrIdx = 0; attrIdx < node.nAttribute(); attrIdx++)
	{
		XMLAttribute attr = node.getAttribute(attrIdx);
		if (SimpleStrCmpLower(attr.lpszName, "Name"))
		{
			m_hairName = std::string(attr.lpszValue);
		}
		else if (SimpleStrCmpLower(attr.lpszName, "dialogueName"))
		{
			m_dialogueHairName = std::string(attr.lpszValue);
		}
		else if (SimpleStrCmpLower(attr.lpszName, "isDefault")
			|| SimpleStrCmpLower(attr.lpszName, "default"))
		{
			std::string val = MakeLower(attr.lpszValue);
			if (val == "false"
				|| val == "f")
			{
				m_isDefaultHair = false;
			}
			else if (val == "true"
				|| val == "t")
			{
				m_isDefaultHair = true;
			}
		}
	}
}

void HairTemplate::ReadXMLNodeChildren(const XMLNode& node)
{
	for (int childIdx = 0; childIdx < node.nChildNode(); childIdx++)
	{
		XMLNode child = node.getChildNode(childIdx);
		std::string childName = MakeLower(child.getName());
		if (childName == "colors")
		{
			ReadColorsXMLNode(child);
		}
		else if (childName == "animations")
		{
			ReadAnimationXMLNode(child);
		}
	}
}

void HairTemplate::ReadColorsXMLNode(const XMLNode& node)
{
	for (int colorIdx = 0; colorIdx < node.nChildNode(); colorIdx++)
	{
		HairColorNameValuePair colorValPair = HairColorNameValuePair();
		colorValPair.m_HairName = m_hairName;

		XMLNode color = node.getChildNode(colorIdx);
		colorValPair.m_ColorName = color.getName();
		colorValPair.m_LowerCaseColorName = MakeLower(colorValPair.m_ColorName);

		for (int attrIdx = 0; attrIdx < color.nAttribute(); attrIdx++)
		{
			XMLAttribute attr = color.getAttribute(attrIdx);
			std::string attrName = MakeLower(attr.lpszName);
			if (attrName == "value")
			{
				colorValPair.m_ColorIndex = atoi(attr.lpszValue);
			}
		}
		m_hairColors.push_back(colorValPair);
	}
}

void HairTemplate::ReadAnimationXMLNode(const XMLNode& node)
{
	for (int animIdx = 0; animIdx < node.nChildNode(); animIdx++)
	{
		XMLNode anim = node.getChildNode(animIdx);
		Directions4 dir = INVALID_DIR4;
		int animIdxVal = -1;
		std::string animName = "";
		SpriteAnimationSequence* sequence = nullptr;
		for (int attrIdx = 0; attrIdx < anim.nAttribute(); attrIdx++)
		{
			XMLAttribute attr = anim.getAttribute(attrIdx);
			if (SimpleStrCmpLower(attr.lpszName, "animationName"))
			{
				animName = std::string(attr.lpszValue);
				sequence = SpriteResourceHandler::GetSpriteAnimationSequenceByName(animName);
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
		AddSpriteAnimationSequence(sequence, dir, animIdxVal, animName);
	}
}

void HairTemplate::SetHairName(const std::string& name)
{
	m_hairName = name;
}

void HairTemplate::AddSpriteAnimationSequence(SpriteAnimationSequence* sequence, Directions4 dir, int idx,
	const std::string& str)
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
		m_SequenceNames[dir].resize(idx + 1);
	}
	m_sequences[dir][idx] = sequence;
	m_SequenceNames[dir][idx] = str;
}

//--------------------------------------------------------------
//--------------------------------------------------------------
//--------------------------------------------------------------
//Getters
void HairTemplate::WriteGeneratorToString(std::string& str, int indentationLevel) const
{
	GameDebuggingCommons::AddIndentation(str, indentationLevel);
	str += "\n!---!\n";
	GameDebuggingCommons::AddIndentation(str, indentationLevel);
	str += "Hair Generator: " + m_hairName + "\n";

	indentationLevel += 3;
	GameDebuggingCommons::AddIndentation(str, indentationLevel);
	str += "Hair dialogue name: " + m_dialogueHairName + "\n";

	GameDebuggingCommons::AddIndentation(str, indentationLevel);
	str += "Is a default Hair: "; 
	GameDebuggingCommons::WriteBoolOntoString(str, m_isDefaultHair);

	GameDebuggingCommons::AddIndentation(str, indentationLevel);
	str += "Animations per Direction\n";
	int nextTierIndentation = indentationLevel + 3;
	int animationTierIndentation = nextTierIndentation + 3;
	for (int dir = 0; (size_t)dir < m_SequenceNames.size(); dir++)
	{
		switch (dir)
		{
		case NORTH:
			GameDebuggingCommons::AddIndentation(str, nextTierIndentation);
			str += "NORTH\n";
			break;
		case EAST:
			GameDebuggingCommons::AddIndentation(str, nextTierIndentation);
			str += "EAST\n";
			break;
		case WEST:
			GameDebuggingCommons::AddIndentation(str, nextTierIndentation);
			str += "WEST\n";
			break;
		case SOUTH:
			GameDebuggingCommons::AddIndentation(str, nextTierIndentation);
			str += "SOUTH\n";
			break;
		default:
			break;
		}
		for (int animIdx = 0; (size_t)animIdx < m_SequenceNames[dir].size(); animIdx++)
		{
			GameDebuggingCommons::AddIndentation(str, animationTierIndentation);
			str += "Animation: " + m_SequenceNames[dir][animIdx] + "\n";
		}
	}
}

const XMLNode& HairTemplate::GetXMLNode() const
{
	return m_hairNode;
}

const std::string& HairTemplate::GetHairName() const
{
	return m_hairName;
}

bool HairTemplate::IsDefaultHair() const
{
	return m_isDefaultHair;
}