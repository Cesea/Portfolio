#ifndef INPUT_SYSTEM_H
#define INPUT_SYSTEM_H

#include "System.h"

//#include "InputComponent.h"

constexpr int32 MOUSE_BUTTON_LEFT = 0;
constexpr int32 MOUSE_BUTTON_MIDDLE = 1;
constexpr int32 MOUSE_BUTTON_RIGHT = 2;

class InputSystem;

class Keyboard
{
public :
	Keyboard(InputSystem *pParent);
	~Keyboard();

	void UpdateWithMessage(WPARAM wParam, LPARAM lParam);

	void Update();

	inline bool32 IsDown(int32 keyCode) const { return _currentState[keyCode]; }
	inline bool32 IsReleased(int32 keyCode) const { return (!_currentState[keyCode] && _oldState[keyCode]); }
	inline bool32 IsPressed(int32 keyCode) const { return (_currentState[keyCode] && !_oldState[keyCode]); }


	inline bool32 IsShiftDown() { return _shiftDown; }
	inline bool32 IsAltDown() { return _altDown; }
	inline bool32 IsControlDown() { return _ctrlDown; }

private :

	void ProcessWindowMessage(bool32 *button, bool32 isDown);

private :
	bool32 _currentState[256] = {0, };
	bool32 _oldState[256] = {0, };

	bool32 _shiftDown{};
	bool32 _altDown{};
	bool32 _ctrlDown{};

	uint8 _charInput{};

	InputSystem *_pParent;
};


class Mouse
{
public:
	Mouse(InputSystem *pParent);
	~Mouse();

	void UpdateWithMessage(WPARAM wParam, LPARAM lParam);
	void UpdateWheelWithMessage(WPARAM wParam, LPARAM lParam);

	void Update();

	inline bool32 IsDown(int32 button) const { return _currentState[button]; }
	inline bool32 IsReleased(int32 button) const { return (!_currentState[button] && _oldState[button]); }
	inline bool32 IsPressed(int32 button) const { return (_currentState[button] && !_oldState[button]); }

	inline const POINT &GetCurrentPoint() const { return _currentPoint; }
	inline const POINT &GetOldPoint() const { return _oldPoint; }
	inline const POINT &GetMouseDelta() const
	{
		POINT result;
		result.x = _currentPoint.x - _oldPoint.x;
		result.y = _currentPoint.y - _oldPoint.y;
		return result;
	}

private:
	bool32 _currentState[3] = { 0, };
	bool32 _oldState[3] = { 0, };

	POINT _currentPoint{};
	POINT _oldPoint{};

	int32 _wheelDelta{};

	InputSystem *_pParent;
};


class InputSystem : public System
{
	friend class Keyboard;
	friend class Mouse;
public :
	//Key에서의 code는 vkcode를 의미한다
	struct KeyDownEvent
	{
		KeyDownEvent(uint32 code) :code(code) {}
		uint32 code;
	};

	struct KeyPressedEvent
	{
		KeyPressedEvent(uint32 code) : code(code) {}
		uint32 code;
	};

	struct KeyReleasedEvent
	{
		KeyReleasedEvent(uint32 code) : code(code) {}
		uint32 code;
	};

	//Mouse에서의 code는 위의 정의에 따른다
	struct MouseMoveEvent
	{
		MouseMoveEvent(POINT coord) :
			coord(coord)
		{}
		POINT coord;
	};
	struct MouseDownEvent
	{
		MouseDownEvent(uint32 code, POINT coord) :
			code(code), coord(coord)
		{}
		uint32 code;
		POINT coord;
	};
	struct MousePressedEvent
	{
		MousePressedEvent(uint32 code, POINT coord) 
			: code(code),  coord(coord)
		{}
		uint32 code;
		POINT coord;
	};
	struct MouseReleasedEvent
	{
		MouseReleasedEvent(uint32 code, POINT coord) :
			code(code), coord(coord)
		{}
		uint32 code;
		POINT coord;
	};
	struct MouseWheelEvent
	{
		MouseWheelEvent(uint32 code, uint32 syskey, POINT coord)
			:code(code), coord(coord)
		{}
		uint32 code;
		POINT coord;
	};

	InputSystem();
	virtual ~InputSystem();

	virtual bool Init(const std::string &name, const SystemSetting &setting);
	virtual void ShutDown();

	virtual void Update(float deltaTime);
	virtual EventChannel GetChannel() { return _channel; }

	Keyboard keyboard;
	Mouse mouse;
private :


};

#endif
