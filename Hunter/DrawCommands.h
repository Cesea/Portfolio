#ifndef DRAW_COMMANDS_H
#define DRAW_COMMANDS_H

#include "Typedefs.h"

class DrawDispatcher;

struct DrawCommand
{
	typedef void(*DispatchCommand)(const void *pData, const DrawDispatcher &dispatcher);
	DispatchCommand dispatch{ nullptr };

	DrawCommand *pNext{nullptr};
	char *extraCommandData{nullptr};
};

template <class Derived>
struct DrawCommandData
{
	static const DrawCommand::DispatchCommand dispatchFunction;
};


//Draw Vertex Buffer Command
void DispatchDrawPrimitive(const void *pData, const DrawDispatcher &dispatcher);
struct DrawPrimitiveCommand :public DrawCommandData<DrawPrimitiveCommand>
{
	uint32 numVertex;
	uint32 first;

	IDirect3DVertexBuffer9 *pVertexBuffer;
};

//Draw Indexed Primitive command
void DispatchDrawIndexPrimitive(const void *pData, const DrawDispatcher &dispatcher);
struct DrawIndexPrimitiveCommand : public DrawCommandData<DrawIndexPrimitiveCommand>
{
	uint32 numVertex;

	//TODO : Add neded things

	IDirect3DVertexBuffer9 *pVertexBuffer;
	IDirect3DIndexBuffer9 *pIndexBuffer;
};

const DrawCommand::DispatchCommand DrawIndexPrimitiveCommand::dispatchFunction = &DispatchDrawIndexPrimitive;


//Set Effect Command
void DispatchSetEffect(const void *pData, const DrawDispatcher &dispatcher);
struct SetEffectCommand : public DrawCommandData<SetEffectCommand>
{
	Handle effectHandle;
};

const DrawCommand::DispatchCommand SetEffectCommand::dispatchFunction = &DispatchSetEffect;


#endif