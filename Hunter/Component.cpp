#include "stdafx.h"
#include "Component.h"

bool Component::Init(Entity * pOwner)
{
	this->_pOwner = pOwner;
	return true;
}
