#ifndef MATH_UTILS_H
#define MATH_UTILS_H

#include "Vector2.h"
#include "Vector3.h"

constexpr float PI_DIV_2 = D3DX_PI * 0.5f;
constexpr float PI_DIV_4 = D3DX_PI * 0.25f;

//������ �븻�� ������ִ� �Լ�
void ComputeNormal( Vector3 *pNormal, // ����� ���� �븻
	const Vector3 * pVertices, //������ ��ġ
	int verticeNum, //������ ����
	const uint32 * pIndices, //�ε��� �迭 ����Ʈ
	int indicesNum //�ε��� ���� 
);
void ComputeTangentAndBinormal(
	Vector3* outTangents,
	Vector3* outBinormal,
	const Vector3 *pPotisions,
	const Vector3 *pNormals,
	const Vector2 *pUVs,
	const uint32 *pIndices,
	uint32 NumTris,
	uint32 NumVertices
);


#endif