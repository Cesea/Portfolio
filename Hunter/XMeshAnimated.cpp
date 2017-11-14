#include "stdafx.h"
#include "XMeshAnimated.h"

STDMETHODIMP HierachyEX::CreateFrame(LPCSTR Name, LPD3DXFRAME * ppNewFrame)
{
	//ppNewFrame 으로 넘겨줄 새로운 BONE 생성
	FrameEX *newFrame = new FrameEX;
	ZeroMemory(newFrame, sizeof(FrameEX));

	//본이름 복사
	if (Name != NULL)
	{
		int len = strlen(Name);
		newFrame->Name = new char[len + 1];
		strcpy_s(newFrame->Name, len + 1, Name);
	}

	//매트릭스 정보 초기화
	D3DXMatrixIdentity(&newFrame->TransformationMatrix);
	D3DXMatrixIdentity(&newFrame->CombinedTransformationMatrix);

	//newFrame->pBoneTransform = NULL;
	//newFrame->pApplyTransform = NULL;

	//리턴값에 새로운 본 주소 대입
	*ppNewFrame = newFrame;
	return S_OK;
}

STDMETHODIMP HierachyEX::CreateMeshContainer(LPCSTR Name, 
	CONST D3DXMESHDATA * pMeshData, 
	CONST D3DXMATERIAL * pMaterials, 
	CONST D3DXEFFECTINSTANCE * pEffectInstance, 
	DWORD NumMaterials, 
	CONST DWORD * pAdjacency, 
	LPD3DXSKININFO pSkinInfo, 
	LPD3DXMESHCONTAINER * ppNewMeshContainer)
{
	////ppNewMeshContainer 로 넘겨줄 새로운 BONEMESH 생성
	ContainerEX* meshContainer = new ContainerEX;
	ZeroMemory(meshContainer, sizeof(ContainerEX));

	////매개변수로 받은 메쉬 데이터의 Device 를 얻는다.
	LPDIRECT3DDEVICE9 d3dDevice = NULL;
	pMeshData->pMesh->GetDevice(&d3dDevice);

	////메시데이터를 물린다.
	meshContainer->MeshData.pMesh = pMeshData->pMesh;
	meshContainer->MeshData.Type = D3DXMESHTYPE_MESH;

	////메시데이터의 레퍼런스 카운트 증가
	pMeshData->pMesh->AddRef();
	////레퍼런스 카운트를 명시적으로 해주지 않는다면 매개변수로 받은 pMeshData 가 함수 호출후 해제 되어 버린다!
	////즉 함수 호출후에도 내부적으로 참조수가 0 이면 자동으로 해제되는 식이다.
	////따라서 새로만들어진 BONEMESH 맴버데이터에 pMeshData 포인터를 물고있으니
	////레퍼런스 카운트 수를 증가 시켜 함수가 끈난뒤에도 해제를 막는다.

	////이름 복사
	if (nullptr != Name)
	{
		int32 len = strlen(Name);
		meshContainer->Name = new char[len + 1];
		strcpy_s(meshContainer->Name, len + 1, Name);
	}

	////인접정보 복사 ( 인접 정보는 면단 3개씩 지니게 된다 왜냐면 폴리곤의 인접수는 3개니깐 )
	DWORD numAdjacency = 3 * (meshContainer->MeshData.pMesh->GetNumFaces());		//인접정보의 수
	meshContainer->pAdjacency = new DWORD[numAdjacency];							//인접 정보의 수만큼 증가
	memcpy(meshContainer->pAdjacency, pAdjacency, sizeof(DWORD) * numAdjacency);

	std::string texFilePath;
	std::string texExp;			//파일 확장자 명
	std::string texFile;		//파일 명

								//재질정보
	std::string texPath;

	meshContainer->NumMaterials = NumMaterials;
	meshContainer->pMaterials = new D3DXMATERIAL[NumMaterials];
	for (DWORD i = 0; i < NumMaterials; i++)
	{
		//메터리얼 정보 복사
		memcpy(&meshContainer->pMaterials[i], &pMaterials[i], sizeof(D3DXMATERIAL));

		//메터리얼정보 푸쉬
		//D3DMATERIAL9 mtrl;
		//memcpy(&mtrl, &pMaterials[i].MatD3D, sizeof(D3DMATERIAL9));
		//mtrl.Ambient = mtrl.Diffuse;		//재질 Ambient
		//meshContainer->Materials.push_back(mtrl);

		//Texture 가 존재한다면 texture경로를 저장하고 
		if (nullptr != pMaterials[i].pTextureFilename)
		{
			meshContainer->_texturePaths.push_back(std::string(pMaterials[i].pTextureFilename));
		}
		//아니라면 이름을 저장하지 않는다
		else
		{
			meshContainer->_texturePaths.push_back(std::string());
		}
	}


	//NOTE : OptimizeInplace를 하지 않았는데도 GetAttributeTable을 가져왔다...???
	//Static Mesh에서 했을때는 optimizeInPlace를 하지 않았을때는 GetAttributeTable함수가 제대로 작동하지 않았다
	//실험해보자...
	//속성테이블을 얻는다.
	meshContainer->MeshData.pMesh->GetAttributeTable(
		NULL, &meshContainer->_numAttributeGroup);	//속성 그룹 갯수를 얻는다.
	if (meshContainer->_numAttributeGroup)
	{
		meshContainer->_attributeRange = new D3DXATTRIBUTERANGE[meshContainer->_numAttributeGroup];
			meshContainer->MeshData.pMesh->GetAttributeTable(
				meshContainer->_attributeRange, nullptr);  //속성 테이블을 얻는다.
	}

	*ppNewMeshContainer = meshContainer;

	//																							
	//스키닝 정보 가 있다면....
	//if (pSkinInfo != NULL)
	//{
	//	//스키닝 정보를 물어놓는다
	//	meshContainer->pSkinInfo = pSkinInfo;
	//	//스킨정보 레퍼런스 카운트 증가 ( 위의 메시데이터의 레퍼런스 카운트와 동일한 과정 )
	//	pSkinInfo->AddRef();
	//	//이메시에 사용되는 본의 수를 얻는다.
	//	int numBone = pSkinInfo->GetNumBones();
	//	//사용되는 본의 수만큼 행렬 동적할당
	//	meshContainer->pBoneOffsetMatices = new Matrix[numBone];
	//	//본 Offset 행렬 셋팅
	//	for (int i = 0; i < numBone; i++)
	//	{
	//		meshContainer->pBoneOffsetMatices[i] = *(Matrix *)(meshContainer->pSkinInfo->GetBoneOffsetMatrix(i));
	//	}
	//	//Effect 에서 사용되는 행렬 팔래트 쵀대 수를 얻는다.
	//	UINT iPaletteSize = 0;
	//	m_pSkinnedMesh->m_pSkinnedEffect->GetInt("MATRIX_PALETTE_SIZE", (INT*)&iPaletteSize);

	//	//기본적으로 적용되는 본수 만큼 팔래트를 할당하는데 Effect 의 최대 MATRIX_PALETTE_SIZE 보다 적용되는 본의 
	//	//수가 많다면 팔래트 수는 Effect MATRIX_PALETTE_SIZE 수로 고정 한다.
	//	meshContainer->NumPaletteEntries = min(iPaletteSize, meshContainer->pSkinInfo->GetNumBones());

	//	//위의 작업에이어 Mesh에  적용되는 bone의 수가 Effect 에서 지원되는 MATRIX_PALETTE_SIZE 수보다 많은 경우
	//	//boneMesh->NumPaletteEntries 수에 맞추어서 Mesh 의 SubSet 을 나눌 필요가 있다.
	//	//아래의 함수는 그러한 작업 들을 WorkingMesh 에 해주고 또한 WorkingMesh 에 
	//	//정점 가중치 개념을 추가해준다.

	//	meshContainer->pSkinInfo->ConvertToIndexedBlendedMesh(
	//		meshContainer->MeshData.pMesh,		//원본 메쉬
	//		D3DXMESH_MANAGED | D3DXMESHOPT_VERTEXCACHE,	//메쉬 옵션
	//		meshContainer->NumPaletteEntries,		//본 팔래트 수를 넘긴다.
	//		meshContainer->pAdjacency,			//인접 버퍼를 넘긴다.
	//		NULL,							//갱신된 인접 버퍼를 받아온다 ( 필요없으면 NULL )
	//		NULL,							//갱신된 폴리곤(감각인덱스) 버퍼를 받아온다 ( 필요없으면 NULL )
	//		NULL,							//갱신된 정점 버퍼를 받아온다 ( 필요없으면 NULL )
	//		&meshContainer->MaxNumFaceInfls,		//한 버텍스에 영향을 주는 본의 최대 갯수를 얻는더.
	//		&meshContainer->NumAttributesGroup,	//본의 컴비네이션 속성그룹의 수를 얻는다ㅓ.
	//		&meshContainer->BufBoneCombos,		//본의 컴비네이션 버퍼를 얻는다.
	//		&meshContainer->WorkingMesh			//원본메쉬에서 작업된 메쉬
	//	);
	//	//본의 컴비네이션 은 메쉬의 Subset 별로 어떠한 본에 영향을 받는지에 대한 정보를 지니게 된다.
	//	//본매트릭스 인덱스 = 본컴비네이션[Subset].BoneID[팔래트]

	//	//cMesh_Skinned 에 작업에 사용되는 팔래트 행렬 수가 부족하면 
	//	//다시 재할당 한다.
	//	if (m_pSkinnedMesh->m_dwWorkingPaletteSize < meshContainer->NumPaletteEntries)
	//	{
	//		if (m_pSkinnedMesh->m_pmWorkingPalette)
	//			delete[] m_pSkinnedMesh->m_pmWorkingPalette;

	//		m_pSkinnedMesh->m_dwWorkingPaletteSize = meshContainer->NumPaletteEntries;
	//		m_pSkinnedMesh->m_pmWorkingPalette = new D3DXMATRIXA16[meshContainer->NumPaletteEntries];
	//	}
	//}

	////밖으로 리턴될 LPD3DXMESHCONTAINER 에 본메쉬 대입
	//*ppNewMeshContainer = meshContainer;

	////얻어온 Device 해재
	//SAFE_RELEASE(d3dDevice);

	return S_OK;
}

