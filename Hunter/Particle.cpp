#include "stdafx.h"
#include "Particle.h"

Particle::Particle()
{
}


Particle::~Particle()
{
}

void Particle::createParticle(float maxTime, const Vector3 pos, const Vector3 accel, const Vector3 vel, float scale)
{
	_maxTime = maxTime;
	_pos = pos;
	_accel = accel;
	_vel = vel;
	_scale = scale;
	_time = 0;

	fin = false;

}

void Particle::updateParticle(float timeDelta)
{
}
