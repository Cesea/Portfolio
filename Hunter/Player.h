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

class PlayerCallbackHandler : public GameObjectAnimationCallbackHandler
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

		switch (*pData->_animtionEnum)
		{
		case PlayerAnimationEnum::eWarSwingLeft :
		{
			int a = 0;
		}break;

		}
        return S_OK;
    }

private :
	Player *_pPlayer{};
};

class Player : public BaseGameObject
{
	friend class PlayerStateMachine;
	friend class PlayerCallbackHandler;
public :
	friend class PlayerStanceState;
public :
	Player();
	virtual ~Player();

	virtual bool CreateFromWorld(World &world);
	void Update(float deltaTime);

	void Handle(const InputManager::KeyPressedEvent &event);
	void Handle(const InputManager::MousePressedEvent &event);
	void Handle(const InputManager::KeyDownEvent &event);

private :
	void SetupCallbackAndCompression();
	PlayerCallbackData _callbackData;
	PlayerStateMachine *_pStateMachine;
	GameCommand _currentCommand;
	ActionComponent *_pActionComp{};
	void QueueAction(const Action &action);

	//TerrainChunkPos _chunkPos;
	TerrainTilePos _tilePos;

private :
	EventChannel _channel;
	float _speed{20.0f};
public :

	struct PlayerPositionEvent
	{
		PlayerPositionEvent(const Vector3 &position)
			:_position(position)
		{
		}
		Vector3 _position;
	};

};

#endif