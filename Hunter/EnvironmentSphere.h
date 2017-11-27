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
	video::TextureHandle _texture;
	video::EffectHandle _effect;
	ID3DXMesh *_pMesh{};

};

#endif