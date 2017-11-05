#include "stdafx.h"
#include "DebugDraw.h"


constexpr uint32 CIRCLEGIZMO_SEGMENT = 36;

struct GizmoVertex
{
	Vector3 pos;
	DWORD color;
	enum {FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE, };
};

namespace  debugdraw
{

	void Line(const Vector3 & startPos, const Vector3 & endPos, DWORD color)
	{
		//이전 월드 행렬을 기억하자
		Matrix matPrevWorld;
		gpDevice->GetTransform(D3DTS_WORLD, &matPrevWorld);

		//사용할 월드
		D3DXMATRIXA16 matIdentity;
		D3DXMatrixIdentity(&matIdentity);
		gpDevice->SetTransform(D3DTS_WORLD, &matIdentity);

		//이전 라이트 기억
		DWORD prevLight = 0;
		gpDevice->GetRenderState(D3DRS_LIGHTING, &prevLight);

		//라이트 끄자
		gpDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

		GizmoVertex vertices[2];
		vertices[0].pos = startPos;
		vertices[0].color = color;
		vertices[1].pos = endPos;
		vertices[1].color = color;

		gpDevice->SetFVF(GizmoVertex::FVF);
		gpDevice->DrawPrimitiveUP(
			D3DPT_LINELIST, 1, vertices, sizeof(GizmoVertex));

		gpDevice->SetTransform(D3DTS_WORLD, &matPrevWorld);
		gpDevice->SetRenderState(D3DRS_LIGHTING, prevLight);
	}

	void WorldGrid(float cellsize, int cellNum)
	{
		float axisLineLength = cellsize * cellNum;

		//x 축 라인
		Vector3 xStart(-axisLineLength, 0, 0);
		Vector3 xEnd(axisLineLength, 0, 0);

		//y 축 라인
		Vector3 yStart(0, -axisLineLength, 0);
		Vector3 yEnd(0, axisLineLength, 0);

		//z 축 라인
		Vector3 zStart(0, 0, -axisLineLength);
		Vector3 zEnd(0, 0, axisLineLength);

		Line(xStart, xEnd, 0xFFFF0000);
		Line(yStart, yEnd, 0xFF00FF00);
		Line(zStart, zEnd, 0xFF0000FF);

		Vector3 arrowPos[4];
		arrowPos[0] = Vector3(-1.0f, 0.0f, -1.0f);
		arrowPos[1] = Vector3(0.0f, 0.0f, 0.0f);
		arrowPos[2] = Vector3(0.0f, 0.0f, 0.0f);
		arrowPos[3] = Vector3(1.0f, 0.0f, -1.0f);

		Vector3 arrowVertices[4];

		Matrix matTrans;
		MatrixTranslation(&matTrans, zEnd.x, zEnd.y, zEnd.z);
		Matrix matRotation;
		Matrix matFinal = matTrans;

		//z축 화살표
		for (int i = 0; i < 4; i++)
		{
			Vec3TransformCoord(&arrowVertices[i], &arrowPos[i], &matTrans);
		}

		Line(arrowVertices[0], arrowVertices[1], 0xFF0000FF);
		Line(arrowVertices[2], arrowVertices[3], 0xFF0000FF);

		//x 축 화살표

		MatrixTranslation(&matTrans, xEnd.x, xEnd.y, xEnd.z);
		MatrixRotationY(&matRotation, 90.0f* ONE_RAD);
		matFinal = matRotation * matTrans;
		for (int i = 0; i < 4; i++)
			Vec3TransformCoord(&arrowVertices[i], &arrowPos[i], &matFinal);
		//그리자
		Line(arrowVertices[0], arrowVertices[1], 0xFFFF0000);
		Line(arrowVertices[2], arrowVertices[3], 0xFFFF0000);

		//y축
		MatrixTranslation(&matTrans, yEnd.x, yEnd.y, yEnd.z);
		MatrixRotationX(&matRotation, -90.0f * ONE_RAD);
		matFinal = matRotation * matTrans;
		for (int i = 0; i < 4; i++)
			Vec3TransformCoord(&arrowVertices[i], &arrowPos[i], &matFinal);

		//그리자
		Line(arrowVertices[0], arrowVertices[1], 0xFF00FF00);
		Line(arrowVertices[2], arrowVertices[3], 0xFF00FF00);

		for (int i = -cellNum; i <= cellNum; i++)
		{
			if (i == 0)continue;

			Vector3 posStartX(i * cellsize, 0, -axisLineLength);
			Vector3 posEndX(i * cellsize, 0, axisLineLength);

			Line(posStartX, posEndX);

			Vector3 posStartZ(-axisLineLength, 0, i * cellsize);
			Vector3 posEndZ(axisLineLength, 0, i * cellsize);

			Line(posStartZ, posEndZ);
		}
	}

