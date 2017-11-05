#ifndef RESOURCE_H
#define RESOURCE_H

#include "Typedefs.h"

class Resource
{
public :
	Resource() {}
	virtual ~Resource() {}

	void SetID(uint32 id) { _id = id; }
	uint32 GetID() { return _id; }

protected :
	uint32 _id;
};

#endif