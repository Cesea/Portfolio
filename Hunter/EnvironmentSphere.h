#ifndef ENVIRONMENT_SPHERE_H
#define ENVIRONMENT_SPHERE_H

//TODO : Implement this

class EnvironmentSphere
{
public :
	EnvironmentSphere();
	~EnvironmentSphere();

	void Create(const std::string &fileName);
	void Destroy();

	void Render(const Camera &camera);

private :
	IDirect3DCubeTexture9 *_pTexture;

	video::Effect *_pEffect{};
	video::EffectHandle _effectHandle;
	ID3DXMesh *_pMesh{};

};

#endif