STDMETHODIMP HierachyEX::DestroyFrame(LPD3DXFRAME pFrameToFree)
{
	if (pFrameToFree)
	{
		//Transform 작살냄
		//SAFE_DELETE( ( (BONE*)pFrameToFree )->pBoneTransform );
		//이름 해제
		SAFE_DELETE_ARRAY(pFrameToFree->Name);
		SAFE_DELETE(pFrameToFree);
	}
	return S_OK;
}

STDMETHODIMP HierachyEX::DestroyMeshContainer(LPD3DXMESHCONTAINER pMeshContainerToFree)
{
	ContainerEX *toFree = (ContainerEX *)(pMeshContainerToFree);
	SAFE_DELETE_ARRAY(toFree->_attributeRange);
	SAFE_DELETE_ARRAY(toFree->Name);

	SAFE_DELETE(toFree);

	//boneMesh 로 형변환
	//ContainerEX* meshContainer = (ContainerEX*)pMeshContainerToFree;

	////텍스쳐 해제
	//UINT numTex = meshContainer->DiffuseTexs.size();
	//for (UINT i = 0; i < numTex; i++)
	//{
	//	if (meshContainer->pMaterials[i].pTextureFilename != NULL)
	//	{
	//		SAFE_DELETE_ARRAY(meshContainer->pMaterials[i].pTextureFilename);
	//	}
	//}

	//SAFE_DELETE_ARRAY(meshContainer->Name);
	//SAFE_DELETE_ARRAY(meshContainer->pMaterials);
	//SAFE_DELETE_ARRAY(meshContainer->pAdjacency);

	//SAFE_RELEASE(meshContainer->MeshData.pMesh);
	//SAFE_RELEASE(meshContainer->WorkingMesh);
	//SAFE_RELEASE(meshContainer->pSkinInfo);

	//SAFE_DELETE_ARRAY(meshContainer->ppBoneMatrixPtrs);
	//SAFE_DELETE_ARRAY(meshContainer->pBoneOffsetMatices);
	//SAFE_RELEASE(meshContainer->BufBoneCombos);

	//SAFE_DELETE(meshContainer);
	return S_OK;
}


