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
	virtual bool Init(BaseGameObject *pSnake) { _pSnake = (Snake *)pSnake; return true; }
    HRESULT CALLBACK HandleCallback( THIS_ UINT Track, LPVOID pCallbackData )
    {
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
	enum SNAKESKINSTATE
	{
		SNAKESKINSTATE_RED,
		SNAKESKINSTATE_BLACK,
		SNAKESKINSTATE_CYAN
	};
	SNAKESTATE _state;
	SNAKESKINSTATE _skinType;
public:
	Snake();
	virtual ~Snake();

	virtual bool CreateFromWorld(World &world, const Vector3 &Pos);
	void Update(float deltaTime);
	void Handle(const CollisionSystem::ActorTriggerEvent & event);

	void render();

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