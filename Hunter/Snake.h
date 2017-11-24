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

private:
	enum SNAKESTATE
	{
		SNAKESTATE_IDLE,
		SNAKESTATE_PATROL,
		SNAKESTATE_FIND,
		SNAKESTATE_RUN,
		SNAKESTATE_ATK1,
		SNAKESTATE_ATK2,
		SNAKESTATE_ATK3,
		SNAKESTATE_STAND,
		SNAKESTATE_HURT,
		SNAKESTATE_DIE,
	};
	SNAKESTATE _state;
public:
	Snake();
	virtual ~Snake();

	virtual bool CreateFromWorld(World &world);
	void Update(float deltaTime);
<<<<<<< HEAD
<<<<<<< HEAD
	void render();
=======
=======
>>>>>>> parent of 4902ea6... Gizmo추가

>>>>>>> parent of 4902ea6... Gizmo추가
protected:
	void SetupCallbackAndCompression();

	SnakeCallbackData _callbackData;

	SnakeStateMachine *_pStateMachine;

	GameCommand _currentCommand;

	ActionComponent *_pActionComp{};

	void QueueAction(const Action &action);
public:
	bool findPlayer(Vector3 forward,Vector3 playerPos, Vector3 myPos, float range1,float range2,float findRadian);
};

#endif