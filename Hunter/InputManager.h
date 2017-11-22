#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

//#include "InputComponent.h"

constexpr int32 MOUSE_BUTTON_LEFT = 0;
constexpr int32 MOUSE_BUTTON_MIDDLE = 2;
constexpr int32 MOUSE_BUTTON_RIGHT = 1;

class InputManager;

class Keyboard
{
public :
	Keyboard();
	~Keyboard();

	bool Init(InputManager *pParent);


	void UpdateWithMessage(UINT msg, WPARAM wParam, LPARAM lParam);
	void Update();

	inline bool32 IsDown(int32 keyCode) const { return _currentState[keyCode]; }
	inline bool32 IsReleased(int32 keyCode) const { return (!_currentState[keyCode] && _oldState[keyCode]); }
	inline bool32 IsPressed(int32 keyCode) const { return (_currentState[keyCode] && !_oldState[keyCode]); }


	inline bool32 IsShiftDown() { return _shiftDown; }
	inline bool32 IsAltDown() { return _altDown; }
	inline bool32 IsControlDown() { return _ctrlDown; }

	inline uint8 GetCharInput() const { return _vkCode; }
	inline bool32 GetShiftDown() const { return _shiftDown; }
	inline void SetCharInput(uint8 ch) { _vkCode = ch; }

private :

	void UpdateOnKeyUp(WPARAM wParam, LPARAM lParam);
	void UpdateOnKeyDown(WPARAM wParam, LPARAM lParam);
	void UpdateOnChar(WPARAM wParam, LPARAM lParam);

	//void ProcessWindowMessage(bool32 *button, bool32 isDown);
private :
	bool _currentState[256] = {0, };
	bool _oldState[256] = {0, };

	bool32 _shiftDown{};
	bool32 _altDown{};
	bool32 _ctrlDown{};

	WPARAM _wParam{};
	uint8 _vkCode{};

	InputManager *_pParent;
};


class Mouse
{
public:
	Mouse();
	~Mouse();

	bool Init(InputManager *pParent);

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

	inline const int32 GetWheelDelta() const { return _wheelDelta; }

private:
	bool32 _currentState[3] = { 0, };
	bool32 _oldState[3] = { 0, };

	POINT _currentPoint{};
	POINT _oldPoint{};

	int32 _wheelDelta{};

	InputManager *_pParent;
};


class InputManager 
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
		MouseMoveEvent(POINT current, POINT old) :
			current(current), old(old)
		{}
		POINT current;
		POINT old;
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
		MouseWheelEvent(int32 delta, POINT coord)
			:delta(delta), coord(coord)
		{}
		int32 delta;
		POINT coord;
	};

	InputManager();
	virtual ~InputManager();

	bool Init();
	void ShutDown();

	void Update(float deltaTime);
	EventChannel GetChannel() { return _channel; }

	Keyboard keyboard;
	Mouse mouse;
private :

	EventChannel _channel;


};

#endif
