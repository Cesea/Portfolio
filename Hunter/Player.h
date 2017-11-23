#ifndef PLAYER_H
#define PLAYER_H

#include "BaseGameObject.h"
#include "PlayerAnimationString.h"

class Player;
class PlayerStateMachine;

struct PlayerCallbackData
{
	PlayerAnimationEnum *_animtionEnum;
};

class PlayerCallbackHandler : public ID3DXAnimationCallbackHandler
{
public :
	bool Init(Player *pPlayer) { _pPlayer = pPlayer; }
    HRESULT CALLBACK HandleCallback( THIS_ UINT Track, LPVOID pCallbackData )
    {
        PlayerCallbackData* pData = ( PlayerCallbackData* )pCallbackData;
		if (nullptr == pData)
		{
            return S_OK;
		}

		switch (pData->_animtionEnum)
		{

		}
        return S_OK;
    }

private :
	Player *_pPlayer{};
};

struct InputConfig
{
	uint32 _up;
	uint32 _down;
	uint32 _left;
	uint32 _right;
	uint32 _jump;
	uint32 _sneak;
	uint32 _special;

	uint32 _attack;
	uint32 _block;
	//마우스 키로 하자..
	//uint32 _attack;
	//uint32 _block;
};

class Player : public BaseGameObject
{
	friend class PlayerStateMachine;
	friend class PlayerCallbackHandler;
public :
	struct AttackEvent
	{
		int32 _a;
	};

	struct MoveEvent
	{
		int32 _a;
	};
	struct InteractEvent
	{
		int32 _a;
	};
	struct JumpEvent
	{
		int32 _a;
	};

	struct PositionEvent
	{
		int32 _a;
	};

	struct QueueActionEvent
	{
		QueueActionEvent(const Action &action)
			:_action(action)
		{}
		const Action &_action;
	};

	friend class PlayerStanceState;
public :
	Player();
	virtual ~Player();

	virtual bool CreateFromWorld(World &world);
	void Update(float deltaTime);

	void Handle(const InputManager::KeyPressedEvent &event);
	void Handle(const InputManager::MousePressedEvent &event);
	void Handle(const InputManager::KeyDownEvent &event);

protected :

	void SetupCallbackAndCompression();

	PlayerCallbackData _callbackData;

	PlayerStateMachine *_pStateMachine;

	GameCommand _currentCommand;

	ActionComponent *_pActionComp{};

	void QueueAction(const Action &action);

	PlayerAnimationEnum _currentAnimationEnum{};

private :
	void SetInputConfig();
	InputConfig _inputConfig;
	EventChannel _channel;

	float _speed{2.0f};

	bool32 _inCombat{ false };

};

#endif