#ifndef NODE_H
#define NODE_H

#include "Transform.h"
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
	Transform worldTransform;
	Transform localTransform;
	std::list<Node *> _childrens;
};


#endif