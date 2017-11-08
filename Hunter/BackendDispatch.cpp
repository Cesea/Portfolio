#include "stdafx.h"
#include "BackendDispatch.h"

#include "Command.h"

void backenddispatch::Draw(const void * pData)
{
	const commands::Draw *pConverted = static_cast<const commands::Draw *>(pData);
	VIDEO->Draw(pConverted->vertexBufferHandle, pConverted->startVertex, pConverted->primitiveCount, pConverted->worldMatrix);
	//backend::Draw(pConverted->primitiveCount, );
}

void backenddispatch::DrawIndexed(const void * pData)
{
	const commands::DrawIndexed *pConverted = static_cast<const commands::DrawIndexed *>(pData);
	VIDEO->DrawIndexed(pConverted->vertexBufferHandle, pConverted->indexBufferHandle,
		pConverted->numVertex, pConverted->startIndex, pConverted->numVertex, pConverted->worldMatrix);
	//backend::Draw(pConverted->primitiveCount, );
}

void backenddispatch::SetEffect(const void * pCommand)
{
}

void backenddispatch::SetSurfaceMaterial(const void * pCommand)
{
}
