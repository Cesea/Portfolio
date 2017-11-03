#include "stdafx.h"
#include "RenderComponent.h"

bool RenderComponent::PreInit(Entity * pOwner)
{
	return false;
}

bool RenderComponent::Init()
{
	return false;
}

bool RenderComponent::Release()
{
	return false;
}

void RenderComponent::Update(float deltaTime)
{
}

bool BufferRenderComponent::PreInit(Entity * pOwner)
{
	return false;
}

bool BufferRenderComponent::Init()
{
	return false;
}

bool BufferRenderComponent::Release()
{
	return false;
}

void BufferRenderComponent::Update(float deltaTime)
{
}

bool MeshRenderComponent::PreInit(Entity * pOwner)
{
	return false;
}

bool MeshRenderComponent::Init()
{
	return false;
}

bool MeshRenderComponent::Release()
{
	return false;
}

void MeshRenderComponent::Update(float deltaTime)
{
}
