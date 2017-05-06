#include "Game/GameEntities/Agents/NPCs/NPCBehaviorRegistrationHelper.hpp"
#include "Engine/Core/StringUtils.hpp"

//-----------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------
//Static Variables
STATIC std::map<std::string, NPCBehaviorRegistrationHelper>* 
NPCBehaviorRegistrationHelper::s_behaviorRegistrations = nullptr;

//-----------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------
//Constructors
NPCBehaviorRegistrationHelper::NPCBehaviorRegistrationHelper(const std::string& nam, BehaviorCreationFunc* bevCreatFunc,
	BehaviorDefaultCreationFunc* defCreatFunc)
	: m_name(nam),
	m_behaviorCreationFunc(bevCreatFunc),
	m_defaultBehaviorCreationFunc(defCreatFunc)
{
	SetName(m_name, false);

	if (s_behaviorRegistrations == nullptr)
	{
		s_behaviorRegistrations = new std::map<std::string, NPCBehaviorRegistrationHelper>();
	}

	std::map<std::string, NPCBehaviorRegistrationHelper>::const_iterator it
		= s_behaviorRegistrations->find(m_lowerCaseName);
	if (it != s_behaviorRegistrations->end())
	{
		//SPIT OUT ERROR!!!
		return;
	}

	s_behaviorRegistrations->insert(std::pair<std::string, NPCBehaviorRegistrationHelper>(m_lowerCaseName, *this));
}

NPCBehaviorRegistrationHelper::~NPCBehaviorRegistrationHelper()
{

}

//-----------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------
//Operations
NPCBehavior* NPCBehaviorRegistrationHelper::CreateBehavior(const XMLNode& node)
{
	if (m_behaviorCreationFunc == nullptr)
	{
		return nullptr;
	}
	return m_behaviorCreationFunc(node);
}

NPCBehavior* NPCBehaviorRegistrationHelper::CreateDefaultBehavior()
{
	if (m_defaultBehaviorCreationFunc == nullptr)
	{
		return nullptr;
	}
	return m_defaultBehaviorCreationFunc();
}

//-----------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------
//Setters
void NPCBehaviorRegistrationHelper::SetName(const std::string& str, bool handleChangingRegistration)
{
	std::string originalListing = m_lowerCaseName;

	m_name = str;
	m_lowerCaseName = MakeLower(m_name);

	if (handleChangingRegistration)
	{
		//register registration under new name
		if (s_behaviorRegistrations == nullptr)
		{
			s_behaviorRegistrations = new std::map<std::string, NPCBehaviorRegistrationHelper>();
		}
	}


	if (handleChangingRegistration)
	{
		//Unregister present existing registration
		if (s_behaviorRegistrations == nullptr)
		{
			return;
		}

	}

}

void NPCBehaviorRegistrationHelper::SetCreationFunc(BehaviorCreationFunc* func)
{
	m_behaviorCreationFunc = func;
}

void NPCBehaviorRegistrationHelper::SetDefaultCreationFunc(BehaviorDefaultCreationFunc* func)
{
	m_defaultBehaviorCreationFunc = func;
}

//-----------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------
//Getters
const std::string& NPCBehaviorRegistrationHelper::GetName() const
{
	return m_name;
}

const std::string& NPCBehaviorRegistrationHelper::GetLowerCaseName() const
{
	return m_lowerCaseName;
}

BehaviorCreationFunc* NPCBehaviorRegistrationHelper::GetCreationFunc() const
{
	return m_behaviorCreationFunc;
}

BehaviorDefaultCreationFunc* NPCBehaviorRegistrationHelper::GetDefaultCreationFunc() const
{
	return m_defaultBehaviorCreationFunc;
}

//-----------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------
//Static functions
STATIC NPCBehavior* NPCBehaviorRegistrationHelper::CreateDefaultBehaviorByName(const std::string& name)
{
	if (s_behaviorRegistrations == nullptr)
	{
		return nullptr;
	}

	std::string lower = MakeLower(name);
	std::map<std::string, NPCBehaviorRegistrationHelper>::iterator it = s_behaviorRegistrations->find(lower);

	if (it == s_behaviorRegistrations->end())
	{
		return nullptr;
	}
	return it->second.CreateDefaultBehavior();
}

STATIC NPCBehavior* NPCBehaviorRegistrationHelper::CreateBehaviorByName(const std::string& name, const XMLNode& node)
{
	if (s_behaviorRegistrations == nullptr)
	{
		return nullptr;
	}

	std::string lower = MakeLower(name);
	std::map<std::string, NPCBehaviorRegistrationHelper>::iterator it = s_behaviorRegistrations->find(lower);

	if (it == s_behaviorRegistrations->end())
	{
		return nullptr;
	}
	return it->second.CreateBehavior(node);
}

STATIC bool NPCBehaviorRegistrationHelper::DetermineBehaviorExists(const std::string& name)
{
	if (s_behaviorRegistrations == nullptr)
	{
		return false;
	}

	std::string lower = MakeLower(name);
	std::map<std::string, NPCBehaviorRegistrationHelper>::iterator it = s_behaviorRegistrations->find(lower);
	if (it == s_behaviorRegistrations->end())
	{
		return false;
	}

	return true;
}

STATIC void NPCBehaviorRegistrationHelper::ClearAllBehaviorRegistrations()
{
	if (s_behaviorRegistrations == nullptr)
	{
		return;
	}

	delete s_behaviorRegistrations;
	s_behaviorRegistrations = nullptr;
}