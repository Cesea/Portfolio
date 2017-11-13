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

	//�ε� ��ο��� ���ϸ� �����ϰ� ��θ� �޴´�.
	std::string path;

	std::string nameCopy = fileName;
	path = GetFilePath(nameCopy);

	std::string texFilePath;	//���� ���...
	std::string texExp;			//���� Ȯ���� ��
	std::string texFile;		//���� ��	

	LPD3DXMATERIAL pMaterials = (LPD3DXMATERIAL)_pMaterial->GetBufferPointer();

	for (uint32 i = 0; i < this->_numMaterial; i++) {

		D3DMATERIAL9 mtrl = pMaterials[i].MatD3D;

		//Diffuse ������ �����ϰ� ����..
		mtrl.Ambient = mtrl.Diffuse;

		//���͸��� ���� Ǫ��
		this->_vecMaterials.push_back(mtrl);

		//���͸����� Texture ������ �ִٸ�..
		if (pMaterials[i].pTextureFilename != NULL) {

			//tex ���ϰ�δ� Mesh ���ϰ�� + texture �����̸�
			texFilePath = path + pMaterials[i].pTextureFilename;

			_texturePaths.push_back(texFilePath);
		}
	}
	SAFE_RELEASE(_pMaterial);

	//�޽� ����ȭ ( ���� ���۸� �̿��Ͽ� �޽��� ����ȭ �Ѵ� )
	_pMesh->OptimizeInplace(
		D3DXMESHOPT_ATTRSORT |		//�޽� ����¼������ ���� ( DrawSubset ȿ���� ���δ� )
		D3DXMESHOPT_COMPACT |		//�޽����� ������ �ʴ� ������ �ε��� ����
		D3DXMESHOPT_VERTEXCACHE,	//���� Cache ��Ʈ�� ������.
		(DWORD*)_pAdjacency->GetBufferPointer(),		//�������� �Ѱܾ� �Ѵ�.
		nullptr,			//����ȭ�� ������ ����� ��� �������� ( ���̻� �ʿ������ NULL )
		nullptr,			//����ȭ�� �ε��� ����
		nullptr			//����ȭ�� ���� ���� ���� 
	);

	//attribute table�� Optimization�� �Ҷ� D3DXMESHOPT_ATTSORT�� �����μ� �����ȴ�
	DWORD attributeTableSize{};
	_pMesh->GetAttributeTable(nullptr, &attributeTableSize);
	_attributeRange = new D3DXATTRIBUTERANGE[attributeTableSize];
	_pMesh->GetAttributeTable(_attributeRange, &attributeTableSize);

	//
	// �޽� ���� ó��
	//
	//��������� �޾Ҵٸ�..
	if (matCorrection) 
	{
		this->MeshCorrection(matCorrection);
	}
	else 
	{
		//��������� ������ ����ó�����ؾ� Bound ������ ������ �ִ�.
		Matrix matIden;
		MatrixIdentity(&matIden);
		this->MeshCorrection(&matIden);
	}

	//���׸��󸶴� Bound������ ��´�
	BuidSubMeshBoundInfo();
}

void XMesh::Release()
{
	SAFE_RELEASE(_pAdjacency);
	SAFE_DELETE_ARRAY(_attributeRange);
	SAFE_DELETE_ARRAY(_subMeshBoundInfo);
	SAFE_RELEASE(_pMesh);
}

