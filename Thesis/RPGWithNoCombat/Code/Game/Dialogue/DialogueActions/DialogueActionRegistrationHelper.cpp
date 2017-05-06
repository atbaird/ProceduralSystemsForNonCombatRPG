#include "Game/Dialogue/DialogueActions/DialogueActionRegistrationHelper.hpp"
#include "Game/Dialogue/DialogueActions/DialogueTrigger.hpp"
#include "Engine/Core/EngineXMLParser.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"

//--------------------------------------------------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------
//Static Variables
STATIC std::map<std::string, DialogueActionRegistrationHelper>*
	DialogueActionRegistrationHelper::s_DialogueActionRegistrations = nullptr;

//--------------------------------------------------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------
//Constructors
DialogueActionRegistrationHelper::DialogueActionRegistrationHelper(const std::string& registrationName,
	DialogueActionCreationFunc* creationFunc,
	DialogueActionDefaultCreationFunc* defaultCreationFunc)
	: m_creationFunc(creationFunc),
	m_defaultCreationFunc(defaultCreationFunc)
{
	SetDialogueActionRegistrationHelperName(registrationName);
	GUARANTEE_RECOVERABLE(registrationName != "", 
		"DialogueActionRegistrationHelpers can not be registered if they have no name!");
	if (registrationName == "")
	{
		return;
	}

	if (s_DialogueActionRegistrations == nullptr)
	{
		s_DialogueActionRegistrations = new std::map<std::string, DialogueActionRegistrationHelper>();
	}
	std::map<std::string, DialogueActionRegistrationHelper>::const_iterator it
		= s_DialogueActionRegistrations->find(m_lowerCaseRegistrationName);
	GUARANTEE_RECOVERABLE(it == s_DialogueActionRegistrations->end(), 
		"Can not register duplicate DialogueActionRegistrationHelpers!");
	if (it != s_DialogueActionRegistrations->end())
	{
		return;
	}
	s_DialogueActionRegistrations->insert(
		std::pair<std::string, DialogueActionRegistrationHelper>(m_lowerCaseRegistrationName, *this));

}

DialogueActionRegistrationHelper::~DialogueActionRegistrationHelper()
{

}

//--------------------------------------------------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------
//Setters
void DialogueActionRegistrationHelper::SetDialogueActionRegistrationHelperName(const std::string& str)
{
	m_registrationName = str;
	m_lowerCaseRegistrationName = MakeLower(str);
}

void DialogueActionRegistrationHelper::SetCreationFunc(DialogueActionCreationFunc* func)
{
	m_creationFunc = func;
}

void DialogueActionRegistrationHelper::SetDefaultCreationFunc(DialogueActionDefaultCreationFunc* func)
{
	m_defaultCreationFunc = func;
}

//--------------------------------------------------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------
//Getters
const std::string& DialogueActionRegistrationHelper::GetName() const
{
	return m_registrationName;
}

const std::string& DialogueActionRegistrationHelper::GetLowerCaseName() const
{
	return m_lowerCaseRegistrationName;
}

DialogueActionCreationFunc* DialogueActionRegistrationHelper::GetCreationFunc() const
{
	return m_creationFunc;
}

DialogueActionDefaultCreationFunc* DialogueActionRegistrationHelper::GetDefaultCreationFunc() const
{
	return m_defaultCreationFunc;
}

//--------------------------------------------------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------
//Operations
DialogueTrigger* DialogueActionRegistrationHelper::CreateDialogueAction(const XMLNode& node) const
{
	return m_creationFunc(node);
}

DialogueTrigger* DialogueActionRegistrationHelper::CreateDefaultDialogueAction() const
{
	return m_defaultCreationFunc();
}

//--------------------------------------------------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------
//Static Functions
STATIC DialogueTrigger* DialogueActionRegistrationHelper::CreateDefaultDialogueActionByName(const std::string& name)
{
	if (s_DialogueActionRegistrations == nullptr)
	{
		return nullptr;
	}
	std::string lowerCase = MakeLower(name);

	std::map<std::string, DialogueActionRegistrationHelper>::const_iterator it
		= s_DialogueActionRegistrations->find(lowerCase);
	if (it == s_DialogueActionRegistrations->end())
	{
		return nullptr;
	}
	return it->second.CreateDefaultDialogueAction();
}

STATIC DialogueTrigger* DialogueActionRegistrationHelper::CreateDialogueActionByName(const std::string& name,
	const XMLNode& node)
{
	if (s_DialogueActionRegistrations == nullptr)
	{
		return nullptr;
	}
	std::string lowerCase = MakeLower(name);

	std::map<std::string, DialogueActionRegistrationHelper>::const_iterator it
		= s_DialogueActionRegistrations->find(lowerCase);
	if (it == s_DialogueActionRegistrations->end())
	{
		return nullptr;
	}
	return it->second.CreateDialogueAction(node);
}

STATIC bool DialogueActionRegistrationHelper::DetermineDialogueActionExists(const std::string& name)
{
	if (s_DialogueActionRegistrations == nullptr)
	{
		return false;
	}
	std::string lowerCase = MakeLower(name);

	std::map<std::string, DialogueActionRegistrationHelper>::const_iterator it
		= s_DialogueActionRegistrations->find(lowerCase);
	if (it == s_DialogueActionRegistrations->end())
	{
		return false;
	}
	return true;
}

STATIC void DialogueActionRegistrationHelper::ClearAllDialogueActionRegistrations()
{
	if (s_DialogueActionRegistrations == nullptr)
	{
		return;
	}
	delete s_DialogueActionRegistrations;
	s_DialogueActionRegistrations = nullptr;
}