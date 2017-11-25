#include "stdafx.h"

void ComputeNormal(Vector3 * pNormal, const Vector3 * pVertices, int verticeNum, const uint32 * pIndices, int indicesNum)
{
	ZeroMemory(pNormal, sizeof(Vector3) * verticeNum);

	DWORD triNum = indicesNum / 3;

	for (DWORD i = 0; i < triNum; i++)
	{
		//해당삼각형의 인덱스
		DWORD i0 = pIndices[(i * 3) + 0];
		DWORD i1 = pIndices[(i * 3) + 1];
		DWORD i2 = pIndices[(i * 3) + 2];

		//정점 3개
		Vector3 v0 = pVertices[i0];
		Vector3 v1 = pVertices[i1];
		Vector3 v2 = pVertices[i2];

		//모서리
		Vector3 edge1 = v1 - v0;
		Vector3 edge2 = v2 - v0;

		//cross
		Vector3 cross;
		Vec3Cross(&cross, &edge1, &edge2);

		Vector3 normal;
		Vec3Normalize(&normal, &cross);

		pNormal[i0] += normal;
		pNormal[i1] += normal;
		pNormal[i2] += normal;
	}
	for (DWORD i = 0; i < verticeNum; i++)
	{
		Vec3Normalize(&pNormal[i], &pNormal[i]);
	}
}

void ComputeTangentAndBinormal(Vector3 * outTangents, Vector3 * outBinormal, const Vector3 * pPotisions, 
	const Vector3 * pNormals, const Vector2 * pUVs, const uint32 * pIndices, uint32 NumTris, uint32 NumVertices)
{
	ZeroMemory(outTangents, sizeof(Vector3), *NumVertices);
	ZeroMemory(outBinormal, sizeof(Vector3), *NumVertices);

	//임시 Tangent Binormal 배열
	Vector3 *pTangents = new Vector3[NumVertices];
	Vector3 *pBinormals = new Vector3[NumVertices];

	ZeroMemory(pTangents, sizeof(Vector3), *NumVertices);
	ZeroMemory(pBinormals, sizeof(Vector3), *NumVertices);

	for (DWORD a = 0; a < NumTris; a++)
	{
		//edge1 = (uvEdge1.x) *Tangent + (uvEdge1.y) * Binormal;
		//edge2 = (uvEdge2.x) *Tangent + (uvEdge2.y) * Binormal;

		//해당 삼각형의 인덱스
		DWORD i0 = pIndices[a * 3 + 0];
		DWORD i1 = pIndices[a * 3 + 1];
		DWORD i2 = pIndices[a * 3 + 2];

		//해당 삼각형의 정점의 위치
		Vector3 p0 = pPotisions[i0];
		Vector3 p1 = pPotisions[i1];
		Vector3 p2 = pPotisions[i2];

		//해당 삼각형의 UV
		Vector2 uv0 = pUVs[i0];
		Vector2 uv1 = pUVs[i1];
		Vector2 uv2 = pUVs[i2];

		//각변의 Edge 
		Vector3 edge1 = p1 - p0;
		Vector3 edge2 = p2 - p0;

		//UV Edge
		Vector2 uvEdge1 = uv1 - uv0;
		Vector2 uvEdge2 = uv2 - uv0;

		//다음과 같이 치환
		//E1 = edge1;
		//E2 = edge2;
		//U1 = uvEdge1.x;
		//V1 = uvEdge1.y;
		//U2 = uvEdge2.x;
		//V2 = uvEdge2.y;
		//T = Tangent;
		//B = Binormal;

		//E1 = U1 * T + V1 * B
		//E2 = U2 * T + V2 * B

		// T = ((E1 *  V2) + (E2 * -V1)) * R;
		// B = ((E1 * -U2) + (E2 *  U1)) * R;

		float r = 1.0f / ((uvEdge1.x * uvEdge2.y) - (uvEdge1.y * uvEdge2.x));

		//Tangent
		Vector3 t = ((edge1 * uvEdge2.y) + (edge2 * -uvEdge1.y)) *r;
		Vec3Normalize(&t, &t);

		//binormal
		Vector3 b = ((edge1 * -uvEdge2.x) + (edge2 * uvEdge1.x)) *r;
		Vec3Normalize(&b, &b);

		pTangents[i0] += t;
		pTangents[i1] += t;
		pTangents[i2] += t;
		pBinormals[i0] += t;
		pBinormals[i1] += t;
		pBinormals[i2] += t;
	}
	//binormal tangent 노말에 직교
	for (int i = 0; i < NumVertices; i++)
	{
		Vector3 n = pNormals[i];
		//tangent 직교(그람 슈미트)
		Vector3 t = pTangents[i] - (Vec3Dot(&pTangents[i], &n) * n);
		Vec3Normalize(&t, &t);

		//노말과 직교화된 T와 외적하여 B
		Vector3 b;
		Vec3Cross(&b, &n, &t);
		Vec3Normalize(&b, &b);

		outTangents[i] = t;
		outBinormal[i] = b;

	}
	SAFE_DELETE_ARRAY(pTangents);
	SAFE_DELETE_ARRAY(pBinormals);
}
