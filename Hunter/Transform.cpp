#include "stdafx.h"
#include "Transform.h"

Transform::Transform()
{
	SetTranslation(0.0f, 0.0f, 0.0f);
	SetRotation(0.0f);
	SetScaling(1.0f, 1.0f, 1.0f);
	SetAxis(0.0f, 1.0f, 0.0f);
}

Matrix Transform::GetWorldMatrix()
{
	Matrix translation, rotation, scaling;

	MatrixTranslation(&translation, _translation.x, _translation.y, _translation.z);
	MatrixRotationAxis(&rotation, &_rotationAxis, _rotation);
	MatrixScaling(&scaling, _scaling.x, _scaling.y, _scaling.z);

	MatrixMultiply(&scaling, &scaling, &rotation);
	MatrixMultiply(&scaling, &scaling, &translation);

	return scaling;
}

Matrix Transform::GetModelMatrix()
{
	Matrix rotation;
	MatrixRotationAxis(&rotation, &_rotationAxis, _rotation);
	return rotation;
}

Matrix Transform::GetModelViewProjectionMatrix()
{
	return Matrix();
}

const Vector3 &Transform::GetTranslation()
{
	return _translation;
}

void Transform::SetTranslation(const Vector3 &translation)
{
	_translation = translation;
}

const float Transform::GetRotation()
{
	return _rotation;
}


const Vector3 &Transform::GetScaling()
{
	return _scaling;
}

void Transform::SetScaling(const Vector3 &scaling)
{
	_scaling = scaling;
}

void Transform::SetScaling(float x, float y, float z)
{
	_scaling.x = x;
	_scaling.y = y;
	_scaling.z = z;
}

void Transform::SetTranslation(float x, float y, float z)
{
	_translation.x = x;
	_translation.y = y;
	_translation.z = z;
}

void Transform::SetRotation(float f)
{
	_rotation = f;
}

void Transform::SetAxis(float x, float y, float z)
{
	_rotationAxis.x = x;
	_rotationAxis.y = y;
	_rotationAxis.z = z;
}

void Transform::SetAxis(const Vector3 &axis)
{
	_rotationAxis = axis;
}

const Vector3 &Transform::GetAxis()
{
	return _rotationAxis;
}

void Transform::MoveBy(float x, float y, float z)
{
	_translation.x += x;
	_translation.y += y;
	_translation.z += z;
}

void Transform::MoveBy(const Vector3 & offset)
{
	_translation += offset;
}
