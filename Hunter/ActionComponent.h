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
	//���� �ڿ� ���ϸ��̼��� �´ٸ� �� ���ϸ��̼��� ������ ����ǰ� �Ұų� �ƴϸ� �ٷ� ������ �ǰԲ� �� �ų�??
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