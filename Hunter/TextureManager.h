#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include "ResourceManager.h"
#include "Texture.h"

class TextureManager : public ResourceManager<Texture>
{
public :
	TextureManager();
	~TextureManager();
	TextureHandle CreateFromFile(const std::string &fileName);
	TextureHandle CreateFromMemory(int32 width, int32 height, void *pData);

	virtual bool32 Destroy(ResourceHandle handle);

};




#endif