#ifndef NODE_H
#define NODE_H

#include "TransformComponent.h"
#include <list>

class Node
{
public :
	Node();
	~Node();

	virtual void Release();

	void AddChild(Node *pChild);
	virtual void Update();
	virtual void Input();
	virtual void Render();

private :
	Node *_pParent;
	TransformComponent worldTransform;
	TransformComponent localTransform;
	std::list<Node *> _childrens;
};


#endif