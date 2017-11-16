#ifndef MOVE_COMPONENT_H
#define MOVE_COMPONENT_H

#include "Component.h"

struct MoveComponent : public Component
{
public :
	MoveComponent();
	virtual ~MoveComponent() {}

	Vector3 velocity{};
	Vector3 accel{};

};

#endif
