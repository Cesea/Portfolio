#include "stdafx.h"
#include "InputSystem.h"

InputSystem::InputSystem()
	:keyboard(this), mouse(this)
{
	
}

InputSystem::~InputSystem()
{
}

bool InputSystem::Init(const std::string & name, const SystemSetting & setting)
{
	_name = name;
	return true;
}

void InputSystem::ShutDown()
{
}

void InputSystem::Update(float deltaTime)
{
}

Keyboard::Keyboard(InputSystem *parent)
{
	_pParent = parent;
}

Keyboard::~Keyboard()
{
}

void Keyboard::UpdateWithMessage(WPARAM wParam, LPARAM lParam)
{
	bool32 wasDown = ((lParam & (1 << 30)) != 0);
	bool32 isDown = ((lParam & (1 << 31)) == 0);

	_altDown = ((lParam & (1 << 29)));

	if (isDown != wasDown)
	{
		ProcessWindowMessage(&_currentState[wParam], isDown);
	}
	if (IsDown(wParam))
	{
		_pParent->_channel.Broadcast<InputSystem::KeyDownEvent>(InputSystem::KeyDownEvent(wParam));
	}
	if (IsReleased(wParam))
	{
		_pParent->_channel.Broadcast<InputSystem::KeyReleasedEvent>(InputSystem::KeyReleasedEvent(wParam));
	}
	if (IsPressed(wParam))
	{
		_pParent->_channel.Broadcast<InputSystem::KeyPressedEvent>(InputSystem::KeyPressedEvent(wParam));
	}
}

void Keyboard::Update()
{
	memcpy(_oldState, _currentState, sizeof(bool32) * 256);
}

void Keyboard::ProcessWindowMessage(bool32 * button, bool32 isDown)
{
	if (*button != isDown)
	{
		*button = isDown;
	}
}

Mouse::Mouse(InputSystem *parent)
{
	_pParent = parent;
}

Mouse::~Mouse()
{
}

void Mouse::UpdateWithMessage(WPARAM wParam, LPARAM lParam)
{
	bool32 lButtonDown = ((wParam & 0x0001));
	bool32 rButtonDown = ((wParam & 0x0002));
	bool32 mButtonDown = ((wParam & 0x0010));

	_currentState[0] = lButtonDown;
	_currentState[1] = rButtonDown;
	_currentState[2] = mButtonDown;

	_currentPoint.x = LOWORD(lParam);
	_currentPoint.y = HIWORD(lParam);

	if (_currentPoint.x != _oldPoint.x || _currentPoint.y != _oldPoint.y)
	{
		_pParent->_channel.Broadcast<InputSystem::MouseMoveEvent>(InputSystem::MouseMoveEvent(PointMake(_currentPoint.x, _currentPoint.y)));
	}

	//constexpr�� ��ư�� ���� �Ǿ��ִ�
	for (int32 i = 0; i < 3; ++i)
	{
		if (IsDown(i))
		{
			_pParent->_channel.Broadcast<InputSystem::MouseDownEvent>(InputSystem::MouseDownEvent(i,
				PointMake(_currentPoint.x, _currentPoint.y)));
		}
		if (IsReleased(i))
		{
			_pParent->_channel.Broadcast<InputSystem::MouseReleasedEvent>(InputSystem::MouseReleasedEvent(i,
				PointMake(_currentPoint.x, _currentPoint.y)));
		}
		if (IsPressed(i))
		{
			_pParent->_channel.Broadcast<InputSystem::MousePressedEvent>(InputSystem::MousePressedEvent(i, 
				PointMake(_currentPoint.x, _currentPoint.y)));
		}
	}

}

void Mouse::UpdateWheelWithMessage(WPARAM wParam, LPARAM lParam)
{
	_wheelDelta = GET_WHEEL_DELTA_WPARAM(wParam);
	_currentPoint.x = LOWORD(lParam);
	_currentPoint.y = HIWORD(lParam);
}

void Mouse::Update()
{
	memcpy(_oldState, _currentState, sizeof(bool32) * 3);

	_oldPoint.x = _currentPoint.x;
	_oldPoint.y = _currentPoint.y;
}
