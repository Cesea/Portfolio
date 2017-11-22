#ifndef DEBUG_DRAWER_H
#define DEBUG_DRAWER_H

#include "SingletonBase.h"

#define DEBUG_BUFFER_MAX_SIZE (2048)

#define DEBUG_DRAWER_CIRCLE_SEGMENT (36)

class DebugDrawer : public SingletonBase<DebugDrawer>
{
public:
	DebugDrawer();
	virtual ~DebugDrawer();

	bool Init();
	void Shutdown();

	inline void DrawLine(const Vector3 &start, const Vector3 &end, uint32 color);
	inline void DrawLine(const Vector3 &start, const Vector3 &end, uint32 colorStart, uint32 colorEnd);

	void DrawWorldGrid(float cellSize, int32 cellCount);

	void DrawAABB(const Vector3 &minPos, const Vector3 &maxPos, uint32 color);
	void DrawBox(const Vector3 &a, const Vector3 &b, const Vector3 &c, const Vector3 &d,
		const Vector3 &e, const Vector3 &f, const Vector3 &g, const Vector3 &h, uint32 color);
	void DrawTriangle(const Vector3	&a, const Vector3 &b, const Vector3 &c, uint32 color);

	//void DrawSphere(const Vector3 &center, float radius, uint32 color);
	//void DrawCircle(const Vector3 &center, const Vector3 &axis, float radius, uint32 color);

	void FillRenderCommand(video::RenderView &renderView);

private:
	video::DebugBuffer *_debugBuffer{};

	Vector3 _circlePoses[DEBUG_DRAWER_CIRCLE_SEGMENT + 1];

};


#define DEBUG_DRAWER (DebugDrawer::GetInstance())


#endif