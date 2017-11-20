#pragma once

typedef struct tagPARTICLE_VERTEX {
	D3DXVECTOR3 pos;
	float size;
	DWORD color;
	enum { FVF = D3DFVF_XYZ | D3DFVF_PSIZE | D3DFVF_DIFFUSE };
}PARTICLE_VERTEX, *LPPARTICLE_VERTEX;

typedef std::vector<D3DXCOLOR>   VEC_COLOR;
typedef std::vector<float>			  VEC_SCALE;

class Particle
{
private :
	//파티클이 가지고있는거
	Vector3 _pos;
	Vector3 _accel;
	Vector3 _vel;
	float _time;
	float _maxTime;
	float _scale;

	bool fin;
public:
	Particle();
	~Particle();


	void createParticle(float maxTime, const Vector3 pos, const Vector3 accel, const Vector3 vel, float scale);
	void updateParticle(float timeDelta);
};

