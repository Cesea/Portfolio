#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "Vector3.h"
#include "Matrix.h"

class Transform
{

public  :
	Transform();

	Matrix GetWorldMatrix();

	Matrix GetModelMatrix();

	Matrix GetModelViewProjectionMatrix();

	const Vector3 &GetTranslation();

	void SetTranslation(const Vector3 &translation);

	const float GetRotation();

	const Vector3 &GetScaling(); 

	void SetScaling(const Vector3 &scaling); 

	void SetScaling(float x, float y, float z); 

	void SetTranslation(float x, float y, float z); 

	void SetRotation(float f);

	void SetAxis(float x, float y, float z);
	void SetAxis(const Vector3 &axis);
	const Vector3 &GetAxis();

	void MoveBy(float x, float y, float z);
	void MoveBy(const Vector3 &offset);

private:

	Vector3 _translation;
	Vector3 _scaling;

	Vector3 _rotationAxis;
	float _rotation;
};


#endif