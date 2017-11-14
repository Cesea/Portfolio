#include "stdafx.h"
#include "XMeshAnimated.h"

STDMETHODIMP HierachyEX::CreateFrame(LPCSTR Name, LPD3DXFRAME * ppNewFrame)
{
	//ppNewFrame ���� �Ѱ��� ���ο� BONE ����
	FrameEX *newFrame = new FrameEX;
	ZeroMemory(newFrame, sizeof(FrameEX));

	//���̸� ����
	if (Name != NULL)
	{
		int len = strlen(Name);
		newFrame->Name = new char[len + 1];
		strcpy_s(newFrame->Name, len + 1, Name);
	}

	//��Ʈ���� ���� �ʱ�ȭ
	D3DXMatrixIdentity(&newFrame->TransformationMatrix);
	D3DXMatrixIdentity(&newFrame->CombinedTransformationMatrix);

	//newFrame->pBoneTransform = NULL;
	//newFrame->pApplyTransform = NULL;

	//���ϰ��� ���ο� �� �ּ� ����
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
	////ppNewMeshContainer �� �Ѱ��� ���ο� BONEMESH ����
	ContainerEX* meshContainer = new ContainerEX;
	ZeroMemory(meshContainer, sizeof(ContainerEX));

	////�Ű������� ���� �޽� �������� Device �� ��´�.
	LPDIRECT3DDEVICE9 d3dDevice = NULL;
	pMeshData->pMesh->GetDevice(&d3dDevice);

	////�޽õ����͸� ������.
	meshContainer->MeshData.pMesh = pMeshData->pMesh;
	meshContainer->MeshData.Type = D3DXMESHTYPE_MESH;

	////�޽õ������� ���۷��� ī��Ʈ ����
	pMeshData->pMesh->AddRef();
	////���۷��� ī��Ʈ�� ��������� ������ �ʴ´ٸ� �Ű������� ���� pMeshData �� �Լ� ȣ���� ���� �Ǿ� ������!
	////�� �Լ� ȣ���Ŀ��� ���������� �������� 0 �̸� �ڵ����� �����Ǵ� ���̴�.
	////���� ���θ������ BONEMESH �ɹ������Ϳ� pMeshData �����͸� ����������
	////���۷��� ī��Ʈ ���� ���� ���� �Լ��� �����ڿ��� ������ ���´�.

	////�̸� ����
	if (nullptr != Name)
	{
		int32 len = strlen(Name);
		meshContainer->Name = new char[len + 1];
		strcpy_s(meshContainer->Name, len + 1, Name);
	}

	////�������� ���� ( ���� ������ ��� 3���� ���ϰ� �ȴ� �ֳĸ� �������� �������� 3���ϱ� )
	DWORD numAdjacency = 3 * (meshContainer->MeshData.pMesh->GetNumFaces());		//���������� ��
	meshContainer->pAdjacency = new DWORD[numAdjacency];							//���� ������ ����ŭ ����
	memcpy(meshContainer->pAdjacency, pAdjacency, sizeof(DWORD) * numAdjacency);

	std::string texFilePath;
	std::string texExp;			//���� Ȯ���� ��
	std::string texFile;		//���� ��

								//��������
	std::string texPath;

	meshContainer->NumMaterials = NumMaterials;
	meshContainer->pMaterials = new D3DXMATERIAL[NumMaterials];
	for (DWORD i = 0; i < NumMaterials; i++)
	{
		//���͸��� ���� ����
		memcpy(&meshContainer->pMaterials[i], &pMaterials[i], sizeof(D3DXMATERIAL));

		//���͸������� Ǫ��
		//D3DMATERIAL9 mtrl;
		//memcpy(&mtrl, &pMaterials[i].MatD3D, sizeof(D3DMATERIAL9));
		//mtrl.Ambient = mtrl.Diffuse;		//���� Ambient
		//meshContainer->Materials.push_back(mtrl);

		//Texture �� �����Ѵٸ� texture��θ� �����ϰ� 
		if (nullptr != pMaterials[i].pTextureFilename)
		{
			meshContainer->_texturePaths.push_back(std::string(pMaterials[i].pTextureFilename));
		}
		//�ƴ϶�� �̸��� �������� �ʴ´�
		else
		{
			meshContainer->_texturePaths.push_back(std::string());
		}
	}


	//NOTE : OptimizeInplace�� ���� �ʾҴµ��� GetAttributeTable�� �����Դ�...???
	//Static Mesh���� �������� optimizeInPlace�� ���� �ʾ������� GetAttributeTable�Լ��� ����� �۵����� �ʾҴ�
	//�����غ���...
	//�Ӽ����̺��� ��´�.
	meshContainer->MeshData.pMesh->GetAttributeTable(
		NULL, &meshContainer->_numAttributeGroup);	//�Ӽ� �׷� ������ ��´�.
	if (meshContainer->_numAttributeGroup)
	{
		meshContainer->_attributeRange = new D3DXATTRIBUTERANGE[meshContainer->_numAttributeGroup];
			meshContainer->MeshData.pMesh->GetAttributeTable(
				meshContainer->_attributeRange, nullptr);  //�Ӽ� ���̺��� ��´�.
	}

	*ppNewMeshContainer = meshContainer;

	//																							
	//��Ű�� ���� �� �ִٸ�....
	//if (pSkinInfo != NULL)
	//{
	//	//��Ű�� ������ ������´�
	//	meshContainer->pSkinInfo = pSkinInfo;
	//	//��Ų���� ���۷��� ī��Ʈ ���� ( ���� �޽õ������� ���۷��� ī��Ʈ�� ������ ���� )
	//	pSkinInfo->AddRef();
	//	//�̸޽ÿ� ���Ǵ� ���� ���� ��´�.
	//	int numBone = pSkinInfo->GetNumBones();
	//	//���Ǵ� ���� ����ŭ ��� �����Ҵ�
	//	meshContainer->pBoneOffsetMatices = new Matrix[numBone];
	//	//�� Offset ��� ����
	//	for (int i = 0; i < numBone; i++)
	//	{
	//		meshContainer->pBoneOffsetMatices[i] = *(Matrix *)(meshContainer->pSkinInfo->GetBoneOffsetMatrix(i));
	//	}
	//	//Effect ���� ���Ǵ� ��� �ȷ�Ʈ ���� ���� ��´�.
	//	UINT iPaletteSize = 0;
	//	m_pSkinnedMesh->m_pSkinnedEffect->GetInt("MATRIX_PALETTE_SIZE", (INT*)&iPaletteSize);

	//	//�⺻������ ����Ǵ� ���� ��ŭ �ȷ�Ʈ�� �Ҵ��ϴµ� Effect �� �ִ� MATRIX_PALETTE_SIZE ���� ����Ǵ� ���� 
	//	//���� ���ٸ� �ȷ�Ʈ ���� Effect MATRIX_PALETTE_SIZE ���� ���� �Ѵ�.
	//	meshContainer->NumPaletteEntries = min(iPaletteSize, meshContainer->pSkinInfo->GetNumBones());

	//	//���� �۾����̾� Mesh��  ����Ǵ� bone�� ���� Effect ���� �����Ǵ� MATRIX_PALETTE_SIZE ������ ���� ���
	//	//boneMesh->NumPaletteEntries ���� ���߾ Mesh �� SubSet �� ���� �ʿ䰡 �ִ�.
	//	//�Ʒ��� �Լ��� �׷��� �۾� ���� WorkingMesh �� ���ְ� ���� WorkingMesh �� 
	//	//���� ����ġ ������ �߰����ش�.

	//	meshContainer->pSkinInfo->ConvertToIndexedBlendedMesh(
	//		meshContainer->MeshData.pMesh,		//���� �޽�
	//		D3DXMESH_MANAGED | D3DXMESHOPT_VERTEXCACHE,	//�޽� �ɼ�
	//		meshContainer->NumPaletteEntries,		//�� �ȷ�Ʈ ���� �ѱ��.
	//		meshContainer->pAdjacency,			//���� ���۸� �ѱ��.
	//		NULL,							//���ŵ� ���� ���۸� �޾ƿ´� ( �ʿ������ NULL )
	//		NULL,							//���ŵ� ������(�����ε���) ���۸� �޾ƿ´� ( �ʿ������ NULL )
	//		NULL,							//���ŵ� ���� ���۸� �޾ƿ´� ( �ʿ������ NULL )
	//		&meshContainer->MaxNumFaceInfls,		//�� ���ؽ��� ������ �ִ� ���� �ִ� ������ ��´�.
	//		&meshContainer->NumAttributesGroup,	//���� �ĺ���̼� �Ӽ��׷��� ���� ��´٤�.
	//		&meshContainer->BufBoneCombos,		//���� �ĺ���̼� ���۸� ��´�.
	//		&meshContainer->WorkingMesh			//�����޽����� �۾��� �޽�
	//	);
	//	//���� �ĺ���̼� �� �޽��� Subset ���� ��� ���� ������ �޴����� ���� ������ ���ϰ� �ȴ�.
	//	//����Ʈ���� �ε��� = ���ĺ���̼�[Subset].BoneID[�ȷ�Ʈ]

	//	//cMesh_Skinned �� �۾��� ���Ǵ� �ȷ�Ʈ ��� ���� �����ϸ� 
	//	//�ٽ� ���Ҵ� �Ѵ�.
	//	if (m_pSkinnedMesh->m_dwWorkingPaletteSize < meshContainer->NumPaletteEntries)
	//	{
	//		if (m_pSkinnedMesh->m_pmWorkingPalette)
	//			delete[] m_pSkinnedMesh->m_pmWorkingPalette;

	//		m_pSkinnedMesh->m_dwWorkingPaletteSize = meshContainer->NumPaletteEntries;
	//		m_pSkinnedMesh->m_pmWorkingPalette = new D3DXMATRIXA16[meshContainer->NumPaletteEntries];
	//	}
	//}

	////������ ���ϵ� LPD3DXMESHCONTAINER �� ���޽� ����
	//*ppNewMeshContainer = meshContainer;

	////���� Device ����
	//SAFE_RELEASE(d3dDevice);

	return S_OK;
}

