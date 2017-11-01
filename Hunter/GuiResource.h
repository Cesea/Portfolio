#ifndef GUI_RESOURCE_H
#define GUI_RESOURCE_H

namespace gui
{
	class Dialog;

	struct TextureNode
	{
		~TextureNode();
		char fileName[MAX_PATH];
		uint32 width;
		uint32 height;
		IDirect3DTexture9* pTexture;
	};

	struct FontNode
	{
		~FontNode();
		char face[MAX_PATH];
		int32 width;
		int32 height;;
		ID3DXFont *pFont;
	};

	class DialogResourceManager
	{
	public :
		DialogResourceManager();
		~DialogResourceManager();

		bool Initialize();
		bool Shutdown();
		//HRESULT OnD3D9ResetDevice();
		//void    OnD3D9LostDevice();
		//void    OnD3D9DestroyDevice();
		FontNode* GetFontNode(int32 index)
		{
			return _fontCache[index];
		};
		TextureNode* GetTextureNode(int32 index)
		{
			return _textureCache[index];
		};
		int     AddFont(const std::string &faceNaame, int32 height, int32 weight);
		int     AddTexture(const std::string &fileName);

		bool    RegisterDialog(Dialog* pDialog);
		void    UnregisterDialog(Dialog* pDialog);
		void    EnableKeyboardInputForAllDialogs();

		IDirect3DStateBlock9* _pStateBlock;
		ID3DXSprite* _pSprite;          // Sprite used for drawing

		std::vector<Dialog *> _dialogs;
	protected:
		bool CreateFontResource(uint32 index);
		bool CreateTextureResource(uint32 index);

		std::vector<TextureNode *> _textureCache;
		std::vector<FontNode *> _fontCache;

	};

}

#endif