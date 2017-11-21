#ifndef PLAYER_H
#define PLAYER_H

#include "PlayerAnimationString.h"
#include "PlayerStates.h"

#define TRANSITION_TIME 0.125f

struct PlayerCallbackData
{
	int32 _a;
	Vector3 *_pPlayerPosition;
};

class PlayerCallbackHandler : public ID3DXAnimationCallbackHandler
{
public :
    HRESULT CALLBACK HandleCallback( THIS_ UINT Track, LPVOID pCallbackData )
    {
        PlayerCallbackData* pData = ( PlayerCallbackData* )pCallbackData;

        // this is set to NULL if we're not playing sounds
		if (nullptr == pData)
		{
            return S_OK;
		}

		Console::Log("%d\n", pData->_a);
        return S_OK;
    }
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

//class BaseObject
//{
//public :
//	BaseObject();
//	virtual ~BaseObject();
//
//	virtual void CreateFromWorld(World &world);
//	virtual void Update(float deltaTime);
//
//protected :
//};

class Player 
{
	friend class PlayerState;
	friend class PlayerNormalState;
	friend class PlayerWarState;
	friend class PlayerDeadState;
public :
	Player();
	virtual ~Player();

	void CreateFromWorld(World &world);
	void Update(float deltaTime);


	void Handle(const InputManager::KeyPressedEvent &event);
	void Handle(const InputManager::MousePressedEvent &event);
protected :

	void SetupCallbackAndCompression();

	Entity _entity;

	PlayerCallbackData _callbackData;

	PlayerStateMachine _stateMachine;

	GameCommand _lastCommand;

	ActionComponent *_pActionComp{};

private :
	void SetInputConfig();
	InputConfig _inputConfig;

};

#endif