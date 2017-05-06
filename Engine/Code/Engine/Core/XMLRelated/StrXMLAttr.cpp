#include "Engine/Core/XMLRelated/StrXMLAttr.hpp"
#include "Thirdparty/xmlParser/xmlParser.h"

//--------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------
//Constructors
StrXMLAttr::StrXMLAttr(const std::string& nam, const std::string& va)
	:name(nam),
	val(va)
{

}

StrXMLAttr::StrXMLAttr(const XMLAttribute& attr)
	: name(attr.lpszName),
	val(attr.lpszValue)
{

}

StrXMLAttr::StrXMLAttr(const StrXMLAttr& other)
	: name(other.name),
	val(other.val)
{

}

StrXMLAttr::~StrXMLAttr()
{

}