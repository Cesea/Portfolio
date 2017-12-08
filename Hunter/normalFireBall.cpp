#include "stdafx.h"
#include "normalFireBall.h"


normalFireBall::normalFireBall(const std::string& fxName,
	const std::string& techName,
	const std::string& texName,
	const Vector3& accel,
	int maxNumParticles,
	float timePerParticle, Vector3 pos, float radius)
	:PSystem(fxName, techName, texName, accel,
		maxNumParticles, timePerParticle, pos)
{
	_radius = radius;

	D3DVERTEXELEMENT9 vertElement[8];

	vertElement[0].Stream = 0;
	vertElement[0].Offset = 0;
	vertElement[0].Type = D3DDECLTYPE_FLOAT3;
	vertElement[0].Method = D3DDECLMETHOD_DEFAULT;
	vertElement[0].Usage = D3DDECLUSAGE_POSITION;
	vertElement[0].UsageIndex = 0;


	vertElement[1].Stream = 0;
	vertElement[1].Offset = 12;
	vertElement[1].Type = D3DDECLTYPE_FLOAT3;
	vertElement[1].Method = D3DDECLMETHOD_DEFAULT;
	vertElement[1].Usage = D3DDECLUSAGE_TEXCOORD;
	vertElement[1].UsageIndex = 0;

	//size
	vertElement[2].Stream = 0;
	vertElement[2].Offset = 24;
	vertElement[2].Type = D3DDECLTYPE_FLOAT1;
	vertElement[2].Method = D3DDECLMETHOD_DEFAULT;
	vertElement[2].Usage = D3DDECLUSAGE_TEXCOORD;
	vertElement[2].UsageIndex = 1;

	//time
	vertElement[3].Stream = 0;
	vertElement[3].Offset = 28;
	vertElement[3].Type = D3DDECLTYPE_FLOAT1;
	vertElement[3].Method = D3DDECLMETHOD_DEFAULT;
	vertElement[3].Usage = D3DDECLUSAGE_TEXCOORD;
	vertElement[3].UsageIndex = 2;

	//lifetime
	vertElement[4].Stream = 0;
	vertElement[4].Offset = 32;
	vertElement[4].Type = D3DDECLTYPE_FLOAT1;
	vertElement[4].Method = D3DDECLMETHOD_DEFAULT;
	vertElement[4].Usage = D3DDECLUSAGE_TEXCOORD;
	vertElement[4].UsageIndex = 3;

	//mass
	vertElement[5].Stream = 0;
	vertElement[5].Offset = 36;
	vertElement[5].Type = D3DDECLTYPE_FLOAT1;
	vertElement[5].Method = D3DDECLMETHOD_DEFAULT;
	vertElement[5].Usage = D3DDECLUSAGE_TEXCOORD;
	vertElement[5].UsageIndex = 4;

	//color
	vertElement[6].Stream = 0;
	vertElement[6].Offset = 40;
	vertElement[6].Type = D3DDECLTYPE_FLOAT4;
	vertElement[6].Method = D3DDECLMETHOD_DEFAULT;
	vertElement[6].Usage = D3DDECLUSAGE_COLOR;
	vertElement[6].UsageIndex = 0;


	D3DVERTEXELEMENT9 end = D3DDECL_END();

	vertElement[7] = end;

	gpDevice->CreateVertexDeclaration(vertElement, &Decl);

	direction = accel;
}


normalFireBall::~normalFireBall()
{
}