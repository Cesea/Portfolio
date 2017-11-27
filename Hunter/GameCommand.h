#ifndef GAME_COMMAND_H
#define GAME_COMMAND_H

enum VERTICAL_MOVEMENT_TYPE
{
	VERTICAL_MOVEMENT_NONE,
	VERTICAL_MOVEMENT_UP,
	VERTICAL_MOVEMENT_DOWN,
};

enum HORIZONTAL_MOVEMENT_TYPE
{
	HORIZONTAL_MOVEMENT_NONE,
	HORIZONTAL_MOVEMENT_LEFT,
	HORIZONTAL_MOVEMENT_RIGHT,
};

struct Movement
{
	VERTICAL_MOVEMENT_TYPE _vertical;
	HORIZONTAL_MOVEMENT_TYPE _horizontal;
};

bool IsMovementNone(Movement &movement);

void MovementStop(Movement &movement);
void MovementLeft(Movement &movement);
void MovementRight(Movement &movement);
void MovementUp(Movement &movement);
void MovementDown(Movement &movement);

enum BEHAVIOR_TYPE
{
	BEHAVIOR_NONE,
	BEHAVIOR_ATTACK,
	BEHAVIOR_SPECIAL_ATTACK,
	BEHAVIOR_SKILL,
	BEHAVIOR_BLOCK,
	BEHAVIOR_FLY,
	BEHAVIOR_INTERACT
};

struct Behavior
{
	BEHAVIOR_TYPE _type{BEHAVIOR_NONE};
	int32 _usageIndex{0};
};


enum GAMECOMMAND_TYPE
{
	GAMECOMMAND_NONE,
	GAMECOMMAND_MOVE,
	GAMECOMMAND_ACTION,
	GAMECOMMAND_JUMP,
	GAMECOMMAND_INTERACT,
};

struct GameCommand
{
	GAMECOMMAND_TYPE _type{ GAMECOMMAND_NONE };
	Movement _movement;
	Behavior _behavior;
	bool32 _interpreted{false};

	void Reset();

	GameCommand();
	GameCommand(const GameCommand &other);
	GameCommand &operator= (const GameCommand &other);
};


#endif