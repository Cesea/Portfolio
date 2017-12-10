#pragma once

class Player;
class BmpFont;
class IScene;

#include "Inventory.h"

class UI
{
private:

	Player* _pPlayer;

	bool _uiOn;

	video::Texture* _pHpBg{};
	video::Texture* _pFuryBg{};
	video::Texture* _pMiniMenu{};

	video::TextureHandle _hpBgHandle;
	video::TextureHandle _furyBgHandle;
	video::TextureHandle _miniMenuHandle;

	video::Texture* _pHp;
	video::Texture* _pFury;

	video::TextureHandle _hpHandle;
	video::TextureHandle _furyHandle;

	video::Texture* _pSlot;

	video::TextureHandle _slotHandle;

	video::Texture* _bt1_A;
	video::Texture* _bt1_B;
	bool _bt1On;
	video::TextureHandle _bt1AHandle;
	video::TextureHandle _bt1BHandle;

	video::Texture* _bt2_A;
	video::Texture* _bt2_B;
	bool _bt2On;
	video::TextureHandle _bt2AHandle;
	video::TextureHandle _bt2BHandle;

	video::Texture* _bt3_A;
	video::Texture* _bt3_B;
	bool _bt3On;
	video::TextureHandle _bt3AHandle;
	video::TextureHandle _bt3BHandle;

	video::Texture* _mush1;
	video::Texture* _mush2;
	video::Texture* _mush3;

	video::TextureHandle _mush1Handle;
	video::TextureHandle _mush2Handle;
	video::TextureHandle _mush3Handle;

	video::Texture* _potion1;
	video::Texture* _potion2;
	video::Texture* _potion3;

	video::TextureHandle _potion1Handle;
	video::TextureHandle _potion2Handle;
	video::TextureHandle _potion3Handle;

	float MaxHp;
	float CurHp;

	float MaxFury;
	float CurFury;

	Inventory* _inventory;

	IScene *_pCurrentScene;

public:

	void Update(float deltaTime, const InputManager &input);

	void RenderUI(void);

	bool Init(IScene *currentScene);
	void Release();

	UI();
	~UI();
};

