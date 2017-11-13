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

	//로딩 경로에서 파일명만 제거하고 경로만 받는다.
	std::string path;

	std::string nameCopy = fileName;
	path = GetFilePath(nameCopy);

	std::string texFilePath;	//최종 경로...
	std::string texExp;			//파일 확장자 명
	std::string texFile;		//파일 명	

	LPD3DXMATERIAL pMaterials = (LPD3DXMATERIAL)_pMaterial->GetBufferPointer();

	for (uint32 i = 0; i < this->_materialNum; i++) {

		D3DMATERIAL9 mtrl = pMaterials[i].MatD3D;

		//Diffuse 정보와 동일하게 하자..
		mtrl.Ambient = mtrl.Diffuse;

		//메터리얼 정보 푸쉬
		this->vecMaterials.push_back(mtrl);

		//메터리얼의 Texture 정보가 있다면..
		if (pMaterials[i].pTextureFilename != NULL) {

			//tex 파일경로는 Mesh 파일경로 + texture 파일이름
			texFilePath = path + pMaterials[i].pTextureFilename;

			texturePaths.push_back(texFilePath);

			//Texture 로딩하고 푸쉬
			//vecDiffuseTex.push_back(RESOURCE_TEXTURE->GetResource(texFilePath));

			//파일 이름과 확장자 위치
			//int dotIndex = texFilePath.find_last_of(".");

			//파일 명과 확장자를 나눈다.
			//texFile = texFilePath.substr(0, dotIndex);
			//texExp = texFilePath.substr(dotIndex + 1, filePath.length());

			////
			//// 노말말 맵은 _N 이 붙는다.
			////
			//texFilePath = texFile + "_N." + texExp;
			//LPDIRECT3DTEXTURE9 pNorTex = RESOURCE_TEXTURE->GetResource(texFilePath);
			////없다면...
			//	this->vecNormalTex.push_back(pNorTex);

			////
			//// 스펙큘러 맵은 _S 이 붙는다.
			////
			//texFilePath = texFile + "_S." + texExp;
			//LPDIRECT3DTEXTURE9 pSpecTex = RESOURCE_TEXTURE->GetResource(texFilePath);
			////없다면...
			//	this->vecSpecularTex.push_back(pSpecTex);


			////
			//// 이미션 맵은 _E 이 붙는다.
			////
			//texFilePath = texFile + "_E." + texExp;
			//LPDIRECT3DTEXTURE9 pEmiTex = RESOURCE_TEXTURE->GetResource(texFilePath);
			////없다면...
			//	this->vecEmissionTex.push_back(pEmiTex);
		}
	}
	//얻어온 재질 정보를 다 사용하여 재질과 Texture 를 로딩 했기때문에 
	//더이상 pMaterial 재질 버퍼는 필요 없어 꼭 메모리 해재를 해주어야 한다.
	SAFE_RELEASE(_pMaterial);

	//if (desiredFVF != 0)
	//{
	//	// 만약 Mesh의 FVF가 인자로 넘긴 desiredFVF와 다르다면 처리를 해 주어야 한다
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
}

void XMesh::Release()
{
	SAFE_RELEASE(_pAdjacency);
	SAFE_DELETE_ARRAY(_attributeRange);
	SAFE_RELEASE(_pMesh);
}

void XMesh::Render()
{
	//월드행렬 셋팅
	//D3DXMATRIXA16 matWorld = Trans->GetFinalMatrix();
	//sStaticMeshEffect->SetMatrix("matWorld", &matWorld);

	////광원 방향 일단...
	//D3DXVECTOR4 lightDir(1, -1, 1, 1);
	//sStaticMeshEffect->SetVector("vLightDir", &lightDir);

	////라이트 컬러 일단 흰색
	//D3DXVECTOR4 lightColor(1, 1, 1, 1);
	//sStaticMeshEffect->SetVector("vLightColor", &lightColor);


	//Effect 로 그리기 시작

	////UINT passNum;
	////sStaticMeshEffect->Begin(&passNum, 0);

	//for (UINT i = 0; i < passNum; i++)
	//{
	//	//sStaticMeshEffect->BeginPass(i);

	//	//서브셋수만큼 돌아 재낀다...
	//	for (int m = 0; m < this->dwMaterialsNum; m++) {

	//		//텍스쳐 셋팅
	//		sStaticMeshEffect->SetTexture("Diffuse_Tex", this->vecDiffuseTex[m]);
	//		sStaticMeshEffect->SetTexture("Normal_Tex", this->vecNormalTex[m]);
	//		sStaticMeshEffect->SetTexture("Specular_Tex", this->vecSpecularTex[m]);
	//		sStaticMeshEffect->SetTexture("Emission_Tex", this->vecEmissionTex[m]);
	//		//스펙파워
	//		sStaticMeshEffect->SetFloat("fSpecPower", this->vecMaterials[m].Power);

	//		//Begin 이 들어오고 난후 값이 바뀌면 다음과 같이 실행
	//		sStaticMeshEffect->CommitChanges();

	//		this->pMesh->DrawSubset(m);
	//	}

	//	sStaticMeshEffect->EndPass();
	//}

	//sStaticMeshEffect->End();

}



////////////////////////////////////////////////////////////////////////

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
	for (DWORD i = 0; i < MAX_FVF_DECL_SIZE; i++) {

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
	this->_boundMin = Vector3(0, 0, 0);
	this->_boundMax = Vector3(0, 0, 0);

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
			if (_boundMin.x > pos->x)		_boundMin.x = pos->x;
			if (_boundMin.y > pos->y)		_boundMin.y = pos->y;
			if (_boundMin.z > pos->z)		_boundMin.z = pos->z;

			//정점 최대 값갱신
			if (_boundMax.x < pos->x)		_boundMax.x = pos->x;
			if (_boundMax.y < pos->y)		_boundMax.y = pos->y;
			if (_boundMax.z < pos->z)		_boundMax.z = pos->z;

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
	this->_boundCenter = (this->_boundMin + this->_boundMax) * 0.5f;
	this->_boundSize = Vector3( _boundMax.x - _boundMin.x, _boundMax.y - _boundMin.y, _boundMax.z - _boundMin.z);
	this->_boundHalfSize = this->_boundSize * 0.5f;
	this->_boundRadius = D3DXVec3Length(&(this->_boundCenter - this->_boundMin));


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
