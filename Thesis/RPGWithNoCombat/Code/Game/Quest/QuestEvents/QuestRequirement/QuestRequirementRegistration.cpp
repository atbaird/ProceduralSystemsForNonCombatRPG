#include "Game/Quest/QuestEvents/QuestRequirement/QuestRequirementRegistration.hpp"
#include "Engine/Core/StringUtils.hpp"

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//Static Variables
STATIC std::map<std::string, QuestRequirementRegistration>* 
QuestRequirementRegistration::s_QuestRequirementRegistrations = nullptr;

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//Constructors
QuestRequirementRegistration::QuestRequirementRegistration(const std::string& nam,
	QuestRequirementCreationFunc* bevCreatFunc,
	QuestRequirementDefaultCreationFunc* defCreatFunc)
	: m_creationFunc(bevCreatFunc),
	m_defaultCreationFunc(defCreatFunc)
{
	SetName(nam);

	if (s_QuestRequirementRegistrations == nullptr)
	{
		s_QuestRequirementRegistrations = new std::map<std::string, QuestRequirementRegistration>();
	}

	s_QuestRequirementRegistrations->insert(
		std::pair<std::string, 
		QuestRequirementRegistration>(
			m_LowerCaseQuestRequirementRegistrationName, 
			*this));
}

QuestRequirementRegistration::~QuestRequirementRegistration()
{
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//Setters
void QuestRequirementRegistration::SetName(const std::string& str)
{
	m_QuestRequirementRegistrationName = str;
	m_LowerCaseQuestRequirementRegistrationName = MakeLower(str);
}

void QuestRequirementRegistration::SetCreationFunc(QuestRequirementCreationFunc* func)
{
	m_creationFunc = func;
}

void QuestRequirementRegistration::SetDefaultCreationFunc(QuestRequirementDefaultCreationFunc* func)
{
	m_defaultCreationFunc = func;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//Getters
const std::string& QuestRequirementRegistration::GetName() const
{
	return m_QuestRequirementRegistrationName;
}

const std::string& QuestRequirementRegistration::GetLowerCaseName() const
{
	return m_LowerCaseQuestRequirementRegistrationName;
}

QuestRequirementCreationFunc* QuestRequirementRegistration::GetCreationFunc() const
{
	return m_creationFunc;
}

QuestRequirementDefaultCreationFunc* QuestRequirementRegistration::GetDefaultCreationFunc() const
{
	return m_defaultCreationFunc;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//Operations
QuestRequirement* QuestRequirementRegistration::CreateQuestRequirement(const XMLNode& node)
{
	if (m_creationFunc == nullptr)
	{
		return nullptr;
	}
	return m_creationFunc(node);
}

QuestRequirement* QuestRequirementRegistration::CreateDefaultQuestRequirement()
{
	if (m_defaultCreationFunc == nullptr)
	{
		return nullptr;
	}
	return m_defaultCreationFunc();
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//Static Functions
STATIC const std::string 
QuestRequirementRegistration::GetRegistrationNameForQuestRequirementTypes(QuestRequirementType reqType,
	QuestRequirementCheckVariableType varType)
{
	std::string typeToCall = "";
	if (reqType == Check_Player_Stats)
	{
		typeToCall = "CheckPlayerStats";
	}
	else if (reqType == Player_Interacting_With)
	{
		typeToCall = "CheckPlayerInteraction";
	}
	else if (reqType == Check_Variable)
	{
		if (varType == Float_Type_Check_Variable)
		{
			typeToCall = "CheckVariableFloat";
		}
		else if (varType == Bool_Type_Check_Variable)
		{
			typeToCall = "CheckVariableBool";
		}
		else if (varType == Entity_Has_Type_Check_Variable)
		{
			typeToCall = "CheckEntityHas";
		}
		else if (varType == Global_Float_Type_Check_Variable)
		{
			typeToCall = "GlobalCheckVariableFloat";
		}
		else if (varType == Global_Bool_Type_Check_Variable)
		{
			typeToCall = "GlobalCheckVariableBool";
		}
	}
	else if (reqType == Dialogue_Choice_Was)
	{
		typeToCall = "CheckPlayerChoice";
	}
	return typeToCall;
}

STATIC QuestRequirement* QuestRequirementRegistration::CreateDefaultQuestRequirementByName(
	const std::string& name)
{
	if (s_QuestRequirementRegistrations == nullptr)
	{
		return nullptr;
	}
	std::string lowerCase = MakeLower(name);
	std::map<std::string, QuestRequirementRegistration>::iterator it 
		= s_QuestRequirementRegistrations->find(lowerCase);
	if (it == s_QuestRequirementRegistrations->end())
	{
		return nullptr;
	}
	return it->second.CreateDefaultQuestRequirement();
}

QuestRequirement* QuestRequirementRegistration::CreateDefaultQuestRequirementByType(QuestRequirementType reqType,
	QuestRequirementCheckVariableType varType)
{
	std::string typeToCall = GetRegistrationNameForQuestRequirementTypes(reqType, varType);
	if (typeToCall == "")
	{
		return nullptr;
	}
	return CreateDefaultQuestRequirementByName(typeToCall);
}

STATIC QuestRequirement* QuestRequirementRegistration::CreateQuestRequirementByName(
	const std::string& name, const XMLNode& node)
{
	if (s_QuestRequirementRegistrations == nullptr)
	{
		return nullptr;
	}
	std::string lowerCase = MakeLower(name);
	std::map<std::string, QuestRequirementRegistration>::iterator it
		= s_QuestRequirementRegistrations->find(lowerCase);
	if (it == s_QuestRequirementRegistrations->end())
	{
		return nullptr;
	}
	return it->second.CreateQuestRequirement(node);
}

STATIC QuestRequirement* 
QuestRequirementRegistration::CreateQuestRequirementByType(const XMLNode& node, QuestRequirementType reqType,
	QuestRequirementCheckVariableType varType)
{
	std::string typeToCall = GetRegistrationNameForQuestRequirementTypes(reqType, varType);
	if (typeToCall == "")
	{
		return nullptr;
	}
	return CreateQuestRequirementByName(typeToCall, node);
}

STATIC bool QuestRequirementRegistration::DetermineQuestRequirementRegistrationExists(const std::string& name)
{
	if (s_QuestRequirementRegistrations == nullptr)
	{
		return false;
	}
	std::string lowerCase = MakeLower(name);
	std::map<std::string, QuestRequirementRegistration>::iterator it
		= s_QuestRequirementRegistrations->find(lowerCase);
	if (it == s_QuestRequirementRegistrations->end())
	{
		return false;
	}
	return true;
}

STATIC void QuestRequirementRegistration::ClearAllQuestRequirementRegistrations()
{
	if (s_QuestRequirementRegistrations != nullptr)
	{
		delete s_QuestRequirementRegistrations;
		s_QuestRequirementRegistrations = nullptr;
	}
}