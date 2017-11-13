#include "stdafx.h"
#include "XMesh.h"

XMesh::XMesh(void)
{
}

XMesh::~XMesh(void)
{
}

HRESULT XMesh::Load(const std::string &fileName, const Matrix* matCorrection /*= NULL */)
{
	DWORD dwMatNum{};
	
	if (FAILED(D3DXLoadMeshFromX(
		fileName.c_str(),
		D3DXMESH_MANAGED,			
		gpDevice,						
		&_pAdjacency,				
		&_pMaterial,					
		NULL,					
		&dwMatNum,
		&this->_pMesh))) 
	{
		SAFE_RELEASE(_pAdjacency);
		SAFE_RELEASE(_pMaterial);
		return E_FAIL;
	}

	_numMaterial = dwMatNum;

	//로딩 경로에서 파일명만 제거하고 경로만 받는다.
	std::string path;

	std::string nameCopy = fileName;
	path = GetFilePath(nameCopy);

	std::string texFilePath;	//최종 경로...
	std::string texExp;			//파일 확장자 명
	std::string texFile;		//파일 명	

	LPD3DXMATERIAL pMaterials = (LPD3DXMATERIAL)_pMaterial->GetBufferPointer();

	for (uint32 i = 0; i < this->_numMaterial; i++) {

		D3DMATERIAL9 mtrl = pMaterials[i].MatD3D;

		//Diffuse 정보와 동일하게 하자..
		mtrl.Ambient = mtrl.Diffuse;

		//메터리얼 정보 푸쉬
		this->_vecMaterials.push_back(mtrl);

		//메터리얼의 Texture 정보가 있다면..
		if (pMaterials[i].pTextureFilename != NULL) {

			//tex 파일경로는 Mesh 파일경로 + texture 파일이름
			texFilePath = path + pMaterials[i].pTextureFilename;

			_texturePaths.push_back(texFilePath);
		}
	}
	SAFE_RELEASE(_pMaterial);

	//메쉬 최적화 ( 인접 버퍼를 이용하여 메쉬를 최적화 한다 )
	_pMesh->OptimizeInplace(
		D3DXMESHOPT_ATTRSORT |		//메쉬 서브셋순서대로 정렬 ( DrawSubset 효율을 높인다 )
		D3DXMESHOPT_COMPACT |		//메쉬에서 사용되지 않는 정점과 인덱스 해제
		D3DXMESHOPT_VERTEXCACHE,	//정점 Cache 히트율 높힌다.
		(DWORD*)_pAdjacency->GetBufferPointer(),		//인접버퍼 넘겨야 한다.
		nullptr,			//최적화를 마지고 결과로 얻는 인접정보 ( 더이상 필요없으면 NULL )
		nullptr,			//최적화된 인덱스 정보
		nullptr			//최적화된 정점 버퍼 정보 
	);

	//attribute table은 Optimization을 할때 D3DXMESHOPT_ATTSORT를 함으로서 생성된다
	DWORD attributeTableSize{};
	_pMesh->GetAttributeTable(nullptr, &attributeTableSize);
	_attributeRange = new D3DXATTRIBUTERANGE[attributeTableSize];
	_pMesh->GetAttributeTable(_attributeRange, &attributeTableSize);

	//
	// 메쉬 보정 처리
	//
	//보정행렬을 받았다면..
	if (matCorrection) 
	{
		this->MeshCorrection(matCorrection);
	}
	else 
	{
		//보정행렬이 없더라도 보정처리를해야 Bound 정보를 얻을수 있다.
		Matrix matIden;
		MatrixIdentity(&matIden);
		this->MeshCorrection(&matIden);
	}

	//머테리얼마다 Bound정보를 얻는다
	BuidSubMeshBoundInfo();
}

void XMesh::Release()
{
	SAFE_RELEASE(_pAdjacency);
	SAFE_DELETE_ARRAY(_attributeRange);
	SAFE_DELETE_ARRAY(_subMeshBoundInfo);
	SAFE_RELEASE(_pMesh);
}

