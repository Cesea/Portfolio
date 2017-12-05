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

	DWORD m_dwParticleCount; //Ȱ��ȭ ����

	VEC_COLOR  m_Colors;
	VEC_SCALE   m_Scales;

	//��Ƽ�� ���̺� Ÿ�� �ּ� �ִ�
	float _liveTimeMin;
	float _liveTimeMax;

	//��ƼŬ ���� �ӵ� �ִ� �ּ�
	D3DXVECTOR3 _startVelMin;
	D3DXVECTOR3 _startVelMax;

	//���ӵ�
	D3DXVECTOR3 _startAccelMin;
	D3DXVECTOR3 _startAccelMax;

	//������
	float _startScaleMin;
	float _startScaleMax;

	float m_fEmissionIntervalTime; //���� ���� �ð�
	float m_fEmissionDeltaTime; //�ϳ� ������ ���� �ð�

	LPDIRECT3DTEXTURE9 m_pTex;

	bool m_bLocal;
public:
	//��ƼŬ ����Ÿ��
	PARTICLE_EMISSION_TYPE EmissionType;

	//��ƼŬ ���� ����
	float  MinEmissionRangeX; //x�� ���� ����
	float  MaxEmissionRangeX; //x�� ���� ����
	float  MinEmissionRangeY; //y�� ���� ����
	float  MaxEmissionRangeY; //y�� ���� ����
	float  MinEmissionRangeZ; //z�� ���� ����
	float  MaxEmissionRangeZ; //z�� ���� ����
	float SphereEmissionRange; // �������� ���� ������
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