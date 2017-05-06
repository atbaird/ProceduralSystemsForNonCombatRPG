#include "Engine/UI/UIWidgets/UIWidgetRegistration/UIWidgetRegistration.hpp"
#include "Engine/UI/UIWidgets/UIWidgetBase.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"

//-----------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------
STATIC std::vector<UIWidgetRegistration> UIWidgetRegistration::s_Registrations;

//-----------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------
//Constructors
UIWidgetRegistration::UIWidgetRegistration()
{
	//This constructor is just so the vector is happy.
}

UIWidgetRegistration::UIWidgetRegistration(const std::string& name, SpawnWidgetFunc* func, SpawnWidgetWithoutDataFunc* func2)
	: m_WidgetName(name),
	m_SpawnFunction(func),
	m_SpawnWithoutDataFunction(func2)
{
	if (name == "")
	{
		ERROR_AND_DIE("Can NOT insert a UI Widget Registration that has no name!");
		return;
	}
	if (m_SpawnFunction == nullptr
		|| m_SpawnWithoutDataFunction == nullptr)
	{
		ERROR_AND_DIE("Can NOT insert a UI Widget Registration that has no spawn function!");
	}

	if (GetIsWidgetTypeAlreadyDefined(name))
	{
		ERROR_AND_DIE("Can NOT insert a UI Widget Registration if it's name is already in use!");
	}
}

UIWidgetRegistration::~UIWidgetRegistration()
{
}

//-----------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------
//Operators
const bool UIWidgetRegistration::operator==(const UIWidgetRegistration& other)
{
	if (m_WidgetName == other.m_WidgetName)
	{
		return true;
	}
	return false;
}

const bool UIWidgetRegistration::operator==(const std::string& name)
{
	if (m_WidgetName == name)
	{
		return true;
	}
	return false;
}

//-----------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------
//Static Functions
STATIC UIWidgetBase* UIWidgetRegistration::SpawnWidgetOfType(const std::string& widgetName, const UIWidgetRegistrationDataPacket& packet)
{
	for (size_t regIdx = 0; regIdx < s_Registrations.size(); regIdx++)
	{
		UIWidgetRegistration* registration = &s_Registrations[regIdx];
		if (registration->m_WidgetName == widgetName)
		{
			return registration->m_SpawnWithoutDataFunction(packet);
		}
	}
	return nullptr;
}

STATIC UIWidgetBase* UIWidgetRegistration::SpawnWidgetOfType(const std::string& widgetName, const UIWidgetRegistrationDataType& data, const UIWidgetRegistrationDataPacket& packet)
{
	for (size_t regIdx = 0; regIdx < s_Registrations.size(); regIdx++)
	{
		UIWidgetRegistration* registration = &s_Registrations[regIdx];
		if (registration->m_WidgetName == widgetName)
		{
			return registration->m_SpawnFunction(data, packet);
		}
	}
	return nullptr;
}

STATIC bool UIWidgetRegistration::GetIsWidgetTypeAlreadyDefined(const std::string& widgetName)
{
	for (size_t regIdx = 0; regIdx < s_Registrations.size(); regIdx++)
	{
		UIWidgetRegistration* registration = &s_Registrations[regIdx];
		if (registration->m_WidgetName == widgetName)
		{
			return true;
		}
	}
	return false;
}

STATIC int UIWidgetRegistration::GetIdxOfWidgetTypeIfAlreadyDefined(const std::string& widgetName)
{
	for (size_t regIdx = 0; regIdx < s_Registrations.size(); regIdx++)
	{
		UIWidgetRegistration* registration = &s_Registrations[regIdx];
		if (registration->m_WidgetName == widgetName)
		{
			return regIdx;
		}
	}
	return -1;
}

#include "Engine/UI/UIWidgets/UIWidgetVariants/UIButtonWidget.hpp"
#include "Engine/UI/UIWidgets/UIWidgetVariants/UIGroupWidgetVariants/UIHorizontalBoxWidget.hpp"
#include "Engine/UI/UIWidgets/UIWidgetVariants/UIGroupWidgetVariants/UIVerticalBoxWidget.hpp"
#include "Engine/UI/UIWidgets/UIWidgetVariants/UIButtonWidgetVariants/UICheckboxWidget.hpp"
#include "Engine/UI/UIWidgets/UIWidgetVariants/UIButtonWidgetVariants/UIRadioWidget.hpp"
#include "Engine/UI/UIWidgets/UIWidgetVariants/UILabelWidget.hpp"
#include "Engine/UI/UIWidgets/UIWidgetVariants/UIProgressBarWidget.hpp"
STATIC void UIWidgetRegistration::RegisterAllWidgetTypes()
{
	s_Registrations.clear();
	UIWidgetRegistration UIButtonRegistration = UIWidgetRegistration("button",
		UIButtonWidget::SpawnUIButtonWidgetFunc, UIButtonWidget::SpawnUIButtonWithoutDataFunc);

	UIWidgetRegistration UIVerticalBoxRegistration = UIWidgetRegistration("verticalbox",
		UIVerticalBoxWidget::SpawnUIVerticalBoxWidgetFunc, UIVerticalBoxWidget::SpawnUIVerticalBoxWithoutDataFunc);

	UIWidgetRegistration UIHorizontalBoxRegistration = UIWidgetRegistration("horizontalbox",
		UIHorizontalBoxWidget::SpawnUIHorizontalBoxWidgetFunc, UIHorizontalBoxWidget::SpawnUIHorizontalBoxWithoutDataFunc);

	UIWidgetRegistration UICheckboxRegistration = UIWidgetRegistration("checkbox",
		UICheckboxWidget::SpawnUICheckboxWidgetFunc, UICheckboxWidget::SpawnUICheckboxWithoutDataFunc);

	UIWidgetRegistration UIRadioRegistration = UIWidgetRegistration("radio",
		UIRadioWidget::SpawnUIRadioWidgetFunc, UIRadioWidget::SpawnUIRadioWithoutDataFunc);

	UIWidgetRegistration UILabelRegistration = UIWidgetRegistration("label",
		UILabelWidget::SpawnUILabelWidgetFunc, UILabelWidget::SpawnUILabelWithoutDataFunc);

	UIWidgetRegistration UIProgressBarRegistration = UIWidgetRegistration("progressbar",
		UIProgressBarWidget::SpawnUIProgressBarWidgetFunc, UIProgressBarWidget::SpawnUIProgressBarWithoutDataFunc);
	s_Registrations.push_back(UIButtonRegistration);
	s_Registrations.push_back(UIVerticalBoxRegistration);
	s_Registrations.push_back(UIHorizontalBoxRegistration);
	s_Registrations.push_back(UICheckboxRegistration);
	s_Registrations.push_back(UIRadioRegistration);
	s_Registrations.push_back(UILabelRegistration);
	s_Registrations.push_back(UIProgressBarRegistration);

}

void UIWidgetRegistration::ClearAllWidgetTypes()
{
	s_Registrations.clear();
	s_Registrations.shrink_to_fit();
}