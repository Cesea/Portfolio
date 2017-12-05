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
	if (fin)return;
	if (_maxTime < _time)
	{
		fin = true;
	}

	_time += timeDelta;

	_pos = _pos + _vel*timeDelta;
	_vel += _vel + _accel*timeDelta;
}

PARTICLE_VERTEX Particle::GetParticleVertex(const VEC_COLOR & colors, const VEC_SCALE & scales)
{
	PARTICLE_VERTEX out;
	out.pos = _pos;

	float t = _time / _maxTime;
	if (t >= 1)
	{
		out.color = colors[colors.size() - 1];
		out.size = _scale * scales[scales.size() - 1];
		return out;
	}
	else if (t == 0)
	{
		out.color = colors[0];
		out.size = _scale * scales[0];
		return out;
	}

	float finalSize = 0.0f;
	int lastIndex = scales.size() - 1;
	float interval = 1.0f / lastIndex;

	//�ð��� �󸶳��귶��
	float position = t / interval;

	//�����κ�
	int startIndex = (int)position;
	int endIndex = startIndex + 1;

	//�Ҽ��κ�
	float fN = position - startIndex;

	finalSize = scales[startIndex] + interval*fN;
	out.size = _scale * finalSize;


	//�÷� ����
	lastIndex = colors.size() - 1;
	interval = 1.0f / lastIndex;
	position = t / interval;
	startIndex = (int)position;
	endIndex = startIndex + 1;
	fN = position - startIndex;
	//���� ����
	D3DXCOLOR color;
	D3DXColorLerp(	&color,&colors[startIndex],&colors[endIndex],fN);
	out.color = (DWORD)color;


	return out;
}
