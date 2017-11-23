#ifndef SNAKE_H
#define SNAKE_H

#include "Enemy.h"
#include "SnakeAnimationString.h"

class Snake;
class SnakeStateMachine;

struct SnakeCallbackData
{
	SNAKE_ANIMATION_ENUM *_animtionEnum;
};

class SnakeCallbackHandler : public GameObjectAnimationCallbackHandler
{
public :
	bool Init(Snake *pSnake) { _pSnake = pSnake; }
    HRESULT CALLBACK HandleCallback( THIS_ UINT Track, LPVOID pCallbackData )
    {
  //      SnakeCallbackData* pData = ( SnakeCallbackData* )pCallbackData;
		//if (nullptr == pData)
		//{
  //          return S_OK;
		//}

		//switch (*pData->_animtionEnum)
		//{

		//}
        return S_OK;
    }

private :
	Snake *_pSnake{};
};

class Snake : public Enemy
{
	friend class SnakeStateMachine;
	friend class SnakeCallbackHandler;
public:
	Snake();
	virtual ~Snake();

	virtual bool CreateFromWorld(World &world);
	void Update(float deltaTime);

protected:
	void SetupCallbackAndCompression();

	SnakeCallbackData _callbackData;

	SnakeStateMachine *_pStateMachine;

	GameCommand _currentCommand;

	ActionComponent *_pActionComp{};

	void QueueAction(const Action &action);
};

#endif