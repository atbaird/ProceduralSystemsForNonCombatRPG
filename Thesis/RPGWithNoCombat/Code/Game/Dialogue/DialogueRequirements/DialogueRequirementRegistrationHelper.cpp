#include "Game/Dialogue/DialogueRequirements/DialogueRequirementRegistrationHelper.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------
//Static Variables
STATIC std::map<std::string, DialogueRequirementRegistrationHelper>* 
	DialogueRequirementRegistrationHelper::s_DialogueRequirementRegistrations = nullptr;

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------
//Constructors
DialogueRequirementRegistrationHelper::DialogueRequirementRegistrationHelper(
	const std::string& nam, DialogueRequirementCreationFunc* bevCreatFunc,
	DialogueRequirementDefaultCreationFunc* defCreatFunc)
	: m_creationFunc(bevCreatFunc),
	m_defaultCreationFunc(defCreatFunc)
{
	bool nameIsNotSet = nam == "";
	GUARANTEE_RECOVERABLE(!nameIsNotSet, "Will not register a dialogue requirement with no name!");
	if (nameIsNotSet)
	{
		return;
	}
	SetName(nam);

	if (s_DialogueRequirementRegistrations == nullptr)
	{
		s_DialogueRequirementRegistrations = new std::map<std::string, DialogueRequirementRegistrationHelper>();
	}

	std::map<std::string, DialogueRequirementRegistrationHelper>::const_iterator it
		= s_DialogueRequirementRegistrations->find(m_lowerCaseRegistrationName);
	bool iteratorHitEnd = it == s_DialogueRequirementRegistrations->end();
	GUARANTEE_RECOVERABLE(iteratorHitEnd, "Will not register a Dialogue Requirement Reg Helper that was predefined!");
	if (!iteratorHitEnd)
	{
		return;
	}

	s_DialogueRequirementRegistrations->insert(
		std::pair<std::string, DialogueRequirementRegistrationHelper>(m_lowerCaseRegistrationName, *this));
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------
//Setters
void DialogueRequirementRegistrationHelper::SetName(const std::string& str)
{
	m_registrationName = str;
	m_lowerCaseRegistrationName = MakeLower(str);
}

void DialogueRequirementRegistrationHelper::SetCreationFunc(DialogueRequirementCreationFunc* func)
{
	m_creationFunc = func;
}

void DialogueRequirementRegistrationHelper::SetDefaultCreationFunc(DialogueRequirementDefaultCreationFunc* func)
{
	m_defaultCreationFunc = func;
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------
//Getters
const std::string& DialogueRequirementRegistrationHelper::GetName() const
{
	return m_registrationName;
}

const std::string& DialogueRequirementRegistrationHelper::GetLowerCaseName() const
{
	return m_lowerCaseRegistrationName;
}

DialogueRequirementCreationFunc* DialogueRequirementRegistrationHelper::GetCreationFunc() const
{
	return m_creationFunc;
}

DialogueRequirementDefaultCreationFunc* DialogueRequirementRegistrationHelper::GetDefaultCreationFunc() const
{
	return m_defaultCreationFunc;
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------
//Operations
DialogueRequirement* DialogueRequirementRegistrationHelper::CreateDialogueRequirement(const XMLNode& node)
{
	if (m_creationFunc == nullptr)
	{
		return nullptr;
	}
	return m_creationFunc(node);
}

DialogueRequirement* DialogueRequirementRegistrationHelper::CreateDefaultDialogueRequirement()
{
	if (m_defaultCreationFunc == nullptr)
	{
		return nullptr;
	}
	return m_defaultCreationFunc();
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------
//Static Functions
STATIC DialogueRequirement* 
	DialogueRequirementRegistrationHelper::CreateDefaultDialogueRequirementByName(const std::string& name)
{
	if (s_DialogueRequirementRegistrations == nullptr)
	{
		return nullptr;
	}

	std::string lowerCaseName = MakeLower(name);
	std::map<std::string, DialogueRequirementRegistrationHelper>::iterator it
		= s_DialogueRequirementRegistrations->find(lowerCaseName);
	if (it == s_DialogueRequirementRegistrations->end())
	{
		return nullptr;
	}
	return it->second.CreateDefaultDialogueRequirement();
}

STATIC DialogueRequirement* 
	DialogueRequirementRegistrationHelper::CreateDialogueRequirementByName(const std::string& name, const XMLNode& node)
{
	if (s_DialogueRequirementRegistrations == nullptr)
	{
		return nullptr;
	}

	std::string lowerCaseName = MakeLower(name);
	std::map<std::string, DialogueRequirementRegistrationHelper>::iterator it
		= s_DialogueRequirementRegistrations->find(lowerCaseName);
	if (it == s_DialogueRequirementRegistrations->end())
	{
		return nullptr;
	}
	return it->second.CreateDialogueRequirement(node);
}

STATIC bool DialogueRequirementRegistrationHelper::DetermineDialogueRequirementExists(const std::string& name)
{
	if (s_DialogueRequirementRegistrations == nullptr)
	{
		return false;
	}

	std::string lowerCaseName = MakeLower(name);
	std::map<std::string, DialogueRequirementRegistrationHelper>::iterator it 
		= s_DialogueRequirementRegistrations->find(lowerCaseName);
	if (it == s_DialogueRequirementRegistrations->end())
	{
		return false;
	}
	return true;
}

STATIC void DialogueRequirementRegistrationHelper::ClearAllDialogueRequirementRegistrations()
{
	if (s_DialogueRequirementRegistrations == nullptr)
	{
		return;
	}
	delete s_DialogueRequirementRegistrations;
	s_DialogueRequirementRegistrations = nullptr;
}