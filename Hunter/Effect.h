#ifndef EFFECT_H
#define EFFECT_H

#include "Resource.h"

class Effect : public Resource
{
private :
	Effect();
	~Effect();
public:
	bool32 Create(const std::string &fileName, DWORD shaderFlag = 0);
	bool32 Destroy();

	D3DXHANDLE GetTechHandle(const char *name);
	D3DXHANDLE GetHandle(const char *name, D3DXHANDLE parent = NULL);

	void SetTechnique(const D3DXHANDLE &handle);

	void SetValue(const D3DXHANDLE &handle, const void *data, uint32 size);
	void SetFloat(const D3DXHANDLE &handle, float data);
	void SetMatrix(const D3DXHANDLE &handle, const D3DXMATRIXA16 &data);
	void SetInt(const D3DXHANDLE &handle, int data);
	void SetTexture(const D3DXHANDLE &handle, LPDIRECT3DTEXTURE9 pTexture);

	void SetValue(const std::string &name, const void *data, uint32 size);
	void SetFloat(const std::string &name, float data);
	void SetMatrix(const std::string &name, const D3DXMATRIXA16 &data);
	void SetInt(const std::string &name, int data);
	void SetTexture(const std::string &name, LPDIRECT3DTEXTURE9 pTexutre);

	void SetMatrixArray(const std::string &name, D3DXMATRIXA16 *matrices, uint32 numMatrix);

	uint32 Begin();
	void End();

	void BeginPass(uint32 passNum);
	void EndPass();

	inline void CommitChanges()
	{
		_effect->CommitChanges();
	}


private:
	ID3DXEffect *_effect{};

	Effect(const Effect &other);
	Effect &operator= (const Effect &other);
};



#endif