//보정행렬대로 메쉬를 수정한다.
void XMesh::MeshCorrection(const Matrix* pMatCorrection)
{
	//pVerElement 배열에는 정점의 요소 정보가 들어가게 된다.
	_pMesh->GetDeclaration(_pVerElement);

	//정점 Position Offset;
	int positionOffset = -1;

	//정점 Normal Offset;
	int normalOffet = -1;

	//정점 Tangent Offset;
	int tangentOffet = -1;

	//정점 Binormal Offset;
	int binormalOffet = -1;

	//일단 돌아재낀다..
	for (DWORD i = 0; i < MAX_FVF_DECL_SIZE; i++) 
	{
		//마지막을 만났다면....
		if (_pVerElement[i].Type == D3DDECLTYPE_UNUSED)
		{
			//탈출
			break;
		}

		//정점위치 정보를 만났다면....
		if (_pVerElement[i].Usage == D3DDECLUSAGE_POSITION)
		{
			positionOffset = _pVerElement[i].Offset;
		}

		//정점노말 정보를 만났다면...
		else if (_pVerElement[i].Usage == D3DDECLUSAGE_NORMAL)
		{
			normalOffet = _pVerElement[i].Offset;
		}

		//정점탄젠트 정보를 만났다면...
		else if (_pVerElement[i].Usage == D3DDECLUSAGE_TANGENT)
		{
			tangentOffet = _pVerElement[i].Offset;
		}

		//정점바이노말 정보를 만났다면...
		else if (_pVerElement[i].Usage == D3DDECLUSAGE_BINORMAL)
		{
			binormalOffet = _pVerElement[i].Offset;
		}
	}

	//버텍스 갯수
	uint32 verNum = _pMesh->GetNumVertices();
	//메쉬의 정점 FVF 정보혹은 Decl 정보를 이용하여 정점하나당 크기를 알아내자.
	//DWORD verSize = D3DXGetFVFVertexSize( pMesh->GetFVF() );
	DWORD verSize = D3DXGetDeclVertexSize(_pVerElement, 0);

	//메쉬의 버텍스 버퍼를 Lock 한다
	void* p = NULL;
	_pMesh->LockVertexBuffer(0, &p);

	//바운드 MinMax 계산을 위한 초기화......
	_meshBoundInfo._boundMin = Vector3(0, 0, 0);
	_meshBoundInfo._boundMax = Vector3(0, 0, 0);

	//버텍스 수만클 돌아 재낀다....
	for (uint32 i = 0; i < verNum; i++) 
	{
		//버텍스 시작 주소
		char* pVertex = ((char*)p + (i * verSize));

		//정점 위치가 있다면...
		if (positionOffset != -1) 
		{
			Vector3* pos = (Vector3*)(pVertex + positionOffset);

			Vec3TransformCoord( pos, pos, pMatCorrection);

			//정점 최소 값갱신
			if (_meshBoundInfo._boundMin.x > pos->x)		_meshBoundInfo._boundMin.x = pos->x;
			if (_meshBoundInfo._boundMin.y > pos->y)		_meshBoundInfo._boundMin.y = pos->y;
			if (_meshBoundInfo._boundMin.z > pos->z)		_meshBoundInfo._boundMin.z = pos->z;

			//정점 최대 값갱신
			if (_meshBoundInfo._boundMax.x < pos->x)		_meshBoundInfo._boundMax.x = pos->x;
			if (_meshBoundInfo._boundMax.y < pos->y)		_meshBoundInfo._boundMax.y = pos->y;
			if (_meshBoundInfo._boundMax.z < pos->z)		_meshBoundInfo._boundMax.z = pos->z;

			//정점 위치 푸쉬
			_vertices.push_back(*pos);
		}

		//노말정보가 있다면..
		if (normalOffet != -1) 
		{
			Vector3* nor = (Vector3*)(pVertex + normalOffet);

			Vec3TransformNormal( nor, nor, pMatCorrection);

			Vec3Normalize(nor, nor);

			//정점 노말 푸쉬
			_normals.push_back(*nor);
		}


		//tangent 정보가 있다면.
		if (tangentOffet != -1) 
		{
			Vector3* tangent = (Vector3*)(pVertex + tangentOffet);

			Vec3TransformNormal( tangent, tangent, pMatCorrection);

			Vec3Normalize(tangent, tangent);

		}

		//binormal 정보가 있다면
		if (binormalOffet != -1) 
		{
			Vector3* binor = (Vector3*)(pVertex + binormalOffet);
			Vec3TransformNormal( binor, binor, pMatCorrection);
			Vec3Normalize(binor, binor);
		}
	}
	_pMesh->UnlockVertexBuffer();

	//Bound 추가 계산
	_meshBoundInfo._boundCenter = (_meshBoundInfo._boundMin + _meshBoundInfo._boundMax) * 0.5f;

	_meshBoundInfo._boundSize = 
		Vector3( _meshBoundInfo._boundMax.x - _meshBoundInfo._boundMin.x, 
			_meshBoundInfo._boundMax.y - _meshBoundInfo._boundMin.y, 
			_meshBoundInfo._boundMax.z - _meshBoundInfo._boundMin.z);

	_meshBoundInfo._boundHalfSize = _meshBoundInfo._boundSize * 0.5f;
	_meshBoundInfo._boundRadius = D3DXVec3Length(&(_meshBoundInfo._boundCenter - _meshBoundInfo._boundMin));

	//
	// 인덱스 버퍼를 얻는다
	//
	LPDIRECT3DINDEXBUFFER9 pIndexBuffer;
	_pMesh->GetIndexBuffer(&pIndexBuffer);

	//면의 갯수
	_triNum = _pMesh->GetNumFaces();

	//인덱스 버퍼에 대한 정보를 얻는다.
	D3DINDEXBUFFER_DESC desc;
	pIndexBuffer->GetDesc(&desc);

	if (desc.Format == D3DFMT_INDEX16)
	{
		uint16* p = NULL;
		pIndexBuffer->Lock(0, 0, (void**)&p, 0);

		for (DWORD i = 0; i < _triNum; i++)
		{
			_indices.push_back(p[i * 3 + 0]);
			_indices.push_back(p[i * 3 + 1]);
			_indices.push_back(p[i * 3 + 2]);

		}
		pIndexBuffer->Unlock();
	}
	else if (desc.Format == D3DFMT_INDEX32)
	{
		uint32* p = NULL;
		pIndexBuffer->Lock(0, 0, (void**)&p, 0);

		for (uint32 i = 0; i < _triNum; i++)
		{
			_indices.push_back(p[i * 3 + 0]);
			_indices.push_back(p[i * 3 + 1]);
			_indices.push_back(p[i * 3 + 2]);
		}

		pIndexBuffer->Unlock();
	}

	//얻어온 인덱스 버퍼는 해재
	SAFE_RELEASE(pIndexBuffer);
}

