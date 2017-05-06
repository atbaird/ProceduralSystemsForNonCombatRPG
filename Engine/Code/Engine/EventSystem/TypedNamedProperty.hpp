#pragma once

#ifndef TYPEDNAMEDPROPERTY_HPP
#define TYPEDNAMEDPROPERTY_HPP
#include "Engine/EventSystem/NamedPropertyBase.hpp"

template<typename T>
struct TypedNamedProperty : public NamedPropertyBase
{
public:
	T m_data;
public:
	//Constructors
	TypedNamedProperty(const T& data);
	TypedNamedProperty(const TypedNamedProperty& other);
	virtual ~TypedNamedProperty();

	virtual NamedPropertyBase* CloneProperty() const override;

};

//----------------------------------------------------------------------------------------------------------------------------------------------
template<typename T>
TypedNamedProperty<T>::TypedNamedProperty(const T& data)
	: NamedPropertyBase(),
	m_data(data)
{

}

template<typename T>
TypedNamedProperty<T>::TypedNamedProperty(const TypedNamedProperty<T>& other)
	: NamedPropertyBase(other),
	m_data(other.m_data)
{

}

//----------------------------------------------------------------------------------------------------------------------------------------------
template<typename T>
TypedNamedProperty<T>::~TypedNamedProperty() 
{
	NamedPropertyBase::~NamedPropertyBase(); 
};

//----------------------------------------------------------------------------------------------------------------------------------------------
template<typename T>
NamedPropertyBase* TypedNamedProperty<T>::CloneProperty() const
{
	TypedNamedProperty<T>* clone = new TypedNamedProperty<T>(m_data);
	return clone;
}

#endif