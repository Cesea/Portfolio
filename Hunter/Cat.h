#ifndef CAT_H
#define CAT_H

#include "Enemy.h"
#include "CatAnimationString.h"

class Cat;

struct CatCallbackData
{
	int32 *_animtionEnum;
	Vector3 *_pPosition{};
	int32 _description;
};

class CatCallbackHandler : public GameObjectAnimationCallbackHandler
{
public:
	virtual bool Init(BaseGameObject *pCat) { _pCat = (Cat *)pCat; return true; }
	HRESULT CALLBACK HandleCallback(THIS_ UINT Track, LPVOID pCallbackData);

private:
	Cat *_pCat{};
};

class Cat : public Enemy
{
	friend class CatStateMachine;
	friend class CatCallbackHandler;
private:
	enum CATSTATE
	{
		CATSTATE_IDLE,
		CATSTATE_PATROL,
		CATSTATE_FIND,
		CATSTATE_RUN,
		CATSTATE_ATK1,
		CATSTATE_ATK2,
		CATSTATE_ATK3,
		CATSTATE_ATK4,
		CATSTATE_ATK5,
		CATSTATE_HURT,
		CATSTATE_DEATH,
		CATSTATE_STAND
	};
	CATSTATE _state;
public:
	Cat();
	virtual ~Cat();

	virtual bool CreateFromWorld(World &world, const Vector3 &Pos);
	void Update(float deltaTime);
	void Handle(const CollisionSystem::ActorTriggerEvent & event);
protected:
	void SetupCallbackAndCompression();

	CatCallbackData _callbackData[6];

	GameCommand _currentCommand;

	ActionComponent *_pActionComp{};

	void QueueAction(Action &action, bool cancle = false);
public:
	bool findPlayer(Vector3 forward, Vector3 playerPos, Vector3 myPos, float range1, float range2, float findRadian);
};

#endif