void XMesh::BuidSubMeshBoundInfo()
{
	//머테리얼의 갯수가 2개 이상일때만 이 함수가 실행 되도록 한다.
	//한개일때는 이미 MatCorrection함수를 통하여 BoundInfo가 생성되므로 그 함수를 사용하도록 한다
	if (_numMaterial > 1)
	{
		_subMeshBoundInfo = new BoundInfo[_numMaterial];
		Assert(_subMeshBoundInfo);

		for (uint32 i = 0; i < _numMaterial; ++i)
		{
			CalculateBoundingInfo(_vertices, &_subMeshBoundInfo[i],
				_attributeRange[i].VertexStart, _attributeRange[i].VertexStart + _attributeRange[i].VertexCount);
		}
	}
}

void XMesh::CalculateBoundingInfo(std::vector<Vector3>& positions, BoundInfo * pOutBoundInfo, uint32 startVertex, uint32 endVertex)
{
	memset(pOutBoundInfo, 0, sizeof(BoundInfo));
	Assert(startVertex >= 0);
	Assert(endVertex <= _vertices.size());
	for (uint32 i = startVertex; i < endVertex; ++i)
	{
		const Vector3 &refVertex = _vertices[i];
		//정점 최소 값갱신
		if (pOutBoundInfo->_boundMin.x > refVertex.x)		pOutBoundInfo->_boundMin.x = refVertex.x;
		if (pOutBoundInfo->_boundMin.y > refVertex.y)		pOutBoundInfo->_boundMin.y = refVertex.y;
		if (pOutBoundInfo->_boundMin.z > refVertex.z)		pOutBoundInfo->_boundMin.z = refVertex.z;

		//정점 최대 값갱신
		if (pOutBoundInfo->_boundMax.x < refVertex.x)		pOutBoundInfo->_boundMax.x = refVertex.x;
		if (pOutBoundInfo->_boundMax.y < refVertex.y)		pOutBoundInfo->_boundMax.y = refVertex.y;
		if (pOutBoundInfo->_boundMax.z < refVertex.z)		pOutBoundInfo->_boundMax.z = refVertex.z;

	}
	//Bound 추가 계산
	pOutBoundInfo->_boundCenter = (pOutBoundInfo->_boundMin + pOutBoundInfo->_boundMax) * 0.5f;

	pOutBoundInfo->_boundSize =
		Vector3(pOutBoundInfo->_boundMax.x - pOutBoundInfo->_boundMin.x,
			pOutBoundInfo->_boundMax.y - pOutBoundInfo->_boundMin.y,
			pOutBoundInfo->_boundMax.z - pOutBoundInfo->_boundMin.z);

	pOutBoundInfo->_boundHalfSize = pOutBoundInfo->_boundSize * 0.5f;
	pOutBoundInfo->_boundRadius = D3DXVec3Length(&(pOutBoundInfo->_boundCenter - pOutBoundInfo->_boundMin));
}

//HRESULT XMesh::CloneMeshFVF(DWORD options, DWORD fvf, HRESULT hRet);
//	LPD3DXMESH pCloneMesh = nullptr;
//
//	// Validate requirements
//	if (!_pMesh || !pMeshOut)
//	{
//		return D3DERR_INVALIDCALL;
//	}
//	  // Attempt to clone the mesh
//	hRet = _pMesh->CloneMeshFVF(options, fvf, gpDevice, &pCloneMesh);
//	if (FAILED(hRet))
//	{
//		Assert(false)//Mesh Cloning failed;
//		return E_FAIL;
//	}
//	pMeshOut = pCloneMesh;
//	pCloneMesh = nullptr;
//
//	// Success!!
//	return S_OK;
//}
