#include "stdafx.h"

void GameCommand::Reset()
{
	_type = GAMECOMMAND_NONE;
	_movement._vertical = VERTICAL_MOVEMENT_NONE;
	_movement._horizontal = HORIZONTAL_MOVEMENT_NONE;
	_behavior._type = BEHAVIOR_NONE;
	_behavior._usageIndex = 0;
	_interpreted = false;
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
	_interpreted = other._interpreted;
}

GameCommand & GameCommand::operator=(const GameCommand & other)
{
	_type = other._type;
	_movement._vertical = other._movement._vertical;
	_movement._horizontal = other._movement._horizontal;
	_behavior._type = other._behavior._type;
	_behavior._usageIndex = other._behavior._usageIndex;
	_interpreted = other._interpreted;

	return *this;
}
