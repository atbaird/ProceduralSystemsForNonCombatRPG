#include "Game/Quest/QuestEvents/QuestTrigger/QuestTriggerPlaySoundFromList.hpp"
#include "Game/Common/GameDebuggingCommons.hpp"

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//Registration Funcs
QuestTrigger* QuestTriggerPlaySoundFromListFloatFunc(const XMLNode& node)
{
	return new QuestTriggerPlaySoundFromList(node);
}

QuestTrigger* QuestTriggerDefaultPlaySoundFromListFunc()
{
	return new QuestTriggerPlaySoundFromList();
}


//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//Static Variables
STATIC QuestTriggerRegistration
QuestTriggerPlaySoundFromList::s_QuestTriggerPlaySoundFromListRegistration
= QuestTriggerRegistration("PlaySoundFromList",
	QuestTriggerPlaySoundFromListFloatFunc,
	QuestTriggerDefaultPlaySoundFromListFunc);

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//Constructors
QuestTriggerPlaySoundFromList::QuestTriggerPlaySoundFromList()
	: QuestTrigger()
{

}

QuestTriggerPlaySoundFromList::QuestTriggerPlaySoundFromList(const XMLNode& node, Quest* owningQuest)
	: QuestTrigger(node, owningQuest)
{
	for (int attrIdx = 0; attrIdx < node.nAttribute(); attrIdx++)
	{
		XMLAttribute attr = node.getAttribute(attrIdx);
		std::string attrName = MakeLower(attr.lpszName);
		if (attrName == "action")
		{
			std::string val = MakeLower(attr.lpszValue);
			if (val == "random"
				|| val == "procedural")
			{
				m_action = PLAY_SOUND_RANDOM;
			}
			else if (val == "sequential"
				|| val == "sequence")
			{
				m_action = PLAY_SOUND_SEQUENTIAL;
			}
		}
	}

	for (int childIdx = 0; childIdx < node.nChildNode(); childIdx++)
	{
		XMLNode child = node.getChildNode(childIdx);
		std::string soundEffectName = "";
		float volume = 1.f;

		for (int childAttrIdx = 0; childAttrIdx < child.nAttribute(); childAttrIdx++)
		{
			XMLAttribute attr = child.getAttribute(childAttrIdx);
			std::string AttrName = MakeLower(attr.lpszName);
			if (AttrName == "soundeffect"
				|| AttrName == "sound"
				|| AttrName == "name")
			{
				soundEffectName = std::string(attr.lpszValue);
			}
			else if (AttrName == "volume"
				|| AttrName == "vol")
			{
				volume = (float)atof(attr.lpszValue);
			}
		}

		AddSoundEffectName(soundEffectName, volume);
	}
}

QuestTriggerPlaySoundFromList::QuestTriggerPlaySoundFromList(const QuestTriggerPlaySoundFromList& other,
	Quest* owningQuest)
	: QuestTrigger(other, owningQuest),
	m_indexLastPlayed(other.m_indexLastPlayed),
	m_soundEffectNames(other.m_soundEffectNames),
	m_action(other.m_action)
{

}

QuestTriggerPlaySoundFromList::~QuestTriggerPlaySoundFromList()
{
	QuestTrigger::~QuestTrigger();
}

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//Setters
void QuestTriggerPlaySoundFromList::AddSoundEffectName(const std::string& name,
	float volume)
{
	if (name == "")
	{
		return;
	}

	m_soundEffectNames.push_back(SoundEffectData(name, volume));
}

void QuestTriggerPlaySoundFromList::SetPlaySoundFromListAction(PlaySoundFromListActionEnum action)
{
	m_action = action;
}

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//Getters
const std::vector<SoundEffectData>& QuestTriggerPlaySoundFromList::GetSoundEffectNames() const
{
	return m_soundEffectNames;
}

int QuestTriggerPlaySoundFromList::GetIndexLastPlayed() const
{
	return m_indexLastPlayed;
}

PlaySoundFromListActionEnum QuestTriggerPlaySoundFromList::GetPlaySoundFromListAction() const
{
	return m_action;
}

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//Operations
bool QuestTriggerPlaySoundFromList::PerformQuestTrigger()
{
	if (m_soundEffectNames.size() == 0)
	{
		return false;
	}

	return PlaySoundAction::PlaySoundFromList(m_soundEffectNames, m_action, m_indexLastPlayed);
}

QuestTrigger* QuestTriggerPlaySoundFromList::Clone(Quest* owningQuest) const
{
	return new QuestTriggerPlaySoundFromList(*this, owningQuest);
}

void QuestTriggerPlaySoundFromList::WriteQuestTriggerToString(std::string& str, int indentationAmt) const
{
	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Quest Trigger Name: Play Sound From List\n";
	indentationAmt += 3;

	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Sound last played: " + std::to_string(m_indexLastPlayed) + "\n";

	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Sound selection mode: ";
	switch (m_action)
	{
	case PLAY_SOUND_INVALID:
		str += "INVALID\n";
		break;
	case PLAY_SOUND_RANDOM:
		str += "RANDOM\n";
		break;
	case PLAY_SOUND_SEQUENTIAL:
		str += "SEQUENTIAL\n";
		break;
	default:
		str += "UNKNOWN\n";
		break;
	}

	int nextLevel = indentationAmt + 3;

	GameDebuggingCommons::AddIndentation(str, indentationAmt);
	str += "Number of Sound Effects: " + std::to_string(m_soundEffectNames.size()) + "\n";

	if (m_soundEffectNames.size() != 0)
	{
		GameDebuggingCommons::AddIndentation(str, indentationAmt);
		str += "Sound Effect Names: \n";
		for (size_t i = 0; i < m_soundEffectNames.size(); i++)
		{
			m_soundEffectNames[i].WriteSoundEffectDataToString(str, nextLevel);
		}
	}
}