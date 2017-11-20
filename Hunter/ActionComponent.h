#ifndef ACTION_COMPONENT_H
#define ACTION_COMPONENT_H

#include "Component.h"

//struct ENUM_PlayerActions
//{
//	enum
//	{
//
//	};
//};

struct Action
{
	int32 _type;
	//만약 뒤에 에니메이션이 온다면 이 에니메이션이 끝나고 실행되게 할거냐 아니면 바로 블랜딩이 되게끔 할 거냐??
	bool32 _blocking{};

};

struct ActionQueue
{

};

struct ActionComponent : public Component
{
public :
	ActionComponent() {}
	virtual ~ActionComponent() {}

	ActionQueue _actionQueue;
};


#endif