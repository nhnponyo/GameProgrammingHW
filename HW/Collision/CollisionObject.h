#pragma once
#include <d3dx9.h>

struct BoundingBox
{
	D3DXVECTOR3 vTans = {0.f, 0.f, 0.f};
	float RotationY = 0.f;
	D3DXVECTOR3 minPos;
	D3DXVECTOR3 maxPos;

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

	D3DXVECTOR3 m_vertices[8];
	LPDIRECT3DVERTEXBUFFER9 m_pVB = NULL;
	LPDIRECT3DINDEXBUFFER9	m_pIB = NULL;
	LPD3DXFONT m_pFont = NULL;

	void SetBoundingBox();

	HRESULT InitVB();
	HRESULT InitIB();
	HRESULT InitFont();

	void InitObject() { InitVB(); InitIB(); InitFont(); }
	void RenderBox();
	void RenderText();
	void UpdateBox(BoundingBox targetBox);
	void Release();

	bool	obbCollide = false;
	bool	aabbCollide = false;

	void	CheckOBB(BoundingBox targetBox);
	void	CheckAABB(BoundingBox targetBox);
};
