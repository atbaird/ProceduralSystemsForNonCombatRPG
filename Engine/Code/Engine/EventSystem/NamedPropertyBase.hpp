#pragma once

#ifndef NAMEDPROPERTYBASE_HPP
#define NAMEDPROPERTYBASE_HPP

struct NamedPropertyBase
{
	NamedPropertyBase();
	NamedPropertyBase(const NamedPropertyBase& other);
	virtual ~NamedPropertyBase();

	virtual NamedPropertyBase* CloneProperty() const = 0;
};

//------------------------------------------------------------------------------------------------------------------------------------


#endif