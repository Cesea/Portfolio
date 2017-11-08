#ifndef TEXTURE_H
#define TEXTURE_H

#include "Resource.h"

class Texture : public Resource
{
public :
	Texture();
	virtual ~Texture();

	bool32 Create(const std::string &fileName);
	bool32 Create(int32 width, int32 height, void *pData = nullptr);

	bool32 Destroy();

	int32 GetWidth() const { return _width; }
	int32 GetHeight() const { return _height; }

	LPDIRECT3DTEXTURE9 GetTexture() { return _pTexture; }

private :

	int32 _width;
	int32 _height;

	LPDIRECT3DTEXTURE9 _pTexture;

};


#endif