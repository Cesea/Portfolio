#ifndef INVENTORY_H
#define INVENTORY_H

#include "BmpFont.h"

enum itemType
{
	Null,
	mush1,
	mush2,
	mush3,
	potion1,
	potion2,
	potion3
};

struct Bag
{
	POINT _pos;
	RECT _slotRect;
	itemType _type;
	int _value;
	int _index;
	BmpFont _valueImage;
};

class Inventory
{
private:

	itemType _tempType;
	int _tempValue;
	int _tempIndex;
	BmpFont _tempValueImage;

	bool _invenOn;

	Bag _bag[11];

	video::Texture* _inven;
	video::Texture* _pSlot;

	video::Texture* _mush1;
	video::Texture* _mush2;
	video::Texture* _mush3;

	video::Texture* _potion1;
	video::Texture* _potion2;
	video::Texture* _potion3;

	video::Texture* _synthesisImage;
	video::Texture* _synthesisImage_push;

	//video::Texture* _systhesisFont;

	bool _synthesisButton;

	video::Texture* _tBar;

	//video::Texture* _Box;

	RECT _invenRect;

	int _ivX;
	int _ivY;

	//RECT _synthesis;

	/*Bag _synthesis1;
	Bag _synthesis2;*/

public :
	void Update(float deltaTime, const InputManager &input);
	void Render();

	Inventory();
	~Inventory();

	void SetInvenOn(bool on) { _invenOn = on; }
	bool GetInvenOn(void) { return _invenOn; }

	bool Additem(itemType type, int val);	//반환값이 펄스면 아이템창이 꽉찬거임
	void DropItem(itemType type, int val);

	bool CheckBag(void);

	Bag* GetBag1(void) { return &_bag[0]; }
	Bag* GetBag2(void) { return &_bag[1]; }
	Bag* GetBag3(void) { return &_bag[2]; }
	Bag* GetBag4(void) { return &_bag[3]; }

	void closeInventory(void);

	void Synthesis(void);

protected :

};

#endif