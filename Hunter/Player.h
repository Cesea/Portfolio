#ifndef PLAYER_H
#define PLAYER_H

struct Command
{
	enum Type
	{
		eMove,
		eAttack,
		eJump,
	};
};

class Player 
{
public :
	Player();
	virtual ~Player();

	void CreateFromWorld(World &world);
	void Update(float deltaTime);

	void Handle(const InputManager::KeyPressedEvent &event);
protected :
	void InterpretCommand();

	Entity entity;

	int32 _animationIndex{-1};

};

#endif