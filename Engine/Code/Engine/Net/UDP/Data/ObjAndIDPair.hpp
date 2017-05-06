#pragma once

#ifndef OBJANDIDPAIR_HPP
#define OBJANDIDPAIR_HPP

struct ObjAndIDPair
{
public:
	void* object;
	int uniqueID;
public:
	ObjAndIDPair();
};

#endif