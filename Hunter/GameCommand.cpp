#include "stdafx.h"

void GameCommand::Reset()
{
	_type = GameCommand::Type::eNone;
	_movement._vertical = Movement::Vertical::eNoneV;
	_movement._horizontal = Movement::Horizontal::eNoneH;
	_behavior._type = Behavior::eNone;
	_behavior._usageIndex = 0;
	_interpreted = false;
}
