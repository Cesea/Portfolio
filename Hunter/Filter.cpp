#include "stdafx.h"
#include "Filter.h"

bool32 Filter::DoesPassFilter(const ComponentTypeList & typeList) const
{
	for (uint32 i = 0; i < _requires.size(); ++i)
	{
		if (_requires[i] == true && typeList[i] == false)
		{
			return false;
		}
	}

	if ((_excludes & typeList).any())
	{
		return false;
	}

	return true;
}
