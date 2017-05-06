#include "Game/Quest/QuestEvents/QuestTrigger/QuestTriggerRegistration.hpp"
#include "Engine/Core/EngineXMLParser.hpp"
#include "Engine/Core/StringUtils.hpp"

//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//Static Variables
STATIC std::map<std::string, QuestTriggerRegistration>* 
	QuestTriggerRegistration::s_QuestTriggerRegistrations = nullptr;

//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//Constructors
QuestTriggerRegistration::QuestTriggerRegistration(const std::string& triggerRegName,
	QuestTriggerCreationFunc* func, QuestTriggerDefaultCreationFunc* defaultFunc)
	: m_func(func),
	m_defaultFunc(defaultFunc)
{
	SetTriggerRegName(triggerRegName);

	if (s_QuestTriggerRegistrations == nullptr)
	{
		s_QuestTriggerRegistrations = new STATIC std::map<std::string, QuestTriggerRegistration>();
	}

	std::map<std::string, QuestTriggerRegistration>::iterator 
		it = s_QuestTriggerRegistrations->find(m_LowerCaseTriggerRegName);
	if (it != s_QuestTriggerRegistrations->end())
	{
		//Spit out error
		return;
	}
	s_QuestTriggerRegistrations->insert(
		std::pair<std::string, QuestTriggerRegistration>(m_LowerCaseTriggerRegName, *this));
}

QuestTriggerRegistration::~QuestTriggerRegistration()
{
}

//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//Setters
void QuestTriggerRegistration::SetTriggerRegName(const std::string regName)
{
	m_TriggerRegName = regName;
	m_LowerCaseTriggerRegName = MakeLower(regName);
}

void QuestTriggerRegistration::SetQuestTriggerCreationFunc(QuestTriggerCreationFunc* func)
{
	m_func = func;
}

void QuestTriggerRegistration::SetQuestTriggerDefaultCreationFunc(QuestTriggerDefaultCreationFunc* func)
{
	m_defaultFunc = func;
}

//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//Getters
const std::string& QuestTriggerRegistration::GetTriggerRegName() const
{
	return m_TriggerRegName;
}

const std::string& QuestTriggerRegistration::GetLowerCaseTriggerRegName() const
{
	return m_LowerCaseTriggerRegName;
}

QuestTriggerCreationFunc* QuestTriggerRegistration::GetCreationFunc() const
{
	return m_func;
}

QuestTriggerDefaultCreationFunc* QuestTriggerRegistration::GetDefaultCreatonFunc() const
{
	return m_defaultFunc;
}

//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//Operations
QuestTrigger* QuestTriggerRegistration::CreateQuestTrigger(const XMLNode& node)
{
	if (m_func == nullptr)
	{
		return nullptr;
	}
	return m_func(node);
}

QuestTrigger* QuestTriggerRegistration::CreateDefaultQuestTrigger()
{
	if (m_defaultFunc == nullptr)
	{
		return nullptr;
	}
	return m_defaultFunc();
}

//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//Static Functions
STATIC QuestTrigger* QuestTriggerRegistration::CreateDefaultQuestTriggerByName(const std::string& name)
{
	if (s_QuestTriggerRegistrations == nullptr)
	{
		return nullptr;
	}
	std::string lowerCase = MakeLower(name);
	std::map<std::string, QuestTriggerRegistration>::iterator 
		it = s_QuestTriggerRegistrations->find(lowerCase);
	if (it != s_QuestTriggerRegistrations->end())
	{
		return it->second.CreateDefaultQuestTrigger();
	}
	return nullptr;
}

STATIC QuestTrigger* QuestTriggerRegistration::CreateQuestTriggerByName(const std::string& name, const XMLNode& node)
{
	if (s_QuestTriggerRegistrations == nullptr)
	{
		return nullptr;
	}
	std::string lowerCase = MakeLower(name);
	std::map<std::string, QuestTriggerRegistration>::iterator
		it = s_QuestTriggerRegistrations->find(lowerCase);
	if (it != s_QuestTriggerRegistrations->end())
	{
		return it->second.CreateQuestTrigger(node);
	}
	return nullptr;
}

STATIC bool QuestTriggerRegistration::DetermineBehaviorExists(const std::string& name)
{
	if (s_QuestTriggerRegistrations == nullptr)
	{
		return false;
	}
	std::string lowerCase = MakeLower(name);
	std::map<std::string, QuestTriggerRegistration>::iterator it = s_QuestTriggerRegistrations->find(lowerCase);
	if (it == s_QuestTriggerRegistrations->end())
	{
		return false;
	}
	return true;
}

STATIC void QuestTriggerRegistration::ClearAllBehaviorRegistrations()
{
	if (s_QuestTriggerRegistrations == nullptr)
	{
		delete s_QuestTriggerRegistrations;
		s_QuestTriggerRegistrations = nullptr;
	}
}