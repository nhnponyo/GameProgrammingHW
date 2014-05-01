#pragma once
#include <d3dx9.h>

struct BoundingBox
{
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

	HRESULT InitVB();
	HRESULT InitIB();
	void RenderBox();

};
