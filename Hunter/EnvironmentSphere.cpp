#include "stdafx.h"
#include "EnvironmentSphere.h"

EnvironmentSphere::EnvironmentSphere()
{
}

EnvironmentSphere::~EnvironmentSphere()
{
}

void EnvironmentSphere::Create(const std::string & fileName)
{
	COM_RELEASE(_pTexture);

	D3DXCreateCubeTextureFromFile(gpDevice, fileName.c_str(), &_pTexture);

	if (nullptr == _pMesh)
	{
		if (FAILED(D3DXCreateSphere(gpDevice, 300.0f, 30, 30, &_pMesh, nullptr)))
		{
			Console::Log("Environment Sphere Creation failed");
			return;
		}
	}

	_effectHandle = VIDEO->GetEffect("EnvironmentSphere.fx");

	_pEffect = VIDEO->GetEffect(_effectHandle);

}

void EnvironmentSphere::Destroy()
{
	COM_RELEASE(_pTexture);
	_pEffect = nullptr;
	VIDEO->DestroyEffect(_effectHandle);

	COM_RELEASE(_pMesh);
	
}

void EnvironmentSphere::Render(const Camera & camera)
{
//환경 맵 로딩된게 없다면 리턴해라.....
	if (nullptr == _pTexture)
	{
		return;
	}

	//환경 Effect 셋팅
	_pEffect->_ptr->SetTexture("EnvironmentTexture", _pTexture);

	//WVP 매트릭스
	Vector3 position = camera.GetEntity().GetComponent<TransformComponent>()._position;
	Matrix matWorld;
	MatrixTranslation(&matWorld, position.x, position.y, position.z);
	Matrix matViewProj = camera.GetViewProjectionMatrix();
	Matrix matWVP = matWorld * matViewProj;

	_pEffect->SetMatrix( "matWVP", matWVP );

	uint32 numPass = _pEffect->BeginEffect();

	for( uint32 i = 0 ; i < numPass ; i++ )
	{
		_pEffect->BeginPass( i );
		_pMesh->DrawSubset(0);
		_pEffect->EndPass();
	}
	_pEffect->EndEffect();
}