XMeshAnimated::XMeshAnimated()
{
}

XMeshAnimated::~XMeshAnimated(void)
{
}

HRESULT XMeshAnimated::Load(const std::string & filePath, const Matrix * pMatCorrection)
{
	//보정행렬이 들어왔다면...
	if (pMatCorrection)
	{
		_correctionMatrix = *pMatCorrection;
	}
	else
	{
		MatrixIdentity(&_correctionMatrix);
	}

	//이펙트 설정??
	//m_pSkinnedEffect = sSkinnedMeshEffect;

	//스킨드메쉬와 본의 계층 구조의 로딩방식이 재정의된 BONE_HIERARCHY 클래스 객체
	HierachyEX hierachy;
	hierachy._pAnimatedMesh = this;

	//계층구조로 되어있는 XFile 메시 로딩....
	if ((D3DXLoadMeshHierarchyFromX(
		filePath.c_str(),
		D3DXMESH_MANAGED,
		gpDevice,
		&hierachy,					//계층 구조의 로딩 방식이 재정의된 객체를 넘긴다.
		nullptr,
		(D3DXFRAME **)&_pRootbone,					//얻어온 루트 본프레임
		nullptr)))
	{
		return E_FAIL;
	}

	//본 매트릭스 포인터 생성
	InitBoneMatrixPointer((FrameEX*)_pRootbone);

	//uint32 totalNumFrame{};
	//GetTotalNumFrame(&totalNumFrame);

	uint32 level = 0;
	uint32 parentIndex = 0;
	std::stack<uint32> indexStack;
	ChildFirstTraverse(indexStack, nullptr);

	return true;
}

