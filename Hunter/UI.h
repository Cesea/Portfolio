#pragma once

class Player;
class BmpFont;

#include "Inventory.h"

class UI
{
private:

	Player* _player;

	bool _uiOn;

	video::Texture* _pHpBg{};
	video::Texture* _pFuryBg{};
	video::Texture* _pMiniMenu{};

	video::Texture* _pHp;
	video::Texture* _pFury;

	video::Texture* _pSlot;

	video::Texture* _bt1_A;
	video::Texture* _bt1_B;
	bool _bt1On;

	video::Texture* _bt2_A;
	video::Texture* _bt2_B;
	bool _bt2On;

	video::Texture* _bt3_A;
	video::Texture* _bt3_B;
	bool _bt3On;

	video::Texture* _mush1;
	video::Texture* _mush2;
	video::Texture* _mush3;

	video::Texture* _potion1;
	video::Texture* _potion2;
	video::Texture* _potion3;

	float MaxHp;
	float CurHp;

	float MaxFury;
	float CurFury;

	Inventory* _inventory;

public:

	void Update(float deltaTime, const InputManager &input);

	void RenderUI(void);

	UI();
	~UI();

	
};

