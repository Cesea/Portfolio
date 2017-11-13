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

	_materialNum = dwMatNum;

	//�ε� ��ο��� ���ϸ� �����ϰ� ��θ� �޴´�.
	std::string path;

	std::string nameCopy = fileName;
	path = GetFilePath(nameCopy);

	std::string texFilePath;	//���� ���...
	std::string texExp;			//���� Ȯ���� ��
	std::string texFile;		//���� ��	

	LPD3DXMATERIAL pMaterials = (LPD3DXMATERIAL)_pMaterial->GetBufferPointer();

	for (uint32 i = 0; i < this->_materialNum; i++) {

		D3DMATERIAL9 mtrl = pMaterials[i].MatD3D;

		//Diffuse ������ �����ϰ� ����..
		mtrl.Ambient = mtrl.Diffuse;

		//���͸��� ���� Ǫ��
		this->vecMaterials.push_back(mtrl);

		//���͸����� Texture ������ �ִٸ�..
		if (pMaterials[i].pTextureFilename != NULL) {

			//tex ���ϰ�δ� Mesh ���ϰ�� + texture �����̸�
			texFilePath = path + pMaterials[i].pTextureFilename;

			texturePaths.push_back(texFilePath);

			//Texture �ε��ϰ� Ǫ��
			//vecDiffuseTex.push_back(RESOURCE_TEXTURE->GetResource(texFilePath));

			//���� �̸��� Ȯ���� ��ġ
			//int dotIndex = texFilePath.find_last_of(".");

			//���� ��� Ȯ���ڸ� ������.
			//texFile = texFilePath.substr(0, dotIndex);
			//texExp = texFilePath.substr(dotIndex + 1, filePath.length());

			////
			//// �븻�� ���� _N �� �ٴ´�.
			////
			//texFilePath = texFile + "_N." + texExp;
			//LPDIRECT3DTEXTURE9 pNorTex = RESOURCE_TEXTURE->GetResource(texFilePath);
			////���ٸ�...
			//	this->vecNormalTex.push_back(pNorTex);

			////
			//// ����ŧ�� ���� _S �� �ٴ´�.
			////
			//texFilePath = texFile + "_S." + texExp;
			//LPDIRECT3DTEXTURE9 pSpecTex = RESOURCE_TEXTURE->GetResource(texFilePath);
			////���ٸ�...
			//	this->vecSpecularTex.push_back(pSpecTex);


			////
			//// �̹̼� ���� _E �� �ٴ´�.
			////
			//texFilePath = texFile + "_E." + texExp;
			//LPDIRECT3DTEXTURE9 pEmiTex = RESOURCE_TEXTURE->GetResource(texFilePath);
			////���ٸ�...
			//	this->vecEmissionTex.push_back(pEmiTex);
		}
	}
	//���� ���� ������ �� ����Ͽ� ������ Texture �� �ε� �߱⶧���� 
	//���̻� pMaterial ���� ���۴� �ʿ� ���� �� �޸� ���縦 ���־�� �Ѵ�.
	SAFE_RELEASE(_pMaterial);

	//if (desiredFVF != 0)
	//{
	//	// ���� Mesh�� FVF�� ���ڷ� �ѱ� desiredFVF�� �ٸ��ٸ� ó���� �� �־�� �Ѵ�
	//	DWORD originalFVF = _pMesh->GetFVF();
	//	ID3DXMesh *cloneMesh = nullptr;
	//	if (originalFVF != desiredFVF)
	//	{
	//		CloneMeshFVF(D3DXMESH_MANAGED, desiredFVF, cloneMesh);
	//	}
	//	if (cloneMesh)
	//	{
	//		COM_RELEASE(_pMesh);
	//		_pMesh = cloneMesh;
	//	}
	//}

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
}

void XMesh::Release()
{
	SAFE_RELEASE(_pAdjacency);
	SAFE_DELETE_ARRAY(_attributeRange);
	SAFE_RELEASE(_pMesh);
}

void XMesh::Render()
{
	//������� ����
	//D3DXMATRIXA16 matWorld = Trans->GetFinalMatrix();
	//sStaticMeshEffect->SetMatrix("matWorld", &matWorld);

	////���� ���� �ϴ�...
	//D3DXVECTOR4 lightDir(1, -1, 1, 1);
	//sStaticMeshEffect->SetVector("vLightDir", &lightDir);

	////����Ʈ �÷� �ϴ� ���
	//D3DXVECTOR4 lightColor(1, 1, 1, 1);
	//sStaticMeshEffect->SetVector("vLightColor", &lightColor);


	//Effect �� �׸��� ����

	////UINT passNum;
	////sStaticMeshEffect->Begin(&passNum, 0);

	//for (UINT i = 0; i < passNum; i++)
	//{
	//	//sStaticMeshEffect->BeginPass(i);

	//	//����¼���ŭ ���� �糤��...
	//	for (int m = 0; m < this->dwMaterialsNum; m++) {

	//		//�ؽ��� ����
	//		sStaticMeshEffect->SetTexture("Diffuse_Tex", this->vecDiffuseTex[m]);
	//		sStaticMeshEffect->SetTexture("Normal_Tex", this->vecNormalTex[m]);
	//		sStaticMeshEffect->SetTexture("Specular_Tex", this->vecSpecularTex[m]);
	//		sStaticMeshEffect->SetTexture("Emission_Tex", this->vecEmissionTex[m]);
	//		//�����Ŀ�
	//		sStaticMeshEffect->SetFloat("fSpecPower", this->vecMaterials[m].Power);

	//		//Begin �� ������ ���� ���� �ٲ�� ������ ���� ����
	//		sStaticMeshEffect->CommitChanges();

	//		this->pMesh->DrawSubset(m);
	//	}

	//	sStaticMeshEffect->EndPass();
	//}

	//sStaticMeshEffect->End();

}



////////////////////////////////////////////////////////////////////////

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
	for (DWORD i = 0; i < MAX_FVF_DECL_SIZE; i++) {

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
	this->_boundMin = Vector3(0, 0, 0);
	this->_boundMax = Vector3(0, 0, 0);

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
			if (_boundMin.x > pos->x)		_boundMin.x = pos->x;
			if (_boundMin.y > pos->y)		_boundMin.y = pos->y;
			if (_boundMin.z > pos->z)		_boundMin.z = pos->z;

			//���� �ִ� ������
			if (_boundMax.x < pos->x)		_boundMax.x = pos->x;
			if (_boundMax.y < pos->y)		_boundMax.y = pos->y;
			if (_boundMax.z < pos->z)		_boundMax.z = pos->z;

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
	this->_boundCenter = (this->_boundMin + this->_boundMax) * 0.5f;
	this->_boundSize = Vector3( _boundMax.x - _boundMin.x, _boundMax.y - _boundMin.y, _boundMax.z - _boundMin.z);
	this->_boundHalfSize = this->_boundSize * 0.5f;
	this->_boundRadius = D3DXVec3Length(&(this->_boundCenter - this->_boundMin));


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
