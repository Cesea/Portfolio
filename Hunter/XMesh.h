#ifndef XMESH_H
#define XMESH_H

class XMesh 
{
public:
	LPD3DXMESH							pMesh;
	DWORD								dwMaterialsNum;			//�ε��� �Ž��� ���� ����
	std::vector<LPDIRECT3DTEXTURE9>		vecDiffuseTex;			//�ε��� �޽����� ����ϴ� DiffuseTexture
	std::vector<LPDIRECT3DTEXTURE9>		vecNormalTex;			//�ε��� �޽����� ����ϴ� NormalTexture
	std::vector<LPDIRECT3DTEXTURE9>		vecSpecularTex;			//�ε��� �޽����� ����ϴ� SpecularTexture
	std::vector<LPDIRECT3DTEXTURE9>		vecEmissionTex;			//�ε��� �޽����� ����ϴ� EmissionTexture
	std::vector<D3DMATERIAL9>			vecMaterials;			//�ε��� �޽����� ����ϴ� ���͸���...

	std::vector<D3DXVECTOR3>			Vertices;				//������
	std::vector<D3DXVECTOR3>			Normals;				//���� �븻
	std::vector<DWORD>					Indices;				//�ε���
	DWORD								TriNum;					//�ﰢ���� ����

	D3DXVECTOR3							Bound_Min;					//�ٿ���� �ּ� ��ġ 
	D3DXVECTOR3							Bound_Max;					//�ٿ���� �ִ� ��ġ 
	D3DXVECTOR3							Bound_Center;				//�ٿ���� ����	( ���� ���� �Ǻ��� �ƴϴ� )
	D3DXVECTOR3							Bound_Size;					//�ٿ���� ũ��
	D3DXVECTOR3							Bound_HalfSize;				//�ٿ���� ����ũ��
	float								Bound_Radius;				//�ٿ���� ������ ������

																	//�����Ŵ����� ��ģ��.....
	friend class cPhysicManager;

public:
	XMesh(void);
	~XMesh(void);

	virtual HRESULT Init(std::string filePath, const D3DXMATRIXA16* matCorrection = NULL);

	virtual void Release();

	virtual void Render();


private:
	//������Ĵ�� �޽��� �����Ѵ�.
	void MeshCorrection(const D3DXMATRIXA16* pmatCorrection);


public:
};



#endif