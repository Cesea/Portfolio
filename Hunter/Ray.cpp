#include "stdafx.h"

DEFINE_META(Ray)
{
	ADD_MEMBER(origin);
	ADD_MEMBER(direction);
}

Ray::Ray()
	:origin(), direction()
{
}

Ray::Ray(const Vector3 & origin, const Vector3 & direction)
{
	this->origin = origin;
	this->direction = direction;
}

Ray::Ray(const Ray & other)
{
	this->origin = other.origin;
	this->direction = other.direction;
}
