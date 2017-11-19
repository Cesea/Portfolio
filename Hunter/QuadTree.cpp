#include "stdafx.h"
#include "QuadTree.h"

int32 QuadTree::SectionResolution;

bool32 IsRayHitBoundSphere(const Ray &ray, const Vector3 &center, float radius,
	Vector3 *pOutHitPos, Vector3 *pOutHitNormal)
{
	//������ ���������� ���� ���� ���ͱ����� ���⺤��
	Vector3 dirToCenter = center - ray.origin;

	//������ ����
	float lengthSq = Vec3LengthSq(&dirToCenter);

	//�������� ����
	float r2 = radius * radius;
	float dot = Vec3Dot( &dirToCenter, &ray.direction );
	// ��Ÿ����� ������ �ϱ����� ���� �ﰢ�� ��������
	// d2 = x2 + y2;
	// d = sqrt( x2 + y2 );

	float x2 = dot * dot;
	float d2 = lengthSq;
	//d2 - x2 = y2;
	float y2 = d2 - x2;

	//������ ������ �����.
	if (y2 > r2)
	{
		return false;
	}

	//��������´ٸ� �ϴ��� ��Ʈ
	//���� ��ư��߾� HitPoint ���ִٸ�..
	if (nullptr != pOutHitPos)
	{
		//d�� raius ����
		d2 = r2;
		//d2 = y2 + x2
		//float x2 = d2 - y2;
		float x = sqrt(d2 - y2);

		//
		*pOutHitPos = ray.origin + (ray.direction * (dot - x));

		//Hit �� ��ġ�� �븻�� ��ڴٸ�..
		if (nullptr != pOutHitNormal)
		{
			*pOutHitNormal = *pOutHitPos - center;
			Vec3Normalize(pOutHitNormal, pOutHitNormal);
		}
	}

	return true;
}

QuadTree::QuadTree()
{
	//���ϵ� ������ NULL ó��
	ZeroMemory(_pChilds, sizeof(QuadTree*) * 4);

	//���� �ε����� -1
	_center = -1;
}

QuadTree::~QuadTree()
{
	//�ڽ� ���� �����
	SAFE_DELETE(_pChilds[0]);
	SAFE_DELETE(_pChilds[1]);
	SAFE_DELETE(_pChilds[2]);
	SAFE_DELETE(_pChilds[3]);
}



bool QuadTree::Init(video::TerrainVertex *pVertices, uint32 verNumEdge, int32 sectionRes)
{
	_pTerrainVertices = pVertices;

	_corners[eCornerLB] = 0;
	_corners[eCornerRB] = verNumEdge - 1;
	_corners[eCornerLT] = (verNumEdge - 1) * verNumEdge;
	_corners[eCornerRT] = verNumEdge * verNumEdge - 1;

	SectionResolution = sectionRes;

	CreateChildTree();
	return true;
}

