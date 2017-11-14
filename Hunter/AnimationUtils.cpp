#include "stdafx.h"

#include "AnimationUtils.h"

namespace animation
{

	//NOTE : 이 함수에서 언제나 parentJoint는 루프 안의 i보다 작은 값을 가지고 있어야 한다...	
	void UpdateSkeleton(video::Skeleton * pSkeleton, const Matrix *pWorldMatrix)
	{
		Matrix world;
		if (pWorldMatrix)
		{
			world = *pWorldMatrix;
		}
		else
		{
			MatrixIdentity(&world);
		}
		//루트 노드는 부모가 없다...
		MatrixMultiply(&pSkeleton->_globalPoses[0], &pSkeleton->_localPoses[0], &world);

		for (uint32 i = 1; i < pSkeleton->_numhierachy; ++i)
		{
			const uint16 parentJoint = pSkeleton->_hierachy[i];
			MatrixMultiply(&pSkeleton->_globalPoses[i],
				&pSkeleton->_localPoses[i],
				&pSkeleton->_globalPoses[parentJoint]);
		}
	}
}