//������Ĵ�� �޽��� �����Ѵ�.
void XMesh::MeshCorrection(const Matrix* pMatCorrection)
{
	//pVerElement �迭���� ������ ��� ������ ���� �ȴ�.
	_pMesh->GetDeclaration(_pVerElement);

	//���� Position Offset;
	int positionOffset = -1;

	//���� Normal Offset;
	int normalOffet = -1;

	//���� Tangent Offset;
	int tangentOffet = -1;

	//���� Binormal Offset;
	int binormalOffet = -1;

	//�ϴ� �����糤��..
	for (DWORD i = 0; i < MAX_FVF_DECL_SIZE; i++) 
	{
		//�������� �����ٸ�....
		if (_pVerElement[i].Type == D3DDECLTYPE_UNUSED)
		{
			//Ż��
			break;
		}

		//������ġ ������ �����ٸ�....
		if (_pVerElement[i].Usage == D3DDECLUSAGE_POSITION)
		{
			positionOffset = _pVerElement[i].Offset;
		}

		//�����븻 ������ �����ٸ�...
		else if (_pVerElement[i].Usage == D3DDECLUSAGE_NORMAL)
		{
			normalOffet = _pVerElement[i].Offset;
		}

		//����ź��Ʈ ������ �����ٸ�...
		else if (_pVerElement[i].Usage == D3DDECLUSAGE_TANGENT)
		{
			tangentOffet = _pVerElement[i].Offset;
		}

		//�������̳븻 ������ �����ٸ�...
		else if (_pVerElement[i].Usage == D3DDECLUSAGE_BINORMAL)
		{
			binormalOffet = _pVerElement[i].Offset;
		}
	}

	//���ؽ� ����
	uint32 verNum = _pMesh->GetNumVertices();
	//�޽��� ���� FVF ����Ȥ�� Decl ������ �̿��Ͽ� �����ϳ��� ũ�⸦ �˾Ƴ���.
	//DWORD verSize = D3DXGetFVFVertexSize( pMesh->GetFVF() );
	DWORD verSize = D3DXGetDeclVertexSize(_pVerElement, 0);

	//�޽��� ���ؽ� ���۸� Lock �Ѵ�
	void* p = NULL;
	_pMesh->LockVertexBuffer(0, &p);

	//�ٿ�� MinMax ����� ���� �ʱ�ȭ......
	_meshBoundInfo._boundMin = Vector3(0, 0, 0);
	_meshBoundInfo._boundMax = Vector3(0, 0, 0);

	//���ؽ� ����Ŭ ���� �糤��....
	for (uint32 i = 0; i < verNum; i++) 
	{
		//���ؽ� ���� �ּ�
		char* pVertex = ((char*)p + (i * verSize));

		//���� ��ġ�� �ִٸ�...
		if (positionOffset != -1) 
		{
			Vector3* pos = (Vector3*)(pVertex + positionOffset);

			Vec3TransformCoord( pos, pos, pMatCorrection);

			//���� �ּ� ������
			if (_meshBoundInfo._boundMin.x > pos->x)		_meshBoundInfo._boundMin.x = pos->x;
			if (_meshBoundInfo._boundMin.y > pos->y)		_meshBoundInfo._boundMin.y = pos->y;
			if (_meshBoundInfo._boundMin.z > pos->z)		_meshBoundInfo._boundMin.z = pos->z;

			//���� �ִ� ������
			if (_meshBoundInfo._boundMax.x < pos->x)		_meshBoundInfo._boundMax.x = pos->x;
			if (_meshBoundInfo._boundMax.y < pos->y)		_meshBoundInfo._boundMax.y = pos->y;
			if (_meshBoundInfo._boundMax.z < pos->z)		_meshBoundInfo._boundMax.z = pos->z;

			//���� ��ġ Ǫ��
			_vertices.push_back(*pos);
		}

		//�븻������ �ִٸ�..
		if (normalOffet != -1) 
		{
			Vector3* nor = (Vector3*)(pVertex + normalOffet);

			Vec3TransformNormal( nor, nor, pMatCorrection);

			Vec3Normalize(nor, nor);

			//���� �븻 Ǫ��
			_normals.push_back(*nor);
		}


		//tangent ������ �ִٸ�.
		if (tangentOffet != -1) 
		{
			Vector3* tangent = (Vector3*)(pVertex + tangentOffet);

			Vec3TransformNormal( tangent, tangent, pMatCorrection);

			Vec3Normalize(tangent, tangent);

		}

		//binormal ������ �ִٸ�
		if (binormalOffet != -1) 
		{
			Vector3* binor = (Vector3*)(pVertex + binormalOffet);
			Vec3TransformNormal( binor, binor, pMatCorrection);
			Vec3Normalize(binor, binor);
		}
	}
	_pMesh->UnlockVertexBuffer();

	//Bound �߰� ���
	_meshBoundInfo._boundCenter = (_meshBoundInfo._boundMin + _meshBoundInfo._boundMax) * 0.5f;

	_meshBoundInfo._boundSize = 
		Vector3( _meshBoundInfo._boundMax.x - _meshBoundInfo._boundMin.x, 
			_meshBoundInfo._boundMax.y - _meshBoundInfo._boundMin.y, 
			_meshBoundInfo._boundMax.z - _meshBoundInfo._boundMin.z);

	_meshBoundInfo._boundHalfSize = _meshBoundInfo._boundSize * 0.5f;
	_meshBoundInfo._boundRadius = D3DXVec3Length(&(_meshBoundInfo._boundCenter - _meshBoundInfo._boundMin));

	//
	// �ε��� ���۸� ��´�
	//
	LPDIRECT3DINDEXBUFFER9 pIndexBuffer;
	_pMesh->GetIndexBuffer(&pIndexBuffer);

	//���� ����
	_triNum = _pMesh->GetNumFaces();

	//�ε��� ���ۿ� ���� ������ ��´�.
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

	//���� �ε��� ���۴� ����
	SAFE_RELEASE(pIndexBuffer);
}

void XMesh::BuidSubMeshBoundInfo()
{
	//���׸����� ������ 2�� �̻��϶��� �� �Լ��� ���� �ǵ��� �Ѵ�.
	//�Ѱ��϶��� �̹� MatCorrection�Լ��� ���Ͽ� BoundInfo�� �����ǹǷ� �� �Լ��� ����ϵ��� �Ѵ�
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
		//���� �ּ� ������
		if (pOutBoundInfo->_boundMin.x > refVertex.x)		pOutBoundInfo->_boundMin.x = refVertex.x;
		if (pOutBoundInfo->_boundMin.y > refVertex.y)		pOutBoundInfo->_boundMin.y = refVertex.y;
		if (pOutBoundInfo->_boundMin.z > refVertex.z)		pOutBoundInfo->_boundMin.z = refVertex.z;

		//���� �ִ� ������
		if (pOutBoundInfo->_boundMax.x < refVertex.x)		pOutBoundInfo->_boundMax.x = refVertex.x;
		if (pOutBoundInfo->_boundMax.y < refVertex.y)		pOutBoundInfo->_boundMax.y = refVertex.y;
		if (pOutBoundInfo->_boundMax.z < refVertex.z)		pOutBoundInfo->_boundMax.z = refVertex.z;

	}
	//Bound �߰� ���
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