void XMeshAnimated::Release()
{
	HierachyEX hierachy;
	hierachy.DestroyFrame(_pRootbone);
}

void XMeshAnimated::MeshCorrection(const Matrix * pMatCorrection)
{
}

void XMeshAnimated::BuidSubMeshBoundInfo()
{
}

void XMeshAnimated::InitBoneMatrixPointer(FrameEX * pFrame)
{
	if (pFrame == nullptr)
	{
		return;
	}

	//해당 본프레임에 메쉬 컨테이너 가 존재한다면...
	if (pFrame->pMeshContainer)
	{
		//메쉬 컨테이너를 BONE_MESH 형으로 형변환
		ContainerEX* containerMesh = (ContainerEX*)pFrame->pMeshContainer;

		//메쉬에 스키닝 정보가 있다면
		if (containerMesh->pSkinInfo)
		{
			//해당 스킨에 적용되는 본의 갯수만큼
			int numBones = containerMesh->pSkinInfo->GetNumBones();

			//Matrix 포인터 할당
			containerMesh->ppBoneMatrixPtrs = new Matrix*[numBones];

			//적용되는 폰 프레임의 매트릭스를 찾아 해당 본프레임의 행렬 포인터를 물린다.
			for (int32 i = 0; i < numBones; i++)
			{
				//해당 스킨에 적용되는 i 번째의 본이름으로 해당 본 프레임을 루프 본부터 찾아 들어간가
				FrameEX* frameFound = (FrameEX*)D3DXFrameFind(_pRootbone, containerMesh->pSkinInfo->GetBoneName(i));
				//찾았다면
				if (nullptr != frameFound)
				{
					containerMesh->ppBoneMatrixPtrs[i] = &frameFound->CombinedTransformationMatrix;
				}
				else
				{
					containerMesh->ppBoneMatrixPtrs[i] = NULL;
				}
			}
		}
	}

	//이웃 본이 존재한다면...
	if (pFrame->pFrameSibling)
	{
		InitBoneMatrixPointer((FrameEX*)pFrame->pFrameSibling);
	}

	//자식 본이 존재한다면...
	if (pFrame->pFrameFirstChild)
	{
		InitBoneMatrixPointer((FrameEX*)pFrame->pFrameFirstChild);
	}
}

