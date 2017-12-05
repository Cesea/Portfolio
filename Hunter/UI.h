#pragma once

class Player;

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

	float MaxHp;
	float CurHp;

	float MaxFury;
	float CurFury;

public:

	void Update(float deltaTime, const InputManager &input);

	void RenderUI(void);

	UI();
	~UI();

	
};