void QuadTree::CreateChildTree()
{
	//�߽���ġ
	_centerPos = (_pTerrainVertices[_corners[0]]._pos +
		_pTerrainVertices[_corners[1]]._pos +
		_pTerrainVertices[_corners[2]]._pos +
		_pTerrainVertices[_corners[3]]._pos) * 0.25f;

	//���ڳ� �� �߿����� �Ÿ��� ������ �̴�
	Vector3 dir = _pTerrainVertices[_corners[0]]._pos - _centerPos;
	float prevLength = Vec3LengthSq(&dir);
	_radius = sqrt(prevLength);

	//�ڽ��� �ִ�.
	if ((_corners[eCornerRT] - _corners[eCornerLT]) > 1)
	{
		//���� �ε��� ���
		_center = (_corners[0] + _corners[1] + _corners[2] + _corners[3]) / 4;

		uint32 topCenter = (_corners[eCornerRT] + _corners[eCornerLT]) / 2; //��� �߾�
		uint32 leftCenter = (_corners[eCornerLT] + _corners[eCornerLB]) / 2; //�� �߾�
		uint32 rightCenter = (_corners[eCornerRT] + _corners[eCornerRB]) / 2; //�� �߾�
		uint32 bottomCenter = (_corners[eCornerRB] + _corners[eCornerLB]) / 2; //�ϴ� �߾�

		//���ϴ� �ڽ�
		_pChilds[eCornerLB] = new QuadTree;
		_pChilds[eCornerLB]->_corners[eCornerLT] = leftCenter;
		_pChilds[eCornerLB]->_corners[eCornerRT] = _center;
		_pChilds[eCornerLB]->_corners[eCornerLB] = _corners[eCornerLB];
		_pChilds[eCornerLB]->_corners[eCornerRB] = bottomCenter;
		_pChilds[eCornerLB]->_pTerrainVertices = _pTerrainVertices;
		_pChilds[eCornerLB]->CreateChildTree();

		//���ϴ� �ڽ�
		_pChilds[eCornerRB] = new QuadTree;
		_pChilds[eCornerRB]->_corners[eCornerLT] = _center;
		_pChilds[eCornerRB]->_corners[eCornerRT] = rightCenter;
		_pChilds[eCornerRB]->_corners[eCornerLB] = bottomCenter;
		_pChilds[eCornerRB]->_corners[eCornerRB] = _corners[eCornerRB];
		_pChilds[eCornerRB]->_pTerrainVertices = _pTerrainVertices;
		_pChilds[eCornerRB]->CreateChildTree();
																				//�»�� �ڽ�
		//�»��
		_pChilds[eCornerLT] = new QuadTree;
		_pChilds[eCornerLT]->_corners[eCornerLT] = _corners[eCornerLT];
		_pChilds[eCornerLT]->_corners[eCornerRT] = topCenter;
		_pChilds[eCornerLT]->_corners[eCornerLB] = leftCenter;
		_pChilds[eCornerLT]->_corners[eCornerRB] = _center;
		_pChilds[eCornerLT]->_pTerrainVertices = _pTerrainVertices;
		_pChilds[eCornerLT]->CreateChildTree();

		//���� �ڽ�
		_pChilds[eCornerRT] = new QuadTree;
		_pChilds[eCornerRT]->_corners[eCornerLT] = topCenter;
		_pChilds[eCornerRT]->_corners[eCornerRT] = _corners[eCornerRT];
		_pChilds[eCornerRT]->_corners[eCornerLB] = _center;
		_pChilds[eCornerRT]->_corners[eCornerRB] = rightCenter;
		_pChilds[eCornerRT]->_pTerrainVertices = _pTerrainVertices;
		_pChilds[eCornerRT]->CreateChildTree();
	}
}

void QuadTree::GetRayHits(const Ray &ray, std::vector<Vector3> *pOutHit)
{
	if (IsRayHitBoundSphere(ray, _centerPos, _radius, NULL, NULL))
	{
		//�ڽ��� �ֳ�?
		if ((_corners[eCornerRT] - _corners[eCornerLT]) > 1)
		{
			//�ڽ� ���
			for (int32 i = 0; i < 4; i++)
			{
				_pChilds[i]->GetRayHits(ray, pOutHit);
			}
		}
		//�ڽ��� ���� ������ ����ϋ�
		else
		{
			//lt--rt
			//|   /|
			//|  / |
			//| /  |
			//|/   |
			//lb--rb

			//��(Ÿ��)�� ������� Ray�� hit������ ����
			float dist = 0.0f;
			Vector3 lt = this->_pTerrainVertices[_corners[eCornerLT]]._pos;
			Vector3 rt = this->_pTerrainVertices[_corners[eCornerRT]]._pos;
			Vector3 lb = this->_pTerrainVertices[_corners[eCornerLB]]._pos;
			Vector3 rb = this->_pTerrainVertices[_corners[eCornerRB]]._pos;

			//�»�� ������ üũ
			if (D3DXIntersectTri( &lt, &rt, &lb, &ray.origin, &ray.direction,
				nullptr, nullptr, &dist))
			{
				Vector3 hitPos = ray.origin + ray.direction * dist;

				//Ǫ��
				pOutHit->push_back(hitPos);
				return;
			}
			//���ϴ� ������ üũ
			if (D3DXIntersectTri( &rt, &rb, &lb, &ray.origin, &ray.direction,
				nullptr, nullptr, &dist))			
			{
				//��Ʈ ����
				Vector3 hitPos = ray.origin + ray.direction * dist;
				//Ǫ��
				pOutHit->push_back(hitPos);
				return;
			}
		}
	}
}

