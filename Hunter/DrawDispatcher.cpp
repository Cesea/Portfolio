#include "stdafx.h"
#include "DrawDispatcher.h"

void DrawDispatcher::DrawPrimitive(DrawPrimitiveCommand * pCommand)
{
	gpDevice->DrawPrimitive(D3DPT_TRIANGLELIST, pCommand->first, pCommand->numVertex / 3);
}

void DrawDispatcher::DrawIndexPrimitive(DrawIndexPrimitiveCommand *pCommand)
{

}

void DrawDispatcher::SetEffect(SetEffectCommand * pCommand)
{
}
