#include "stdafx.h"
#include "ParticleComponent.h"

ParticleComponent::ParticleComponent()
{
}


ParticleComponent::~ParticleComponent()
{
}


void ParticleComponent::Init(DWORD particleNum, float emission, float liveTimeMin, float liveTimeMax, const D3DXVECTOR3 & velocityMin,
	const D3DXVECTOR3 & velocityMax, const D3DXVECTOR3 & accelMin, const D3DXVECTOR3 & accelMax, const VEC_COLOR & colors, const VEC_SCALE & scales, 
	float scaleMin, float scaleMax, LPDIRECT3DTEXTURE9 pParticleTexture, bool bLocal)
{
	_totalNum = particleNum;
	m_ParticlVertices = new PARTICLE_VERTEX[_totalNum];
	_particle = new Particle[_totalNum];

	_emissionPerSec = emission;

	m_fEmissionIntervalTime = _emissionPerSec;

	m_fEmissionDeltaTime = 0.0f;

	isEmission = true;

	m_Colors.clear();
	for (int i = 0; i < colors.size(); i++)
		m_Colors.push_back(colors[i]);
	m_Scales.clear();
	for (int i = 0; i < scales.size(); i++)
		m_Scales.push_back(scales[i]);

	_liveTimeMin = liveTimeMin;
	_liveTimeMax = liveTimeMax;
	_startVelMin = velocityMin;
	_startVelMax = velocityMax;
	_startAccelMin = accelMin;
	_startAccelMax = accelMax;
	_startScaleMin = scaleMin;
	_startScaleMax = scaleMax;

	m_dwParticleCount = 0;

	m_pTex = pParticleTexture;

	m_bLocal = bLocal;

	EmissionType = PZERO;
}

void ParticleComponent::Release()
{
	SAFE_DELETE(_particle);
	SAFE_DELETE(m_ParticlVertices);
	SAFE_RELEASE(m_pTex);
}

void ParticleComponent::update(float timeDelta,Vector3 pos)
{
	for (int i = 0; i < _totalNum; i++)
		_particle[i].updateParticle(timeDelta);

	if (isEmission)
	{
		m_fEmissionDeltaTime += timeDelta;
		while (m_fEmissionDeltaTime >= m_fEmissionIntervalTime)
		{
			m_fEmissionDeltaTime -= m_fEmissionIntervalTime;
			MakeParticle(pos);
		}
	}
}

void ParticleComponent::render(Vector3 pos)
{
	//그릴 파티클 숫자
	DWORD drawParticleNum = 0;
	for (int i = 0; i < _totalNum; i++)
	{
		if (!_particle[i].fin)
		{
			//해당 파티클 정보를 얻는다.
			PARTICLE_VERTEX newVertex = _particle[i].GetParticleVertex(	m_Colors, m_Scales);
			LPPARTICLE_VERTEX t = m_ParticlVertices + drawParticleNum;
			t = &newVertex;
			drawParticleNum++;
		}
	}

	gpDevice->SetRenderState(D3DRS_LIGHTING, false);
	gpDevice->SetRenderState(D3DRS_POINTSPRITEENABLE, true);
	gpDevice->SetRenderState(D3DRS_POINTSCALEENABLE, true);
	gpDevice->SetRenderState(D3DRS_POINTSIZE_MIN, DWORD(0.0f));
	gpDevice->SetRenderState(D3DRS_POINTSIZE_MAX, DWORD(500.0f));
	gpDevice->SetRenderState(D3DRS_ZWRITEENABLE, false);
	gpDevice->SetRenderState(D3DRS_POINTSCALE_A, DWORD(1.0f));
	gpDevice->SetRenderState(D3DRS_POINTSCALE_B, DWORD(1.0f));
	gpDevice->SetRenderState(D3DRS_POINTSCALE_C, DWORD(1.0f));
	gpDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);

	gpDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	gpDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	gpDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	gpDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
	gpDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TEXTURE);

	//특수세팅
	//gpDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//gpDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	//gpDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	//gpDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
	//gpDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TEXTURE);
	//위치세팅
	Matrix matWorld;
	MatrixTranslation(&matWorld, pos.x, pos.y, pos.z);
	gpDevice->SetTransform(D3DTS_WORLD, &matWorld);
	//텍스쳐세팅
	gpDevice->SetTexture(0, m_pTex);
	//파티클 정점 출력
	gpDevice->SetFVF(PARTICLE_VERTEX::FVF);

	gpDevice->DrawPrimitiveUP(
		D3DPT_POINTLIST,
		drawParticleNum,
		m_ParticlVertices,
		sizeof(PARTICLE_VERTEX));



	gpDevice->SetTexture(0, NULL);
	gpDevice->SetRenderState(D3DRS_LIGHTING, true);
	gpDevice->SetRenderState(D3DRS_POINTSPRITEENABLE, false);
	//gpDevice->SetRenderState(D3DRS_POINTSCALEENABLE, false);
	gpDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	gpDevice->SetRenderState(D3DRS_ZWRITEENABLE, true);

	GIZMOMANAGER->AABBBox(Vector3(pos.x - 1.0f, pos.y - 1.0f, pos.z - 1.0f), Vector3(pos.x + 1.0f, pos.y + 1.0f, pos.z + 1.0f), 0xFFFF0000);
}

