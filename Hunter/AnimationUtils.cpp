#include "stdafx.h"

#include "AnimationUtils.h"

namespace animation
{

	//NOTE : �� �Լ����� ������ parentJoint�� ���� ���� i���� ���� ���� ������ �־�� �Ѵ�...	
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
		//��Ʈ ���� �θ� ����...
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
