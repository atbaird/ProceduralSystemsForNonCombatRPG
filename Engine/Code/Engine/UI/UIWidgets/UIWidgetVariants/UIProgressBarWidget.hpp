#pragma once

#ifndef UIPROGRESSBARWIDGET_HPP
#define UIPROGRESSBARWIDGET_HPP
#include "Engine/UI/UIWidgets/UIWidgetBase.hpp"
#include "Engine/UI/UIStructs/UIFillDirection.hpp"
#include "Engine/UI/UIStructs/UIWidgetRegistrationDataPacket.hpp"

class UIProgressBarWidget : public UIWidgetBase
{
private:
protected:
	static const size_t s_NumOfEventsProgressBarNeeds;

	std::vector<Vertex_PCT> m_fillVertices;
	std::vector<unsigned int> m_fillIndices;
	Mesh m_fillMesh;
	Rgba m_fillColor = Rgba(1.f,0.f,0.f,1.f);
	Material* m_fillMat = nullptr;
	const Texture* m_fillImage = nullptr;
	const Texture* m_currentFillImage = nullptr;
	const Texture* m_fillImageManualOverrided = nullptr;
	UIFillDirection m_fillDirection = FillDirection_Right;
	float m_percentage = 0.5f;
	float m_previousPercentage = -1.f;
	bool m_manuallyOverrideFillImage = false;
public:
	//Constructors
	UIProgressBarWidget(size_t rootNodeID = 0, UIStyleSheet* styleSheet = nullptr, UIWidgetBase* parent = nullptr,
		const std::string& widgetName = "ProgressBar", size_t additionalEventsToPrealloc = 0);
	UIProgressBarWidget(const XMLNode& node, size_t rootNodeID = 0, UIStyleSheet* styleSheet = nullptr, 
		UIWidgetBase* parent = nullptr, const std::string& widgetName = "ProgressBar", size_t additionalEventsToPrealloc = 0);
	virtual ~UIProgressBarWidget();

	virtual std::string ReadAndHandleXMLAttribute(const XMLAttribute& attr, bool& readOnceAlready) override; //returns the lowercase string

	//Renders and Updates
	virtual void Update(float deltaSeconds) override;
	virtual void UpdateFillSize(float deltaSeconds);
	virtual void UpdateFillImage(float deltaSeconds);
	virtual void UpdateFillMesh(float deltaSeconds);
	virtual void Render() const override;
	virtual void RenderFillImage() const;

	//Setters
	void SetFillColor(const Rgba& color);
	void SetFillPercentage(float percentage);

	//Getters
	const Rgba GetFillColor() const;
	const float GetPercentage() const;
	const float GetValueInRange(float min, float max) const;
	const Texture* GetPreferredFillImage() const;


	//Static functions
	static UIWidgetBase* SpawnUIProgressBarWidgetFunc(const XMLNode& data, const UIWidgetRegistrationDataPacket& packet);
	static UIWidgetBase* SpawnUIProgressBarWithoutDataFunc(const UIWidgetRegistrationDataPacket& packet);
};

#endif