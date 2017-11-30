#include "stdafx.h"

void GameCommand::Reset()
{
	_type = GAMECOMMAND_NONE;
	_movement._vertical = VERTICAL_MOVEMENT_NONE;
	_movement._horizontal = HORIZONTAL_MOVEMENT_NONE;
	_behavior._type = BEHAVIOR_NONE;
	_behavior._usageIndex = 0;
	_dash = false;
}

GameCommand::GameCommand()
{
	Reset();
}

GameCommand::GameCommand(const GameCommand & other)
{
	_type = other._type;
	_movement._vertical = other._movement._vertical;
	_movement._horizontal = other._movement._horizontal;
	_behavior._type = other._behavior._type;
	_behavior._usageIndex = other._behavior._usageIndex;
	_dash = other._dash;
}

GameCommand & GameCommand::operator=(const GameCommand & other)
{
	_type = other._type;
	_movement._vertical = other._movement._vertical;
	_movement._horizontal = other._movement._horizontal;
	_behavior._type = other._behavior._type;
	_behavior._usageIndex = other._behavior._usageIndex;
	_dash = other._dash;

	return *this;
}

bool IsMovementNone(Movement & movement)
{
	return (movement._horizontal == HORIZONTAL_MOVEMENT_NONE) && (movement._vertical == VERTICAL_MOVEMENT_NONE);
}

void MovementStop(Movement & movement)
{
	movement._vertical = VERTICAL_MOVEMENT_NONE;
	movement._horizontal = HORIZONTAL_MOVEMENT_NONE;
}

//void MovementLeft(Movement & movement)
//{
//	movement._vertical = VERTICAL_MOVEMENT_NONE;
//	movement._horizontal = HORIZONTAL_MOVEMENT_LEFT;
//}
//
//void MovementRight(Movement & movement)
//{
//	movement._vertical = VERTICAL_MOVEMENT_NONE;
//	movement._horizontal = HORIZONTAL_MOVEMENT_RIGHT;
//}
//
//void MovementUp(Movement & movement)
//{
//	movement._vertical = VERTICAL_MOVEMENT_UP;
//	movement._horizontal = HORIZONTAL_MOVEMENT_NONE;
//}
//
//void MovementDown(Movement & movement)
//{
//	movement._vertical = VERTICAL_MOVEMENT_DOWN;
//	movement._horizontal = HORIZONTAL_MOVEMENT_NONE;
//}
