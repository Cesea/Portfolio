#ifndef STATE_H
#define STATE_H


template <typename T>
class StateFoo
{
public:
	virtual void OnEnter(T *actor) = 0;
	virtual State *Update(T *actor, float deltaTime, int32 command) = 0;
	virtual void OnExit(T *actor) = 0;

protected :
	T *_pActor{};
};

#endif