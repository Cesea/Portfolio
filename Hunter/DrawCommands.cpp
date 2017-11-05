#include "stdafx.h"
#include "DrawCommands.h"
#include "DrawDispatcher.h"

void DispatchDrawPrimitive(const void * pData, const DrawDispatcher & dispatcher)
{
	const DrawPrimitiveCommand *pConverted = static_cast<const DrawPrimitiveCommand *>(pData);
	//dispatcher.DrawVertexBuffer(pConverted);
}

void DispatchDrawIndexPrimitive(const void * pData, const DrawDispatcher & dispatcher)
{
	const DrawIndexPrimitiveCommand *pConverted = static_cast<const DrawIndexPrimitiveCommand *>(pData);
	//dispatcher.DrawIndexPrimitive(pConverted);
}


void DispatchSetEffect(const void * pData, const DrawDispatcher & dispatcher)
{
	const SetEffectCommand *pConverted = static_cast<const SetEffectCommand *>(pData);
	//dispatcher.SetEffect(pConverted);
}
