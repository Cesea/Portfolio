#ifndef DRAW_DISPATCHER_H
#define DRAW_DISPATCHER_H

#include "DrawCommands.h" 

class DrawDispatcher
{
public :
	void DrawPrimitive(DrawPrimitiveCommand *pCommand);
	void DrawIndexPrimitive(DrawIndexPrimitiveCommand *pCommand);
	void SetEffect(SetEffectCommand *pCommand);
};

#endif
