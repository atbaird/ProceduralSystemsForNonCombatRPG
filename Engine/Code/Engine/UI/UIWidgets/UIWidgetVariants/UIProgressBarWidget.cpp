#include "Engine/UI/UIWidgets/UIWidgetVariants/UIProgressBarWidget.hpp"
#include "Engine/UI/UISystem.hpp"
#include "Engine/Core/StringUtils.hpp"

const size_t UIProgressBarWidget::s_NumOfEventsProgressBarNeeds = 1;

//------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------
//Constructors
UIProgressBarWidget::UIProgressBarWidget(size_t rootNodeID, UIStyleSheet* styleSheet, UIWidgetBase* parent,
	const std::string& widgetName, size_t additionalEventsToPrealloc)
	: m_fillImage(UISystem::GetUISystemPtr()->GetDefaultBackgroundImageForWidgets()),
	m_fillMat(new Material()),
	UIWidgetBase(rootNodeID, styleSheet, parent, widgetName, false, additionalEventsToPrealloc + s_NumOfEventsProgressBarNeeds)
{
}

UIProgressBarWidget::UIProgressBarWidget(const XMLNode& node, size_t rootNodeID, UIStyleSheet* styleSheet,
	UIWidgetBase* parent, const std::string& widgetName, size_t additionalEventsToPrealloc)
	: m_fillImage(UISystem::GetUISystemPtr()->GetDefaultBackgroundImageForWidgets()),
	m_fillMat(new Material()),
	UIWidgetBase(node, rootNodeID, styleSheet, parent, widgetName, false, additionalEventsToPrealloc + s_NumOfEventsProgressBarNeeds)
{
	ReadAndHandleXMLNodeAttributes(node);
}

UIProgressBarWidget::~UIProgressBarWidget()
{
	if (m_fillMat != nullptr)
	{
		delete m_fillMat;
		m_fillMat = nullptr;
	}
	UIWidgetBase::~UIWidgetBase();
}


std::string UIProgressBarWidget::ReadAndHandleXMLAttribute(const XMLAttribute& attr, bool& readOnceAlready)
{
	std::string lowerCaseAttrName = UIWidgetBase::ReadAndHandleXMLAttribute(attr, readOnceAlready);
	if (readOnceAlready)
	{
		return lowerCaseAttrName;
	}

	if (lowerCaseAttrName == "percantage" || lowerCaseAttrName == "fill")
	{
		readOnceAlready = true;
		float percent = (float)atof(attr.lpszValue);
		SetFillPercentage(percent);
	}
	else if (lowerCaseAttrName == "filldirection")
	{
		readOnceAlready = true;
		std::string lowerCaseVar = MakeLower(attr.lpszValue);
		if (lowerCaseVar == "left")
		{
			m_fillDirection = FillDirection_Left;
		}
		else if (lowerCaseVar == "right")
		{
			m_fillDirection = FillDirection_Right;
		}
		else if (lowerCaseVar == "up")
		{
			m_fillDirection = FillDirection_Up;
		}
		else if (lowerCaseVar == "down")
		{
			m_fillDirection = FillDirection_Down;
		}
	}

	return lowerCaseAttrName;
}

//------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------
//Renders and Updates
void UIProgressBarWidget::Update(float deltaSeconds)
{
	if (m_previousPercentage != m_percentage)
	{
		m_vectorPack.m_eventCallbacks[4].CallEnterEvent(this);
		m_vectorPack.m_eventCallbacks[4].CallUpdateEvent(this);
		m_vectorPack.m_eventCallbacks[4].CallExitEvent(this);
	}
	UIWidgetBase::Update(deltaSeconds);
	UpdateFillSize(deltaSeconds);
	UpdateFillImage(deltaSeconds);
	UpdateFillMesh(deltaSeconds);

	m_previousPercentage = m_percentage;
}

void UIProgressBarWidget::UpdateFillSize(float deltaSeconds)
{
	deltaSeconds;
	if (m_percentage == m_previousPercentage)
	{
		return;
	}


	m_fillIndices = m_vectorPack.m_BackgroundIndices;
	m_fillVertices = m_vectorPack.m_BackgroundModel;
	Vector3& val0 = m_fillVertices[0].m_pos;
	Vector3& val1 = m_fillVertices[1].m_pos;
	Vector3& val2 = m_fillVertices[2].m_pos;
	Vector3& val3 = m_fillVertices[3].m_pos;
	Vector3 upDir = m_fillVertices[2].m_pos - m_fillVertices[0].m_pos;
	Vector3 rightDir = m_fillVertices[1].m_pos - m_fillVertices[0].m_pos;
	float upDist = upDir.normalize();
	float rightDist = rightDir.normalize();

	switch (m_fillDirection)
	{
	case FillDirection_Down:
		val0 = val2 - (upDir * upDist * m_percentage);
		val1 = val3 - (upDir * upDist * m_percentage);
		break;
	case  FillDirection_Left:
		val0 = val1 - (rightDir * rightDist * m_percentage);
		val2 = val3 - (rightDir * rightDist * m_percentage);
		break;
	case FillDirection_Right:
		val1 = val0 + (rightDir * rightDist * m_percentage);
		val3 = val2 + (rightDir * rightDist * m_percentage);
		break;
	case FillDirection_Up:
		val2 = val0 + (upDir * upDist * m_percentage);
		val3 = val2 + (upDir * upDist * m_percentage);
		break;
	default:
		break;
	}
}

