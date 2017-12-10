#ifndef SNAKE_H
#define SNAKE_H

#include "Enemy.h"
#include "SnakeAnimationString.h"

class Snake;
class SnakeStateMachine;

struct SnakeCallbackData
{
	int32 *_animtionEnum;
	Vector3 *_pPosition{};
	int32 _description;
};

class SnakeCallbackHandler : public GameObjectAnimationCallbackHandler
{
public :
	virtual bool Init(BaseGameObject *pSnake) { _pSnake = (Snake *)pSnake; return true; }
	HRESULT CALLBACK HandleCallback(THIS_ UINT Track, LPVOID pCallbackData);

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
		SNAKESTATE_DELAY,
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

	int _delayTime2;
	int _delayCount2;
public:
	Snake();
	virtual ~Snake();

	virtual bool CreateFromWorld(World &world, const Vector3 &Pos);
	void Update(float deltaTime);
	void Handle(const CollisionSystem::ActorTriggerEvent & event);

	void render();

protected:
	void SetupCallbackAndCompression();

	SnakeCallbackData _callbackData[5];

	GameCommand _currentCommand;

	ActionComponent *_pActionComp{};

	void QueueAction(Action &action, bool cancle = false);
public:
	bool findPlayer(Vector3 forward,Vector3 playerPos, Vector3 myPos, float range1,float range2,float findRadian);
};

#endif