#include "stdafx.h"
#include "PSystem.h"
#include "Camera.h"

PSystem::PSystem(const std::string & fxName, const std::string & techName,
	const std::string & texName, const D3DXVECTOR3 & accel, int maxNumParticles, float timePerParticle, D3DXVECTOR3 pos)
{
	mMaxNumParticles = maxNumParticles;
	mTimePerParticle = timePerParticle;
	mAccel = accel;
	mhTex = texName.c_str();
	ab = techName;
	mhTech = ab.c_str();

	mParticles.resize(mMaxNumParticles);
	mAliveParticles.reserve(mMaxNumParticles);
	mDeadParticles.reserve(mMaxNumParticles);

	for (int i = 0; i < mMaxNumParticles; ++i)
	{
		mParticles[i].lifeTime = 0.0f;
		mParticles[i].initialTime = 1.0f;
	}

	D3DXCreateTextureFromFile(gpDevice, mhTex, &mTex);
	LPD3DXBUFFER pError = NULL;
	//particle_Effect = RESOURCE_FX->GetResource(fxName);
    D3DXCreateEffectFromFile(
		gpDevice,					
		fxName.c_str(),				
		NULL, 						
		NULL, 						
		0,				            
		NULL, 						
		&particle_Effect,			
		&pError 					
	);

	mTime = 0;

	D3DXMatrixIdentity(&mWorld);
	D3DXMatrixInverse(&mInvWorld, 0, &mWorld);

	Pos = pos;
	timeAccum = 0.0f;
	makeParticle = true;;
}

PSystem::~PSystem()
{
}

void PSystem::setWorldMtx(const D3DXMATRIX & world)
{
	mWorld = world;
	D3DXMatrixInverse(&mInvWorld, 0, &mWorld);
}

void PSystem::addParticle()
{
	if (mDeadParticles.size() > 0)
	{
		// Reinitialize a particle.
		Particle* p = mDeadParticles.back();
		initParticle(*p);

		// No longer dead.
		mDeadParticles.pop_back();
		mAliveParticles.push_back(p);
	}

}


void PSystem::setCamera(Camera* camera,Vector3 pos)
{
	_camera = camera;
	_cameraPos = pos;
}

void PSystem::update(float timeDelta)
{
	mTime += timeDelta;

	mDeadParticles.resize(0);
	mAliveParticles.resize(0);

	// For each particle.
	for (int i = 0; i < mMaxNumParticles; ++i)
	{
		// Is the particle dead?

		if ((mTime - mParticles[i].initialTime) >
			mParticles[i].lifeTime)
		{
			mDeadParticles.push_back(&mParticles[i]);
		}
		else
		{
			mAliveParticles.push_back(&mParticles[i]);
		}
	}

	// A negative or zero mTimePerParticle value denotes
	// not to emit any particles.
	if (mTimePerParticle > 0.0f&&makeParticle)
	{
		// Emit particles.
		timeAccum += timeDelta;
		while (timeAccum >= mTimePerParticle)
		{
			addParticle();
			timeAccum -= mTimePerParticle;
		}
	}
	else
	{
		timeAccum = 0.0f;
	}

}

void PSystem::release()
{
	SAFE_RELEASE(particle_Effect);
	SAFE_RELEASE(mTex);
	//SAFE_RELEASE(mVB);
}

void PSystem::draw()
{

	if (!mAliveParticles.empty())
	{
		gpDevice->SetRenderState(D3DRS_LIGHTING, false);
		D3DXVECTOR3 eyePosW = _cameraPos;
		D3DXVECTOR3 eyePosL;
		D3DXVec3TransformCoord(&eyePosL, &eyePosW, &mInvWorld);

		// Set FX parameters.
		particle_Effect->SetValue("gEyePosL", &eyePosW, sizeof(D3DXVECTOR3));
		particle_Effect->SetFloat("gTime", mTime);
		particle_Effect->SetMatrix("gWVP", &(mWorld*_camera->GetViewMatrix()*_camera->GetProjectionMatrix()));

		//HWND hwnd = gEngine->;
		RECT clientRect;

		GetClientRect(gWindowHandle, &clientRect);
		particle_Effect->SetInt("gViewportHeight", clientRect.bottom);
		particle_Effect->SetValue("gAccel", mAccel, sizeof(D3DXVECTOR3));


		particle_Effect->SetTexture("gTex", mTex);


		//mhTech = "FireBallTech";
		particle_Effect->SetTechnique(mhTech);
		//particle_Effect->CommitChanges();

		UINT numPasses = 0;
		particle_Effect->Begin(&numPasses, 0);
		particle_Effect->BeginPass(0);


		gpDevice->CreateVertexBuffer(
			sizeof(Particle)*mAliveParticles.size(),
			D3DUSAGE_WRITEONLY | D3DUSAGE_POINTS,
			0,
			D3DPOOL_MANAGED,
			&mVB,
			0);
		Particle* p = 0;
		mVB->Lock(0, 0, (void**)&p, D3DLOCK_DISCARD);
		int vbIndex = 0;
		// For each living particle.
		for (UINT i = 0; i < mAliveParticles.size(); ++i)
		{
			// Copy particle to VB
			p[vbIndex] = *mAliveParticles[i];
			++vbIndex;
		}
		//memcpy(&p, &mVB, sizeof(Particle)*vbIndex);
		mVB->Unlock();

		gpDevice->SetStreamSource(0, mVB, 0, sizeof(Particle));
		gpDevice->SetVertexDeclaration(Decl);
		// Render however many particles we copied over.
		if (vbIndex > 0)
		{
			HRESULT t = gpDevice->DrawPrimitive(
				D3DPT_POINTLIST, 0, vbIndex);
		}

		particle_Effect->EndPass();
		particle_Effect->End();
		gpDevice->SetRenderState(D3DRS_LIGHTING, true);
	}
}

void PSystem::setPos(D3DXVECTOR3 newPos)
{
	Pos = newPos;
}
D3DXVECTOR3 PSystem::getPos()
{
	return Pos;
}


float PSystem::getTime()
{
	return 0.0f;
}

void PSystem::setTime(float t)
{
	mTime = t;
}