void UIProgressBarWidget::UpdateFillImage(float deltaSeconds)
{
	deltaSeconds;
	if (m_percentage == m_previousPercentage)
	{
		return;
	}
	if (m_currentWidgetState == UIWIDGETSTATE_HIDDEN)
	{
		return;
	}

	if (m_styleSheet != nullptr)
	{
		const UIStyleWidgetData* data = m_styleSheet->GetStyleDataForWidgetType(m_widgetName);
		if (data == nullptr)
		{
			data = m_styleSheet->GetStyleDataForWidgetType("default");
		}
		UIStateAppearanceData appData = data->GetAppearanceDataForIdx(m_currentWidgetState);
		m_fillImage = appData.GetFillBarImage();
		m_currentFillImage = m_fillImage;
		m_fillColor = appData.GetFillBarColor();
	}
	else if (m_manuallyOverrideFillImage)
	{
		m_currentFillImage = m_fillImageManualOverrided;
		if (m_fillImageManualOverrided == nullptr)
		{
			m_manuallyOverrideFillImage = false;
		}
	}
	else
	{
		m_currentFillImage = nullptr;
	}


}

void UIProgressBarWidget::UpdateFillMesh(float deltaSeconds)
{
	deltaSeconds;
	if (m_percentage == m_previousPercentage)
	{
		return;
	}

	for (size_t vertIdx = 0; vertIdx < m_fillVertices.size(); vertIdx++)
	{
		Rgba& vertColor = m_fillVertices[vertIdx].m_color;
		vertColor = m_fillColor;
	}
	m_fillMesh.SetDrawMode(PRIMITIVE_TRIANGLES);
	m_fillMesh.setVBO(m_fillVertices);
	m_fillMesh.setIBO(m_fillIndices);

	m_fillMat->SetOrAddTexture(m_currentFillImage, 0, "gDiffuseTex", 0);
	m_fillMat->SetProgram(m_prog);
}

void UIProgressBarWidget::Render() const
{
	if (m_boolPack.m_Render
		&& m_currentWidgetState != UIWIDGETSTATE_HIDDEN
		&& m_boolPack.m_amANamedWidget == false)
	{
		RenderText();
		RenderFillImage();
		RenderBackground();
		RenderMargin();
	}
	RenderChildren();
}

void UIProgressBarWidget::RenderFillImage() const
{
	if (m_fillMat != nullptr)
	{
		m_fillMat->SetModelMatrix(m_ModelMatrix);
		m_fillMesh.DrawWith(m_fillMat);
	}
}

//-------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------
//Setters
void UIProgressBarWidget::SetFillColor(const Rgba& color)
{
	m_fillColor = color;
}

void UIProgressBarWidget::SetFillPercentage(float percentage)
{
	m_percentage = percentage;
	if (m_percentage > 1.f)
	{
		m_percentage = 1.f;
	}
	else if (m_percentage < 0.f)
	{
		m_percentage = 0.f;
	}
}

//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------
//Getters
const Rgba UIProgressBarWidget::GetFillColor() const
{
	return m_fillColor;
}

const float UIProgressBarWidget::GetPercentage() const
{
	return m_percentage;
}

const float UIProgressBarWidget::GetValueInRange(float min, float max) const
{
	float relativeAmount = max - min;
	float percentageEffected = m_percentage * relativeAmount;
	return percentageEffected + min;
}

const Texture* UIProgressBarWidget::GetPreferredFillImage() const
{
	if (m_manuallyOverrideFillImage && m_fillImageManualOverrided != nullptr)
	{
		return m_fillImageManualOverrided;
	}

	return m_fillImage;
}



//Static functions
STATIC UIWidgetBase* UIProgressBarWidget::SpawnUIProgressBarWidgetFunc(const XMLNode& data, const UIWidgetRegistrationDataPacket& packet)
{
	UIProgressBarWidget* progress = new UIProgressBarWidget(data, packet.GetRootNodeID(), packet.GetStyleSheet(), packet.GetUIWidgetParent());
	return progress;
}

STATIC UIWidgetBase* UIProgressBarWidget::SpawnUIProgressBarWithoutDataFunc(const UIWidgetRegistrationDataPacket& packet)
{
	UIProgressBarWidget* progress = new UIProgressBarWidget(packet.GetRootNodeID(), packet.GetStyleSheet(), packet.GetUIWidgetParent());
	return progress;
}