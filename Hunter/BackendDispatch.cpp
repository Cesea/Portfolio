#include "stdafx.h"
#include "BackendDispatch.h"

#include "Command.h"

void backenddispatch::Draw(const void * pData)
{
	const commands::Draw *pConverted = static_cast<const commands::Draw *>(pData);
	VIDEO->Draw(pConverted->vertexBuffer, pConverted->startVertex, pConverted->primitiveCount, 16);
	//backend::Draw(pConverted->primitiveCount, );
}

void backenddispatch::DrawIndexed(const void * pData)
{
	const commands::DrawIndexed *pConverted = static_cast<const commands::DrawIndexed *>(pData);
	//backend::Draw(pConverted->primitiveCount, );
}
