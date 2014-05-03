#pragma once
#include <d3dx9.h>

struct BoundingBox
{
	D3DXVECTOR3 vTans = {0.f, 0.f, 0.f};
	float RotationY = 0;
	D3DXVECTOR3 minPos, maxPos;

	D3DXVECTOR3 centerPos;
	D3DXVECTOR3 axis[3];
	float axisLen[3];
};

struct MYINDEX
{
	WORD	_0, _1, _2;
};

class CollisionObject
{
public:
	CollisionObject();
	~CollisionObject();

	BoundingBox m_boundingBox;

	LPDIRECT3DVERTEXBUFFER9 m_pVB = NULL;
	LPDIRECT3DINDEXBUFFER9	m_pIB = NULL;
	LPD3DXFONT m_pFont = NULL;

	void SetBoundingBox(D3DXVECTOR3* Vertices);

	HRESULT InitVB();
	HRESULT InitIB();
	HRESULT InitFont();

	void InitObject() { InitVB(); InitIB(); }
	void RenderBox();
	void UpdateBox(BoundingBox targetBox);
	void Release();

	bool	obbCollide = false;
	bool	aabbCollide = false;

	void	CheckOBB();
	void	CheckAABB(BoundingBox targetBox);
};
