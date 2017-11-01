#include "stdafx.h"
#include "Effect.h"


Effect::Effect()
{
}

Effect::~Effect()
{
	COM_RELEASE(_effect);
}

Effect::Effect(const Effect & other)
{
	_effect = other._effect;
}

Effect & Effect::operator=(const Effect & other)
{
	_effect = other._effect;
	return *this;
}

bool Effect::Load(const std::string &fileName, DWORD shaderFlag)
{
	bool result = true;;
	ID3DXBuffer *errorBuffer{};
	HRESULT hr = D3DXCreateEffectFromFile(gpDevice, fileName.c_str(), nullptr, NULL, shaderFlag, NULL, &_effect, &errorBuffer);
	if (FAILED(hr))
	{
		MessageBox(NULL, (char *)errorBuffer->GetBufferPointer(), "Error", MB_OK);
		return false;
	}

	return result;
}

D3DXHANDLE Effect::GetTechHandle(const char *name)
{
	D3DXHANDLE result = 0;
	result = _effect->GetTechniqueByName(name);
	return result;
}
D3DXHANDLE Effect::GetHandle(const char *name, D3DXHANDLE parent)
{
	D3DXHANDLE result = 0;
	result = _effect->GetParameterByName(parent, name);
	return result;
}

void Effect::SetTechnique(const D3DXHANDLE & handle)
{
	_effect->SetTechnique(handle);
}


uint32 Effect::Begin()
{
	uint32 result = 0;
	_effect->Begin(&result, 0);
	return result;
}

void Effect::End()
{
	_effect->End();
}

void Effect::BeginPass(uint32 passNum)
{
	_effect->BeginPass(passNum);
}

void Effect::EndPass()
{
	_effect->EndPass();
}

void Effect::SetValue(const D3DXHANDLE &handle, const void *data, uint32 size)
{
	_effect->SetValue(handle, data, size);
}
void Effect::SetFloat(const D3DXHANDLE &handle, float data)
{
	_effect->SetFloat(handle, data);
}
void Effect::SetMatrix(const D3DXHANDLE &handle, const D3DXMATRIXA16 &data)
{
	_effect->SetMatrix(handle, &data);
}
void Effect::SetInt(const D3DXHANDLE &handle, int data)
{
	_effect->SetInt(handle, data);
}

void Effect::SetTexture(const D3DXHANDLE & handle, LPDIRECT3DTEXTURE9 pTexture)
{
	_effect->SetTexture(handle, pTexture);
}

void Effect::SetValue(const std::string & name, const void * data, uint32 size)
{
	_effect->SetValue(name.c_str(), data, size);
}

void Effect::SetFloat(const std::string & name, float data)
{
	_effect->SetFloat(name.c_str(), data);
}

void Effect::SetMatrix(const std::string & name, const D3DXMATRIXA16 & data)
{
	_effect->SetMatrix(name.c_str(), &data);
}

void Effect::SetInt(const std::string & name, int data)
{
	_effect->SetInt(name.c_str(), data);
}

void Effect::SetTexture(const std::string & name, LPDIRECT3DTEXTURE9 pTexture)
{
	_effect->SetTexture(name.c_str(), pTexture);
}

void Effect::SetMatrixArray(const std::string & name, D3DXMATRIXA16 * matrices, uint32 numMatrix)
{
	_effect->SetMatrixArray(name.c_str(), matrices, numMatrix);
}