void XMeshAnimated::GetTotalNumFrame(uint32 *pOutCount, FrameEX *frame)
{
	if (frame == nullptr)
	{
		*pOutCount = 0;
		(*pOutCount)++;

		frame = _pRootbone;
		Console::Log("%s\n", frame->Name);

		if (_pRootbone->pFrameSibling)
		{
			GetTotalNumFrame(pOutCount, (FrameEX *)frame->pFrameSibling);
		}

		if (_pRootbone->pFrameFirstChild)
		{
			GetTotalNumFrame(pOutCount, (FrameEX *)frame->pFrameFirstChild);
		}
	}
	else
	{
		Console::Log("%s\n", frame->Name);

		(*pOutCount)++;

		if (frame->pFrameSibling)
		{
			GetTotalNumFrame(pOutCount, (FrameEX *)frame->pFrameSibling);
		}

		if (frame->pFrameFirstChild)
		{
			GetTotalNumFrame(pOutCount, (FrameEX *)frame->pFrameFirstChild);
		}
	}
}


void XMeshAnimated::ChildFirstTraverse(std::stack<uint32> &indexStack, FrameEX *pFrame)
{
	if (pFrame == nullptr)
	{
		pFrame = _pRootbone;
		_linearFrames.push_back(pFrame);

		indexStack.push(0);
		_frameIndex.push_back(indexStack.top());

		if (pFrame->pFrameFirstChild)
		{
			ChildFirstTraverse(indexStack, (FrameEX *)pFrame->pFrameFirstChild);
		}

		if (pFrame->pFrameSibling)
		{
			ChildFirstTraverse(indexStack, (FrameEX *)pFrame->pFrameSibling);
		}
	}
	else
	{
		_linearFrames.push_back(pFrame);

		_frameIndex.push_back(/*level - 1 + */indexStack.top());

		if (pFrame->pFrameFirstChild)
		{
			indexStack.push(_frameIndex.size() - 1);

			//parentIndex = indexStack.top();
			//parentIndex = _frameIndex.size() - 1;
			ChildFirstTraverse(indexStack, (FrameEX *)pFrame->pFrameFirstChild);
		}

		if (pFrame->pFrameSibling)
		{
			ChildFirstTraverse(indexStack, (FrameEX *)pFrame->pFrameSibling);
		}
		else
		{
			indexStack.pop();
		}
		//--level;
	}
}

void XMeshAnimated::GetNumSiblings(uint32 &pOutCount, FrameEX *pFrame)
{
	FrameEX *traversal = pFrame;
	pOutCount = 0;

	while (true)
	{
		traversal = (FrameEX *)traversal->pFrameSibling;
		if (nullptr == traversal)
		{
			break;
		}
		pOutCount++;
	}
}

//void XMeshAnimated::MakeFrameLinearVector()
//{
//	uint32 totalNumFrame = 0;
//	GetTotalNumFrame(&totalNumFrame);
//	_frameLinearVector.reserve(totalNumFrame);
//
//	FrameEX *traversal = nullptr;
//
//	traversal = _pRootbone;
//	_frameLinearVector.push_back(traversal);
//
//	if (traversal->pFrameSibling)
//	{
//		traversal = (FrameEX *)traversal->pFrameSibling;
//		InternalMakeFrameLinearVector(traversal, _frameLinearVector);
//	}
//
//	if (traversal->pFrameFirstChild)
//	{
//		traversal = (FrameEX *)traversal->pFrameFirstChild;
//		InternalMakeFrameLinearVector(traversal, _frameLinearVector);
//	}
//}
//
//void XMeshAnimated::InternalMakeFrameLinearVector(FrameEX *pFrame, std::vector<FrameEX *> &frames)
//{
//	frames.push_back(pFrame);
//
//	if (pFrame->pFrameSibling)
//	{
//		pFrame = (FrameEX *)pFrame->pFrameSibling;
//		InternalMakeFrameLinearVector(pFrame, frames);
//	}
//
//	if (pFrame->pFrameFirstChild)
//	{
//		pFrame = (FrameEX *)pFrame->pFrameFirstChild;
//		InternalMakeFrameLinearVector(pFrame, frames);
//	}
//}