int32 QuadTree::GenerateIndex(uint8 * pData)
{
	return GenerateIndexInternal(0, pData);
}

int32 QuadTree::GenerateIndexInternal(int32 triangles, uint8 * pData)
{
	if (IsVisible())
	{
		uint32 *p = ((uint32 *)pData) + triangles * 3;
		// ������� �ﰢ��
		*p++ = _corners[eCornerLB];
		*p++ = _corners[eCornerLT];
		*p++ = _corners[eCornerRT];
		triangles++;
		// �����ϴ� �ﰢ��
		*p++ = _corners[eCornerLB];
		*p++ = _corners[eCornerRT];
		*p++ = _corners[eCornerRB];
		triangles++;

		return triangles;
	}

	if (_pChilds[eCornerLB])
	{
		triangles = _pChilds[eCornerLB]->GenerateIndexInternal(triangles, pData);
	}
	if (_pChilds[eCornerRB])
	{
		triangles = _pChilds[eCornerLT]->GenerateIndexInternal(triangles, pData);
	}
	if (_pChilds[eCornerLT])
	{
		triangles = _pChilds[eCornerRB]->GenerateIndexInternal(triangles, pData);
	}
	if (_pChilds[eCornerRT])
	{
		triangles = _pChilds[eCornerRT]->GenerateIndexInternal(triangles, pData);
	}
	return triangles;
}

int32 QuadTree::IsInFrustum(const Frustum & frustum)
{
	bool32 cornerIn[4];

	Vector3 currentPoint = Vector3((_pTerrainVertices + _center)->_pos.x, 0.0f, (_pTerrainVertices + _center)->_pos.z);
	if (frustum.IsSphereInFrustum(currentPoint, _radius))
	{
		cornerIn[0] = frustum.IsPointIntFrustum(*(Vector3 *)(_pTerrainVertices + _corners[0]));
		cornerIn[1] = frustum.IsPointIntFrustum(*(Vector3 *)(_pTerrainVertices + _corners[1]));;
		cornerIn[2] = frustum.IsPointIntFrustum(*(Vector3 *)(_pTerrainVertices + _corners[2]));;
		cornerIn[3] = frustum.IsPointIntFrustum(*(Vector3 *)(_pTerrainVertices + _corners[3]));;

		if (cornerIn[0] + cornerIn[1] + cornerIn[2] + cornerIn[3] == 4)
		{
			return FRUSTUM_COMPLETLY_IN;
		}
		else
		{
			return FRUSTUM_PARTIALLY_IN;
		}
	}
	else
	{
		return FRUSTUM_OUT;
	}
}


//TODO Implement this
IntRect QuadTree::CalculateDrawRange(const Frustum & frustum)
{
	IntRect result;

	//int32 lb = this->_pChilds[Corner::eCornerLB]->IsInFrustum(frustum);
	//int32 rb = this->_pChilds[Corner::eCornerRB]->IsInFrustum(frustum);
	//int32 lt = this->_pChilds[Corner::eCornerLT]->IsInFrustum(frustum);
	//int32 rt = this->_pChilds[Corner::eCornerRT]->IsInFrustum(frustum);

	//if (lb == FRUSTUM_COMPLETLY_IN || lb == FRUSTUM_PARTIALLY_IN)
	//{
	//	result._left = 0;
	//}

	result._left = 0;
	result._top = 16;
	result._right = 16;
	result._bottom = 0;

	return result;
}


int32 QuadTree::MapQuadIndexTo2DIndex(int32 level, QuadTree::Corner corner)
{
	int32 grid = pow(2, level);

	return int32();
}
