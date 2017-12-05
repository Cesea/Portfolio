#ifndef PARTICLE_COMPONENT_H
#define PARTICLE_COMPONENT_H
#include "Component.h"
#include "Particle.h"
class ParticleComponent :
	public Component
{
public:
	enum PARTICLE_EMISSION_TYPE
	{
		PZERO,
		SPHERE,
		SPHERE_OUTLINE,
		BOX
	};
private:
	DWORD _totalNum;
	Particle* _particle;
	LPPARTICLE_VERTEX m_ParticlVertices;
	float _emissionPerSec;

	DWORD m_dwParticleCount; //활성화 순번

	VEC_COLOR  m_Colors;
	VEC_SCALE   m_Scales;

	//파티글 라이브 타임 최소 최대
	float _liveTimeMin;
	float _liveTimeMax;

	//파티클 시작 속도 최대 최소
	D3DXVECTOR3 _startVelMin;
	D3DXVECTOR3 _startVelMax;

	//가속도
	D3DXVECTOR3 _startAccelMin;
	D3DXVECTOR3 _startAccelMax;

	//사이즈
	float _startScaleMin;
	float _startScaleMax;

	float m_fEmissionIntervalTime; //생성 간격 시간
	float m_fEmissionDeltaTime; //하나 생성후 지난 시간

	LPDIRECT3DTEXTURE9 m_pTex;

	bool m_bLocal;
public:
	//파티클 생성타입
	PARTICLE_EMISSION_TYPE EmissionType;

	//파티클 생성 범위
	float  MinEmissionRangeX; //x의 생성 법위
	float  MaxEmissionRangeX; //x의 생성 법위
	float  MinEmissionRangeY; //y의 생성 법위
	float  MaxEmissionRangeY; //y의 생성 법위
	float  MinEmissionRangeZ; //z의 생성 법위
	float  MaxEmissionRangeZ; //z의 생성 법위
	float SphereEmissionRange; // 구생성시 생성 반지름
	bool isEmission;
	void Init(
		DWORD particleNum,
		float emission,
		float liveTimeMin,
		float liveTimeMax,
		const D3DXVECTOR3& velocityMin,
		const D3DXVECTOR3& velocityMax,
		const D3DXVECTOR3& accelMin,
		const D3DXVECTOR3& accelMax,
		const VEC_COLOR& colors,
		const VEC_SCALE& scales,
		float scaleMin,
		float scaleMax,
		LPDIRECT3DTEXTURE9 pParticleTexture,
		bool bLocal = false
	);

	void Release();

	void update(float timeDelta, Vector3 pos);

	void render(Vector3 pos);



	ParticleComponent();
	virtual ~ParticleComponent();
private:
	void MakeParticle(Vector3 pos);
};

#endif