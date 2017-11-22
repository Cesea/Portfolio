#include "stdafx.h"
#include "DebugDrawer.h"

DebugDrawer::DebugDrawer()
{
}

DebugDrawer::~DebugDrawer()
{
}

bool DebugDrawer::Init()
{
	_debugBuffer = new video::DebugBuffer;
	Memory mem;
	mem._data = nullptr;
	mem._size = sizeof(video::DebugVertex) * DEBUG_BUFFER_MAX_SIZE;
	_debugBuffer->_vHandle = VIDEO->CreateVertexBuffer(&mem, VIDEO->GetVertexDecl(video::DebugVertex::_name));

	Assert(_debugBuffer->_vHandle.IsValid());

	float intervalAngle = (D3DX_PI * 2.0f) / DEBUG_DRAWER_CIRCLE_SEGMENT;

	//circle Á¤Á¡ ¼ÂÆÃ
	for (int i = 0; i <= DEBUG_DRAWER_CIRCLE_SEGMENT; i++)
	{
		float angle = i * intervalAngle;
		_circlePoses[i] = Vector3(cosf(angle) , sinf(angle), 0.0f);
	}

	return true;
}

void DebugDrawer::Shutdown()
{
	VIDEO->DestroyVertexBuffer(_debugBuffer->_vHandle);
	SAFE_DELETE(_debugBuffer);
}

void DebugDrawer::DrawLine(const Vector3 & start, const Vector3 & end, uint32 color)
{
	_debugBuffer->Add(video::DebugVertex(start, color));
	_debugBuffer->Add(video::DebugVertex(end, color));
}

void DebugDrawer::DrawLine(const Vector3 & start, const Vector3 & end, uint32 colorStart, uint32 colorEnd)
{
	_debugBuffer->Add(video::DebugVertex(start, colorStart));
	_debugBuffer->Add(video::DebugVertex(end, colorEnd));
}

void DebugDrawer::DrawWorldGrid(float cellSize, int32 cellCount)
{
	float minX = -cellSize * (cellCount / 2);
	float minZ = -cellSize * (cellCount / 2);
	float totalLength = cellSize * cellCount;

	Vector3 start;
	Vector3 end;

	for (int32 z = 0; z <= cellCount; ++z)
	{
		start.x = minX;
		start.y = 0;
		start.z = minZ + z * cellSize;

		end.x = minX + totalLength;
		end.y = 0;
		end.z = minZ + z * cellSize;
		DrawLine(start, end, 0xff000000);
	}

	for (int32 x = 0; x <= cellCount; ++x)
	{
		start.x = minX + x * cellSize;
		start.y = 0;
		start.z = minZ;

		end.x = minX + x * cellSize;
		end.y = 0;
		end.z = minZ + totalLength;
		DrawLine(start, end, 0xff000000);
	}

}

void DebugDrawer::DrawAABB(const Vector3 & minPos, const Vector3 & maxPos, uint32 color)
{
	//¾Õ¸é
	DrawLine(Vector3(minPos.x, minPos.y, minPos.z), Vector3(minPos.x, maxPos.y, minPos.z), color);
	DrawLine(Vector3(minPos.x, maxPos.y, minPos.z), Vector3(maxPos.x, maxPos.y, minPos.z), color);
	DrawLine(Vector3(maxPos.x, maxPos.y, minPos.z), Vector3(maxPos.x, minPos.y, minPos.z), color);
	DrawLine(Vector3(maxPos.x, minPos.y, minPos.z), Vector3(minPos.x, minPos.y, minPos.z), color);

	//µÞ¸é
	DrawLine(Vector3(minPos.x, minPos.y,maxPos.z), Vector3(minPos.x, maxPos.y, maxPos.z), color);
	DrawLine(Vector3(minPos.x, maxPos.y,maxPos.z), Vector3(maxPos.x, maxPos.y, maxPos.z), color);
	DrawLine(Vector3(maxPos.x, maxPos.y,maxPos.z), Vector3(maxPos.x, minPos.y, maxPos.z), color);
	DrawLine(Vector3(maxPos.x, minPos.y,maxPos.z), Vector3(minPos.x, minPos.y, maxPos.z), color);

	//¿·¼±
	DrawLine(Vector3(minPos.x, minPos.y, minPos.z), Vector3(minPos.x, minPos.y, maxPos.z), color);
	DrawLine(Vector3(minPos.x, maxPos.y, minPos.z), Vector3(minPos.x, maxPos.y, maxPos.z), color);
	DrawLine(Vector3(maxPos.x, maxPos.y, minPos.z), Vector3(maxPos.x, maxPos.y, maxPos.z), color);
	DrawLine(Vector3(maxPos.x, minPos.y, minPos.z), Vector3(maxPos.x, minPos.y, maxPos.z), color);
}



void DebugDrawer::DrawBox(const Vector3 & a, const Vector3 & b, const Vector3 & c, const Vector3 & d, 
		const Vector3 & e, const Vector3 & f, const Vector3 & g, const Vector3 & h, uint32 color)
{
	//¾Õ¸é
	DrawLine(a, b, color);
	DrawLine(b, c, color);
	DrawLine(c, d, color);
	DrawLine(d, a, color);
	//µÞ¸é
	DrawLine(e, f, color);
	DrawLine(f, g, color);
	DrawLine(g, h, color);
	DrawLine(h, e, color);
	//¿·¼±
	DrawLine(a, e, color);
	DrawLine(b, f, color);
	DrawLine(d, h, color);
	DrawLine(c, g, color);
}

void DebugDrawer::DrawTriangle(const Vector3 & a, const Vector3 & b, const Vector3 & c, uint32 color)
{
	DrawLine(a, b, color);
	DrawLine(b, c, color);
	DrawLine(c, a, color);
}

//void DebugDrawer::DrawRect(const Vector3 & left, const Vector3 & top,
//	const Vector3 & right, const Vector3 & bottom, uint32 color)
//{
//	DrawLine(left, top, color);
//	DrawLine(left, bottom, color);
//	DrawLine(left, top, color);
//	DrawLine(left, top, color);
//}
//void DebugDrawer::DrawSphere(const Vector3 & center, float radius, uint32 color)
//{
//}
//
//void DebugDrawer::DrawCircle(const Vector3 & center, const Vector3 & axis, float radius, uint32 color)
//{
//}

void DebugDrawer::FillRenderCommand(video::RenderView & renderView)
{
	_debugBuffer->Update();
	Matrix matrix;
	MatrixIdentity(&matrix);
	video::RenderCommand &command = renderView.GetCommand();
	command._vHandle = _debugBuffer->_vHandle;
	command._drawType = video::RenderCommand::DrawType::eStatic;
	command._effectHandle = video::DebugBuffer::sDefaultEffectHandle;
	command._primType = video::RenderCommand::eLineList;
	command._numPrim = _debugBuffer->_count / 2;
	command._materialHandle = VIDEO->GetMaterial("Default");
	command._cacheRange = renderView._matrixCache.Add(&matrix);

	_debugBuffer->Reset();
	}