	//void Circle(const Vector3 & centerPos, float radius, const Vector3 & axis, DWORD color)
	//{
	//	Matrix matPrevWorld;
	//	gpDevice->GetTransform(D3DTS_WORLD, &matPrevWorld);
	//
	//	DWORD prevLight = 0;
	//	gpDevice->GetRenderState(D3DRS_LIGHTING, &prevLight);
	//
	//	gpDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	//
	//	//임시 저장 배열
	//
	//	GizmoVertex vertices[CIRCLEGIZMO_SEGMENTS + 1];
	//	for (int i = 0; i < CIRCLEGIZMO_SEGMENTS + 1; i++)
	//	{
	//		vertices[i].pos = m_pCirclePos[i];
	//		vertices[i].color = color;
	//	}
	//	//circle에 대한 회전 행렬
	//	Vector3 forward = axis;
	//	Vector3 right;
	//	Vector3 up(0, 1, 0);
	//
	//	if (forward == up)
	//	{
	//		up = Vector3(0, 0, -1);
	//	}
	//	else if (forward == Vector3(0, -1, 0))
	//	{
	//		up = Vector3(0, 0, 1);
	//	}
	//
	//	Vec3Cross(&right, &up, &forward);
	//	Vec3Cross(&up, &forward, &right);
	//
	//	//방향벡터 정규화
	//	Vec3Normalize(&forward, &forward);
	//	Vec3Normalize(&right, &right);
	//	Vec3Normalize(&up, &up);
	//
	//	//축에 대한 스케일
	//
	//	forward = forward * radius;
	//	right = right * radius;
	//	up = up * radius;
	//
	//	Matrix matWorld;
	//	MatrixIdentity(&matWorld);
	//
	//	memcpy(&matWorld._11, right, sizeof(Vector3));
	//	memcpy(&matWorld._21, up, sizeof(Vector3));
	//	memcpy(&matWorld._31, forward, sizeof(Vector3));
	//	memcpy(&matWorld._41, centerPos, sizeof(Vector3));
	//
	//	m_pDevice->SetTransform(D3DTS_WORLD, &matWorld);
	//
	//	gpDevice->SetFVF(GizmoVertex::FVF);
	//	gpDevice->DrawPrimitiveUP(
	//		D3DPT_LINESTRIP,
	//		CIRCLEGIZMO_SEGMENTS,
	//		vertices,
	//		sizeof(GizmoVertex));
	//	gpDevice->SetTransform(D3DTS_WORLD, &matPrevWorld);
	//
	//	gpDevice->SetRenderState(D3DRS_LIGHTING, prevLight);
	//}

	//void WireSphere(const Vector3 & centerPos, float radius, DWORD color)
	//{
	//	Circle(centerPos, radius, Vector3(1, 0, 0), color);
	//	Circle(centerPos, radius, Vector3(0, 1, 0), color);
	//	Circle(centerPos, radius, Vector3(0, 0, 1), color);
	//}

	void AABBBox(const Vector3 & minPos, const Vector3 & maxPos, DWORD color)
	{
		//앞면
		Line(Vector3(minPos.x, minPos.y, minPos.z), Vector3(minPos.x, maxPos.y, minPos.z), color);
		Line(Vector3(minPos.x, maxPos.y, minPos.z), Vector3(maxPos.x, maxPos.y, minPos.z), color);
		Line(Vector3(maxPos.x, maxPos.y, minPos.z), Vector3(maxPos.x, minPos.y, minPos.z), color);
		Line(Vector3(maxPos.x, minPos.y, minPos.z), Vector3(minPos.x, minPos.y, minPos.z), color);

		//뒷면
		Line(Vector3(minPos.x, minPos.y, maxPos.z), Vector3(minPos.x, maxPos.y, maxPos.z), color);
		Line(Vector3(minPos.x, maxPos.y, maxPos.z), Vector3(maxPos.x, maxPos.y, maxPos.z), color);
		Line(Vector3(maxPos.x, maxPos.y, maxPos.z), Vector3(maxPos.x, minPos.y, maxPos.z), color);
		Line(Vector3(maxPos.x, minPos.y, maxPos.z), Vector3(minPos.x, minPos.y, maxPos.z), color);
		//옆선

		Line(Vector3(minPos.x, minPos.y, minPos.z), Vector3(minPos.x, minPos.y, maxPos.z), color);
		Line(Vector3(minPos.x, maxPos.y, minPos.z), Vector3(minPos.x, maxPos.y, maxPos.z), color);
		Line(Vector3(maxPos.x, maxPos.y, minPos.z), Vector3(maxPos.x, maxPos.y, maxPos.z), color);
		Line(Vector3(maxPos.x, minPos.y, minPos.z), Vector3(maxPos.x, minPos.y, maxPos.z), color);
	}

	void RenderWorldGizmo(const Vector3 & pos)
	{
		Line(pos, Vector3(pos.x + 1, pos.y, pos.z), 0xFFFF0000);
		Line(pos, Vector3(pos.x, pos.y + 1, pos.z), 0xFF00FF00);
		Line(pos, Vector3(pos.x, pos.y, pos.z + 1), 0xFF0000FF);
	}
}
