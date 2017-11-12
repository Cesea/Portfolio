#ifndef MATH_UTILS_H
#define MATH_UTILS_H

#include "Vector2.h"
#include "Vector3.h"


//정점의 노말을 계산해주는 함수
void ComputeNormal( Vector3 *pNormal, // 결과로 받을 노말
	const Vector3 * pVertices, //정점의 위치
	int verticeNum, //정점의 숫자
	const uint32 * pIndices, //인덱스 배열 포인트
	int indicesNum //인덱스 숫자 
);
void ComputeTangentAngBinormal(
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