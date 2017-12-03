#pragma once

class Player;

class UI
{
private:

	Player* _player;

	video::Texture* _pHpBg{};
	video::Texture* _pFuryBg{};
	video::Texture* _pMiniMenu{};

public:

	void RenderUI(void);

	UI();
	~UI();

	
};

