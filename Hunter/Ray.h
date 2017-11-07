#ifndef RAY_H
#define RAY_H

//#include "Vector3.h"

class Ray
{
public :
	DECLARE_META(Ray);

	Ray();
	Ray(const Vector3 &origin, const Vector3 &direction);
	Ray(const Ray &other);
	Vector3 origin;
	Vector3 direction;
};

#endif