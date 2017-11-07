#include "stdafx.h"
#include "Node.h"

Node::Node()
{
	_pParent = nullptr;
	worldTransform;
	localTransform;
	_childrens.clear();
}

Node::~Node()
{
}

void Node::Release()
{
	for (auto child : _childrens)
	{
		child->Release();
	}
}

void Node::AddChild(Node * pChild)
{
	pChild->_pParent = this;
	_childrens.push_back(pChild);
}

void Node::Update()
{
	for (auto child : _childrens)
	{
		child->Update();
	}
}

void Node::Input()
{
	for (auto child : _childrens)
	{
		child->Input();
	}
}

void Node::Render()
{
	for (auto child : _childrens)
	{
		child->Render();
	}
}
