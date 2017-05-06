#pragma once

#ifndef PROPERTYGETRESULT_HPP
#define PROPERTYGETRESULT_HPP

enum PropertyGetResult
{
	PROPERTY_FETCH_SUCCESS,
	PROPERTY_FETCH_FAILED_WRONG_TYPE,
	PROPERTY_FETCH_FAILED_NO_SUCH_PROPERTY,
	PROPERTY_FETCH_FAILED_NO_PROPERTIES, //Not sure if really need this one.
	NUM_OF_PROPERTY_GET_RESULTS
};

#endif