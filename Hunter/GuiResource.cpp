#include "stdafx.h"
#include "GuiResource.h"

#include "Dialog.h"


using namespace gui;

DialogResourceManager::DialogResourceManager()
{
	_pStateBlock = nullptr;
	_pSprite = nullptr;
}

DialogResourceManager::~DialogResourceManager()
{
	for (int32 i = 0; i < _fontCache.size(); i++)
	{
		FontNode* pFontNode = _fontCache[i];
		SAFE_DELETE(pFontNode);
	}
	_fontCache.clear();

	for (int32 i = 0; i < _textureCache.size(); i++)
	{
		TextureNode* pTextureNode = _textureCache[i];
		SAFE_DELETE(pTextureNode);
	}
	_textureCache.clear();
}

bool DialogResourceManager::Shutdown()
{
	return true;
}

bool DialogResourceManager::Initialize()
{
	//for (i = 0; i < m_FontCache.GetSize(); i++)
	//{
	//	hr = CreateFont9(i);
	//	if (FAILED(hr))
	//		return hr;
	//}

	//for (i = 0; i < m_TextureCache.GetSize(); i++)
	//{
	//	hr = CreateTexture9(i);
	//	if (FAILED(hr))
	//		return hr;
	//}
	HRESULT hr = D3DXCreateSprite(gpDevice, &_pSprite);
	if (FAILED(hr))
	{
		return false;
	}
	return true;
}

int32 DialogResourceManager::AddFont(const std::string & faceName, int32 height, int32 weight)
{
	// See if this font already exists
	for (int32 i = 0; i < _fontCache.size(); i++)
	{
		FontNode* pFontNode = _fontCache[i];
		if (0 == strcmp(pFontNode->face, faceName.c_str()) &&
			pFontNode->height == height &&
			pFontNode->width == weight)
		{
			return i;
		}
	}

	// Add a new font and try to create it
	FontNode* pNewFontNode = new FontNode;
	if (pNewFontNode == NULL)
		return -1;

	ZeroMemory(pNewFontNode, sizeof(FontNode));
	strcpy(pNewFontNode->face, faceName.c_str());
	pNewFontNode->height = height;
	pNewFontNode->width = weight;
	_fontCache.push_back(pNewFontNode);

	int iFont = _fontCache.size() - 1;

	// If a device is available, try to create immediately
	CreateFontResource(iFont);
	return iFont;
}

int DialogResourceManager::AddTexture(const std::string & fileName)
{
	// See if this texture already exists
	for (int32 i = 0; i < _textureCache.size(); i++)
	{
		TextureNode *pTextureNode = _textureCache[i];
		if (0 == strcmp(pTextureNode->fileName, fileName.c_str()))
		{
			return i;
		}
	}

	// Add a new font and try to create it
	TextureNode *pNewTextureNode = new TextureNode;
	if (pNewTextureNode == NULL)
		return -1;

	ZeroMemory(pNewTextureNode, sizeof(TextureNode));
	strcpy(pNewTextureNode->fileName, fileName.c_str());
	_textureCache.push_back(pNewTextureNode);

	int32 textureIndex = _textureCache.size() - 1;

	// If a device is available, try to create immediately
	CreateTextureResource(textureIndex);
	return textureIndex;
}

bool DialogResourceManager::RegisterDialog(Dialog * pDialog)
{
	bool result = true;
	for (int32 i = 0; i < _dialogs.size(); ++i)
	{
		if (_dialogs[i] == pDialog)
		{
			//Dialog already exist
			return true;
		}
	}

	_dialogs.push_back(pDialog);
	return true;
}

void DialogResourceManager::UnregisterDialog(Dialog * pDialog)
{
	bool result = true;
	for (int32 i = 0; i < _dialogs.size(); ++i)
	{
		if (_dialogs[i] == pDialog)
		{
			SAFE_DELETE(pDialog);
			return;
		}
	}
}

void DialogResourceManager::EnableKeyboardInputForAllDialogs()
{
	for (int32 i = 0; i < _dialogs.size(); ++i)
	{
		//_dialogs[i]->Enable
	}
}

bool DialogResourceManager::CreateFontResource(uint32 index)
{
	FontNode *pNode = _fontCache[index];
	//D3DXCreateFont()

	HRESULT hresult = D3DXCreateFont(gpDevice, pNode->height, 0, pNode->width, 1, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
		pNode->face, &pNode->pFont);
	if (FAILED(hresult))
	{
		return false;
	}
	return true;
}

bool DialogResourceManager::CreateTextureResource(uint32 index)
{
	TextureNode *pNode = _textureCache[index];
	D3DXIMAGE_INFO info;
	HRESULT hresult = D3DXGetImageInfoFromFile(pNode->fileName, &info);
	if (FAILED(hresult))
	{
		return false;
	}
	pNode->width = info.Width;
	pNode->height = info.Height;

	hresult = D3DXCreateTextureFromFile(gpDevice, pNode->fileName, &pNode->pTexture);
	if (FAILED(hresult))
	{
		return false;
	}
	return true;
}

TextureNode::~TextureNode()
{
	COM_RELEASE(pTexture);
}

FontNode::~FontNode()
{
	COM_RELEASE(pFont);
}