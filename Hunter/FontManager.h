#ifndef FONT_MANAGER_H
#define FONT_MANAGER_H

#include "ResourceManager.h"	

class FontManager : public ResourceManager<ID3DXFont, FontManager>
{
public:
	FontManager(void);
	~FontManager(void);

	virtual Effect *LoadResource(const std::string &filePath, void* pParam = NULL);

	virtual void ReleaseResource(ID3DXFont *pData);
	virtual void ReleaseResource(const std::string &key);

};

#endif