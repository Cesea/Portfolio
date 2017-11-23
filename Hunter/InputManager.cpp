#include "stdafx.h"
#include "InputManager.h"

InputManager::InputManager()
{
	
}

InputManager::~InputManager()
{
}

bool InputManager::Init()
{
	mouse.Init(this);
	keyboard.Init(this);
	return true;
}

void InputManager::ShutDown()
{
}

void InputManager::Update(float deltaTime)
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

bool Keyboard::Init(InputManager * pParent)
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
	_vkCode = wParam;
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
	}
}

void Keyboard::Update()
{
	_vkCode = 0;
	//_shiftDown = _currentState[VK_SHIFT] ? true : false;
	for (int32 i = 0; i < 256; ++i)
	{
		if (IsDown(i))
		{
			_pParent->GetChannel().Broadcast<InputManager::KeyDownEvent>(InputManager::KeyDownEvent(i));
		}
		if (IsReleased(i))
		{
			_pParent->GetChannel().Broadcast<InputManager::KeyReleasedEvent>(InputManager::KeyReleasedEvent(i));
		}
		if (IsPressed(i))
		{
			_pParent->GetChannel().Broadcast<InputManager::KeyPressedEvent>(InputManager::KeyPressedEvent(i));
			_vkCode = i;
		}
	}
	memcpy(_oldState, _currentState, sizeof(bool) * 256);
}

Mouse::Mouse()
{
}

Mouse::~Mouse()
{
}

bool Mouse::Init(InputManager * pParent)
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
		_pParent->_channel.Broadcast<InputManager::MouseMoveEvent>(
			InputManager::MouseMoveEvent(PointMake(_currentPoint.x, _currentPoint.y),PointMake(_oldPoint.x, _oldPoint.y)));
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
	if (gEditorOn)
	{
		if (_currentPoint.x < EDITORX || _currentPoint.x > EDITORX + EDITORSIZEX &&
			_currentPoint.y < EDITORY || _currentPoint.y > EDITORY + EDITORSIZEY)
		{
			//constexpr로 버튼이 정의 되어있다
			for (int32 i = 0; i < 3; ++i)
			{
				if (IsDown(i))
				{
					_pParent->_channel.Broadcast<InputManager::MouseDownEvent>(InputManager::MouseDownEvent(i,
						PointMake(_currentPoint.x, _currentPoint.y)));
				}
				if (IsReleased(i))
				{
					_pParent->_channel.Broadcast<InputManager::MouseReleasedEvent>(InputManager::MouseReleasedEvent(i,
						PointMake(_currentPoint.x, _currentPoint.y)));
				}
				if (IsPressed(i))
				{
					_pParent->_channel.Broadcast<InputManager::MousePressedEvent>(InputManager::MousePressedEvent(i,
						PointMake(_currentPoint.x, _currentPoint.y)));
				}
			}

			if (_wheelDelta != 0)
			{
				_pParent->_channel.Broadcast<InputManager::MouseWheelEvent>(InputManager::MouseWheelEvent(_wheelDelta,
					PointMake(_currentPoint.x, _currentPoint.y)));
			}
		}
	}
	else
	{
		//constexpr로 버튼이 정의 되어있다
		for (int32 i = 0; i < 3; ++i)
		{
			if (IsDown(i))
			{
				_pParent->_channel.Broadcast<InputManager::MouseDownEvent>(InputManager::MouseDownEvent(i,
					PointMake(_currentPoint.x, _currentPoint.y)));
			}
			if (IsReleased(i))
			{
				_pParent->_channel.Broadcast<InputManager::MouseReleasedEvent>(InputManager::MouseReleasedEvent(i,
					PointMake(_currentPoint.x, _currentPoint.y)));
			}
			if (IsPressed(i))
			{
				_pParent->_channel.Broadcast<InputManager::MousePressedEvent>(InputManager::MousePressedEvent(i,
					PointMake(_currentPoint.x, _currentPoint.y)));
			}
		}

		if (_wheelDelta != 0)
		{
			_pParent->_channel.Broadcast<InputManager::MouseWheelEvent>(InputManager::MouseWheelEvent(_wheelDelta,
				PointMake(_currentPoint.x, _currentPoint.y)));
		}
	}
	

	memcpy(_oldState, _currentState, sizeof(bool32) * 3);

	_oldPoint.x = _currentPoint.x;
	_oldPoint.y = _currentPoint.y;

	_wheelDelta = 0;
}
