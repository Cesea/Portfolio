#include "stdafx.h"
#include "InputSystem.h"

InputSystem::InputSystem()
{
	
}

InputSystem::~InputSystem()
{
}

bool InputSystem::Init(const std::string & name, const SystemSetting & setting)
{
	_name = name;
	mouse.Init(this);
	keyboard.Init(this);
	return true;
}

void InputSystem::ShutDown()
{
}

void InputSystem::Update(float deltaTime)
{
	keyboard.Update();
	mouse.Update();
}

Keyboard::Keyboard()
{
}

Keyboard::~Keyboard()
{
}

bool Keyboard::Init(InputSystem * pParent)
{
	_pParent = pParent;
	return true;
}

void Keyboard::UpdateOnKeyUp(WPARAM wParam, LPARAM lParam)
{
	_currentState[wParam] = false;
}

void Keyboard::UpdateOnKeyDown(WPARAM wParam, LPARAM lParam)
{
	_currentState[wParam] = true;

	UINT tempChar = MapVirtualKey(wParam, MAPVK_VK_TO_CHAR);
	//if (IsLetter(tempChar))
	//{
	//	_charInput = (_shiftDown) ? tempChar : tempChar + 32;
	//}
}

void Keyboard::UpdateOnChar(WPARAM wParam, LPARAM lParam)
{
	_charInput = wParam;
}

void Keyboard::UpdateWithMessage(UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_SYSKEYDOWN :
	case WM_KEYDOWN :
	{
		this->UpdateOnKeyDown(wParam, lParam);
	}break;
	case WM_SYSKEYUP:
	case WM_KEYUP :
	{
		this->UpdateOnKeyUp(wParam, lParam);
	}break;
	case WM_CHAR :
	{
		this->UpdateOnChar(wParam, lParam);
	}break;
	}
}

void Keyboard::Update()
{
	_charInput = 0;
	for (int32 i = 0; i < 256; ++i)
	{
		if (IsDown(i))
		{
			_pParent->GetChannel().Broadcast<InputSystem::KeyDownEvent>(InputSystem::KeyDownEvent(i));
		}
		if (IsReleased(i))
		{
			_pParent->GetChannel().Broadcast<InputSystem::KeyReleasedEvent>(InputSystem::KeyReleasedEvent(i));
		}
		if (IsPressed(i))
		{
			_pParent->GetChannel().Broadcast<InputSystem::KeyPressedEvent>(InputSystem::KeyPressedEvent(i));
		}
	}
	memcpy(_oldState, _currentState, sizeof(bool) * 256);
}

//void Keyboard::ProcessWindowMessage(bool32 * button, bool32 isDown)
//{
//	if (*button != isDown)
//	{
//		*button = isDown;
//	}
//}

Mouse::Mouse()
{
}

Mouse::~Mouse()
{
}

bool Mouse::Init(InputSystem * pParent)
{
	_pParent = pParent;
	return true;
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

	//constexpr로 버튼이 정의 되어있다
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
