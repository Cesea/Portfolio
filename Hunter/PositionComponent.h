#ifndef POSITION_COMPONENT_H
#define POSITION_COMPONENT_H

struct PositionComponent : public Component
{
public :
	PositionComponent(const Vector3 &position);
	virtual ~PositionComponent() {}


	Vector3 position;
};

#endif