#pragma once

class Camera;
class PSystem
{
public:
	struct Particle
	{
		Vector3 initialPos;
		Vector3 initialVelocity;

		float initialSize;
		float initialTime;
		float lifeTime;
		float mass;
		D3DCOLOR initialColor;
	};
	IDirect3DVertexDeclaration9* Decl;


	// Access methods
	float getTime();
	void  setTime(float t);
	//const AABB& getAABB()const;

	void setWorldMtx(const D3DXMATRIX& world);
	void addParticle();
	void setCamera(Camera* camera, Vector3 pos);

	virtual void initParticle(Particle& out) = 0;
	virtual void update(float timeDelta);
	virtual void release();
	virtual void draw();
	bool makeParticle;

	void setPos(Vector3 newPos);
	Vector3 getPos();

protected:
	float timeAccum;
	Camera* _camera;
	Vector3 _cameraPos;
	D3DXHANDLE mhTech;
	D3DXHANDLE mhWVP;
	D3DXHANDLE mhEyePosL;
	D3DXHANDLE mhTex;
	D3DXHANDLE mhTime;
	D3DXHANDLE mhAccel;
	D3DXHANDLE mhViewportHeight;

	IDirect3DTexture9* mTex;
	IDirect3DTexture9* mTex2;
	IDirect3DTexture9* mTex3;
	IDirect3DVertexBuffer9* mVB;

	D3DXMATRIX mWorld;
	D3DXMATRIX mInvWorld;

	float mTime;
	Vector3 mAccel;
	int mMaxNumParticles;
	float mTimePerParticle;

	LPD3DXEFFECT particle_Effect;
	Vector3 Pos;

	std::vector<Particle> mParticles;
	std::vector<Particle*> mAliveParticles;
	std::vector<Particle*> mDeadParticles;

	std::string ab;

	float _radius;

	D3DXVECTOR3 AABBMin;
	D3DXVECTOR3 AABBMax;

public:
	PSystem(const std::string & fxName, const std::string & techName,
		const std::string & texName, const Vector3 & accel, int maxNumParticles, float timePerParticle, Vector3 pos);
	virtual ~PSystem();
};

