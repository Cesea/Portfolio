#ifndef GAME_COMMAND_H
#define GAME_COMMAND_H

struct Movement
{
	enum Vertical
	{
		eNoneV,
		eUp,
		eDown
	};

	enum Horizontal
	{
		eNoneH,
		eLeft,
		eRight
	};

	Movement::Vertical _vertical{Movement::Vertical::eNoneV};
	Movement::Horizontal _horizontal{Movement::Horizontal::eNoneH};
};

struct Behavior
{
	enum Type
	{
		eNone,
		eAttack,
		eSpecialAttack,
		eSkill,
		eBlock,
		eFly,
		eInteract
	};
	Behavior::Type _type{Behavior::Type::eNone};
	int32 _usageIndex{0};
};

struct GameCommand
{
	enum Type
	{
		eNone,
		eMove,
		eAction,
		eJump,
		eInteract,
	};

	Type _type{ GameCommand::Type::eNone };
	Movement _movement;
	Behavior _behavior;
	bool32 _interpreted{false};

	void Reset();

	GameCommand();
	GameCommand(const GameCommand &other);
	GameCommand &operator= (const GameCommand &other);
};


#endif