STDMETHODIMP HierachyEX::DestroyFrame(LPD3DXFRAME pFrameToFree)
{
	if (pFrameToFree)
	{
		//Transform �ۻ쳿
		//SAFE_DELETE( ( (BONE*)pFrameToFree )->pBoneTransform );
		//�̸� ����
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

	//boneMesh �� ����ȯ
	//ContainerEX* meshContainer = (ContainerEX*)pMeshContainerToFree;

	////�ؽ��� ����
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
	//��������� ���Դٸ�...
	if (pMatCorrection)
	{
		_correctionMatrix = *pMatCorrection;
	}
	else
	{
		MatrixIdentity(&_correctionMatrix);
	}

	//����Ʈ ����??
	//m_pSkinnedEffect = sSkinnedMeshEffect;

	//��Ų��޽��� ���� ���� ������ �ε������ �����ǵ� BONE_HIERARCHY Ŭ���� ��ü
	HierachyEX hierachy;
	hierachy._pAnimatedMesh = this;

	//���������� �Ǿ��ִ� XFile �޽� �ε�....
	if ((D3DXLoadMeshHierarchyFromX(
		filePath.c_str(),
		D3DXMESH_MANAGED,
		gpDevice,
		&hierachy,					//���� ������ �ε� ����� �����ǵ� ��ü�� �ѱ��.
		nullptr,
		(D3DXFRAME **)&_pRootbone,					//���� ��Ʈ ��������
		nullptr)))
	{
		return E_FAIL;
	}

	//�� ��Ʈ���� ������ ����
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

	//�ش� �������ӿ� �޽� �����̳� �� �����Ѵٸ�...
	if (pFrame->pMeshContainer)
	{
		//�޽� �����̳ʸ� BONE_MESH ������ ����ȯ
		ContainerEX* containerMesh = (ContainerEX*)pFrame->pMeshContainer;

		//�޽��� ��Ű�� ������ �ִٸ�
		if (containerMesh->pSkinInfo)
		{
			//�ش� ��Ų�� ����Ǵ� ���� ������ŭ
			int numBones = containerMesh->pSkinInfo->GetNumBones();

			//Matrix ������ �Ҵ�
			containerMesh->ppBoneMatrixPtrs = new Matrix*[numBones];

			//����Ǵ� �� �������� ��Ʈ������ ã�� �ش� ���������� ��� �����͸� ������.
			for (int32 i = 0; i < numBones; i++)
			{
				//�ش� ��Ų�� ����Ǵ� i ��°�� ���̸����� �ش� �� �������� ���� ������ ã�� ����
				FrameEX* frameFound = (FrameEX*)D3DXFrameFind(_pRootbone, containerMesh->pSkinInfo->GetBoneName(i));
				//ã�Ҵٸ�
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

	//�̿� ���� �����Ѵٸ�...
	if (pFrame->pFrameSibling)
	{
		InitBoneMatrixPointer((FrameEX*)pFrame->pFrameSibling);
	}

	//�ڽ� ���� �����Ѵٸ�...
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
