#ifndef DEBUG_DRAW_H
#define DEBUG_DRAW_H

namespace debugdraw
{

	void Line(const Vector3 &startPos, const Vector3 &endPos, DWORD color = 0xFF808080);

	void WorldGrid(float cellsize, int cellNum);

	//void Circle(const Vector3 &centerPos, float radius, const Vector3 &axis = Vector3(0, 0, 1),
	//	DWORD color = 0xFF808080);
	//
	//void WireSphere(const Vector3 &centerPos, float radius, DWORD color = 0xFF808080);

	void AABBBox(const Vector3 &minPos, const Vector3 &maxPos, DWORD color = 0xFF808080);

	void RenderWorldGizmo(const Vector3 &pos);
}

#endif