void ParticleComponent::MakeParticle(Vector3 pos)
{
	//라이브 타임 랜덤
	float liveTime = RandFloat(
		_liveTimeMin, _liveTimeMax);

	//파티클 생성 위치
	Vector3 position = pos;

	//생성 타입에 따른 추가 위치
	if (EmissionType == PARTICLE_EMISSION_TYPE::SPHERE)
	{
		//랜덤 벡터
		Vector3 randDir(
			RandFloat(-1.0f, 1.0f),
			RandFloat(-1.0f, 1.0f),
			RandFloat(-1.0f, 1.0f));
		Vec3Normalize(&randDir, &randDir);

		//랜덤 거리
		float randDistance = RandFloat(0, SphereEmissionRange);

		//추가 위치 
		position += randDir * randDistance;

	}
	else if (EmissionType == PARTICLE_EMISSION_TYPE::SPHERE_OUTLINE)
	{
		//랜덤 벡터
		Vector3 randDir(
			RandFloat(-1.0f, 1.0f),
			RandFloat(-1.0f, 1.0f),
			RandFloat(-1.0f, 1.0f));
		Vec3Normalize(&randDir, &randDir);

		//추가 위치 
		position += randDir * SphereEmissionRange;

	}
	else if (EmissionType == PARTICLE_EMISSION_TYPE::BOX)
	{
		//랜덤 벡터
		Vector3 randDir(
			RandFloat(MinEmissionRangeX, MaxEmissionRangeX),
			RandFloat(MinEmissionRangeY, MaxEmissionRangeY),
			RandFloat(MinEmissionRangeZ, MaxEmissionRangeZ));
		//D3DXVec3Normalize(&randDir, &randDir);

		//추가 위치 
		position += randDir;
	}
	//벡터 랜텀
	Vector3 velocity;
	velocity.x = RandFloat(_startVelMin.x, _startVelMax.x);
	velocity.y = RandFloat(_startVelMin.y, _startVelMax.y);
	velocity.z = RandFloat(_startVelMin.z, _startVelMax.z);

	Vector3 accelation;
	accelation.x = RandFloat(_startAccelMin.x, _startAccelMax.x);
	accelation.y = RandFloat(_startAccelMin.y, _startAccelMax.y);
	accelation.z = RandFloat(_startAccelMin.z, _startAccelMax.z);

	//if (m_bLocal == false)
	//{
	//	D3DXMATRIXA16 matWorld = this->pTransform->GetFinalMatrix();
	//	D3DXVec3TransformNormal(&velocity, &velocity, &matWorld);
	//	D3DXVec3TransformNormal(&accelation, &accelation, &matWorld);
	//}

	//스케일 랜덤
	float scale = RandFloat(_startScaleMin, _startScaleMax);

	//순번대로 발생 시킨다.
	_particle[m_dwParticleCount].createParticle(
		liveTime,
		pos, velocity, accelation, scale);

	m_dwParticleCount++;
	if (m_dwParticleCount == this->_totalNum)
		m_dwParticleCount = 0;
}

