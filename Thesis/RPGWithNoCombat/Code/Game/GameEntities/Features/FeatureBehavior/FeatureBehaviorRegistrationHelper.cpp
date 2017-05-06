#include "Game/GameEntities/Features/FeatureBehavior/FeatureBehaviorRegistrationHelper.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Engine/Core/FileUtils.hpp"

//--------------------------------------------------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------
//Static Variables
STATIC std::map<std::string, FeatureBehaviorRegistrationHelper>*
FeatureBehaviorRegistrationHelper::s_behaviorRegistrations = nullptr;

//--------------------------------------------------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------
//Constructors
FeatureBehaviorRegistrationHelper::FeatureBehaviorRegistrationHelper()
{

}

FeatureBehaviorRegistrationHelper::FeatureBehaviorRegistrationHelper(const std::string& registrationName,
	FeatureBehaviorCreationFunc* creationFunc,
	FeatureBehaviorDefaultCreationFunc* defaultCreationFunc)
	: m_featureBehaviorCreationFunc(creationFunc),
	m_featureBehaviorDefaultCreationFunc(defaultCreationFunc)
{
	SetName(registrationName);
	if (s_behaviorRegistrations == nullptr)
	{
		s_behaviorRegistrations = new std::map<std::string, FeatureBehaviorRegistrationHelper>();
	}

	std::map<std::string, FeatureBehaviorRegistrationHelper>::const_iterator it
		= s_behaviorRegistrations->find(m_lowerCaseName);
	if (it != s_behaviorRegistrations->end())
	{
		//SPIT OUT ERROR!!!
		return;
	}

	s_behaviorRegistrations->insert(std::pair<std::string, FeatureBehaviorRegistrationHelper>(m_lowerCaseName, *this));
}

FeatureBehaviorRegistrationHelper::~FeatureBehaviorRegistrationHelper()
{
}

//--------------------------------------------------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------
//Setters
void FeatureBehaviorRegistrationHelper::SetName(const std::string& str,
	bool handleChangingRegistration)
{
	handleChangingRegistration;
	m_registrationName = str;
	m_lowerCaseName = MakeLower(m_registrationName);
}

void FeatureBehaviorRegistrationHelper::SetCreationFunc(FeatureBehaviorCreationFunc* func)
{
	m_featureBehaviorCreationFunc = func;
}

void FeatureBehaviorRegistrationHelper::SetDefaultCreationFunc(FeatureBehaviorDefaultCreationFunc* func)
{
	m_featureBehaviorDefaultCreationFunc = func;
}

//--------------------------------------------------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------
//Getters
const std::string& FeatureBehaviorRegistrationHelper::GetRegistrationName() const
{
	return m_registrationName;
}

const std::string& FeatureBehaviorRegistrationHelper::GetLowerCaseRegistrationName() const
{
	return m_lowerCaseName;
}

FeatureBehaviorCreationFunc* FeatureBehaviorRegistrationHelper::GetBehaviorCreationFunc() const
{
	return m_featureBehaviorCreationFunc;
}

FeatureBehaviorDefaultCreationFunc* FeatureBehaviorRegistrationHelper::GetDefaultBehaviorCreationFunc() const
{
	return m_featureBehaviorDefaultCreationFunc;
}

//--------------------------------------------------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------
//Operations
FeatureBehavior* FeatureBehaviorRegistrationHelper::CreateFeatureBehavior(const XMLNode& node)
{
	if (m_featureBehaviorCreationFunc == nullptr)
	{
		return nullptr;
	}
	return m_featureBehaviorCreationFunc(node);
}

FeatureBehavior* FeatureBehaviorRegistrationHelper::DefaultCreateFeatureBehavior()
{
	if (m_featureBehaviorDefaultCreationFunc == nullptr)
	{
		return nullptr;
	}
	return m_featureBehaviorDefaultCreationFunc();
}

//--------------------------------------------------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------
//Static functions
STATIC FeatureBehavior* FeatureBehaviorRegistrationHelper::CreateDefaultFeatureBehaviorByName(
	const std::string& name)
{
	if (s_behaviorRegistrations == nullptr)
	{
		return nullptr;
	}
	std::string lowerCaseName = MakeLower(name);
	std::map<std::string, FeatureBehaviorRegistrationHelper>::iterator it
		= s_behaviorRegistrations->find(lowerCaseName);
	if (it != s_behaviorRegistrations->end())
	{
		return it->second.DefaultCreateFeatureBehavior();
	}
	return nullptr;
}

STATIC FeatureBehavior* FeatureBehaviorRegistrationHelper::CreateFeatureBehaviorByName(
	const std::string& name, const XMLNode& node)
{
	if (s_behaviorRegistrations == nullptr)
	{
		return nullptr;
	}
	std::string lowerCaseName = MakeLower(name);
	std::map<std::string, FeatureBehaviorRegistrationHelper>::iterator it
		= s_behaviorRegistrations->find(lowerCaseName);
	if (it != s_behaviorRegistrations->end())
	{
		return it->second.CreateFeatureBehavior(node);
	}
	return nullptr;
}

STATIC bool FeatureBehaviorRegistrationHelper::DetermineFeatureBehaviorExists(const std::string& name)
{
	if (s_behaviorRegistrations == nullptr)
	{
		return false;
	}
	std::string lowerCaseName = MakeLower(name);
	std::map<std::string, FeatureBehaviorRegistrationHelper>::iterator it 
		= s_behaviorRegistrations->find(lowerCaseName);
	if (it != s_behaviorRegistrations->end())
	{
		return true;
	}
	return false;
}

STATIC void FeatureBehaviorRegistrationHelper::ClearAllFeatureBehaviorRegistrations()
{
	if (s_behaviorRegistrations == nullptr)
	{
		return;
	}
	delete s_behaviorRegistrations;
	s_behaviorRegistrations = nullptr;
}
