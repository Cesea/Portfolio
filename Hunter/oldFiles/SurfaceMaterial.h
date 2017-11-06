#ifndef SURFACE_MATERIAL_H
#define SURFACE_MATERIAL_H

constexpr uint32 MAX_NUM_TEXTURE = 8;

class Texture : public Resource
{
	friend class TextureManager;
	friend class SurfaceMaterialManager;
private :
	Texture();
	~Texture();
	Texture(const Texture &other);

public:
	LPDIRECT3DTEXTURE9 pTexture{};
	int32 width{};
	int32 height{};
private :
};

class SurfaceMaterial 
{
	friend class SurfaceMaterialManager;
public:
	D3DMATERIAL9 material;
	Texture *textures[MAX_NUM_TEXTURE] = {nullptr, };

private :


};

#endif