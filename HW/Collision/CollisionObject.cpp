#include "CollisionObject.h"
#include "D3DApp.h"
#include <stdio.h>

CollisionObject::CollisionObject()
{
	
}

CollisionObject::~CollisionObject()
{
}

HRESULT CollisionObject::InitVB()
{
	// 박스의 8개 vertex를 만든다
	m_vertices[0] = { -1.f, +1.f, +1.f };	// v0
	m_vertices[1] = { +1.f, +1.f, +1.f };	// v1
	m_vertices[2] = { +1.f, +1.f, -1.f };	// v2
	m_vertices[3] = { -1.f, +1.f, -1.f };	// v3
	m_vertices[4] = { -1.f, -1.f, +1.f };	// v4
	m_vertices[5] = { +1.f, -1.f, +1.f };	// v5
	m_vertices[6] = { +1.f, -1.f, -1.f };	// v6
	m_vertices[7] = { -1.f, -1.f, -1.f };	// v7

	if (FAILED(D3DApp::GetInstance()->g_pd3dDevice->CreateVertexBuffer(8 * sizeof(D3DXVECTOR3),
		0, D3DFVF_CUSTOMVERTEX,
		D3DPOOL_DEFAULT, &m_pVB, NULL)))
	{
		return E_FAIL;
	}

	VOID* pVertices;
	if (FAILED(m_pVB->Lock(0, sizeof(m_vertices), (void**)&pVertices, 0)))
		return E_FAIL;
	memcpy(pVertices, m_vertices, sizeof(m_vertices));
	m_pVB->Unlock();

	SetBoundingBox();

	return S_OK;
}

HRESULT CollisionObject::InitIB()
{
	// 박스의 12개 면을 만든다
	MYINDEX	indices[] =
	{
		{ 0, 1, 2 }, { 0, 2, 3 },	/// 윗면
		{ 4, 6, 5 }, { 4, 7, 6 },	/// 아랫면
		{ 0, 3, 7 }, { 0, 7, 4 },	/// 왼면
		{ 1, 5, 6 }, { 1, 6, 2 },	/// 오른면
		{ 3, 2, 6 }, { 3, 6, 7 },	/// 앞면
		{ 0, 4, 5 }, { 0, 5, 1 }	/// 뒷면
	};

	/// 인덱스버퍼 생성
	/// WORD로 인덱스 버퍼를 선언했으므로 D3DFMT_INDEX16 플래그를 쓴다.
	if (FAILED(D3DApp::GetInstance()->g_pd3dDevice->CreateIndexBuffer(12 * sizeof(MYINDEX), 0, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &m_pIB, NULL)))
	{
		return E_FAIL;
	}

	/// 인덱스버퍼를 값으로 채운다.
	/// 인덱스버퍼의 Lock()함수를 호출하여 포인터를 얻어온다.
	VOID* pIndices;
	if (FAILED(m_pIB->Lock(0, sizeof(indices), (void**)&pIndices, 0)))
		return E_FAIL;
	memcpy(pIndices, indices, sizeof(indices));
	m_pIB->Unlock();

	return S_OK;
}

void CollisionObject::RenderBox()
{
	if (!D3DApp::GetInstance()->g_pd3dDevice || !m_pVB || !m_pIB)
	{
		return;
	}

	D3DXMATRIXA16 matWorld, matRot, matMov;
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixIdentity(&matRot);
	D3DXMatrixIdentity(&matMov);

	D3DXMatrixRotationY(&matRot, m_boundingBox.RotationY );
	D3DXMatrixTranslation(&matMov, m_boundingBox.vTans[0], m_boundingBox.vTans[1], m_boundingBox.vTans[2]);
	D3DXMatrixMultiply(&matWorld, &matRot, &matMov);

	D3DApp::GetInstance()->g_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);

	printf("matWorld[0] : %fl, %fl, %fl, %fl\n", matWorld._11, matWorld._12, matWorld._13, matMov._14);
	printf("matWorld[1] : %fl, %fl, %fl, %fl\n", matWorld._21, matWorld._22, matWorld._23, matMov._24);
	printf("matWorld[2] : %fl, %fl, %fl, %fl\n", matWorld._31, matWorld._32, matWorld._33, matMov._34);
	printf("matWorld[3] : %fl, %fl, %fl, %fl\n", matWorld._41, matWorld._42, matWorld._43, matMov._44);

	//printf("vTans : %fl %fl %fl \n", m_boundingBox.vTans[0], m_boundingBox.vTans[1], m_boundingBox.vTans[2]);

	SetBoundingBox();
	//D3DXVec3TransformCoord(&m_boundingBox.maxPos, &m_boundingBox.maxPos, &matWorld);
	//D3DXVec3TransformCoord(&m_boundingBox.minPos, &m_boundingBox.minPos, &matWorld);

	/// 정점버퍼의 삼각형을 그린다.
	/// 1. 정점정보가 담겨있는 정점버퍼를 출력 스트림으로 할당한다.
	D3DApp::GetInstance()->g_pd3dDevice->SetStreamSource(0, m_pVB, 0, sizeof(D3DXVECTOR3));
	/// 2. D3D에게 정점쉐이더 정보를 지정한다. 대부분의 경우에는 FVF만 지정한다.
	D3DApp::GetInstance()->g_pd3dDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
	/// 3. 인덱스버퍼를 지정한다.
	D3DApp::GetInstance()->g_pd3dDevice->SetIndices(m_pIB);
	D3DApp::GetInstance()->g_pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	/// 4. DrawIndexedPrimitive()를 호출한다.
	D3DApp::GetInstance()->g_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 8, 0, 12);
}

void CollisionObject::CheckAABB(BoundingBox targetBox)
{
	if (m_boundingBox.minPos[0] <= targetBox.maxPos[0] && m_boundingBox.maxPos[0] >= targetBox.minPos[0] &&
		m_boundingBox.minPos[1] <= targetBox.maxPos[1] && m_boundingBox.maxPos[1] >= targetBox.minPos[1] &&
		m_boundingBox.minPos[2] <= targetBox.maxPos[2] && m_boundingBox.maxPos[2] >= targetBox.minPos[2])
	{
		aabbCollide = true;
		return;
	}
	aabbCollide = false;
}

void CollisionObject::UpdateBox(BoundingBox targetBox)
{
	CheckAABB(targetBox);
	CheckOBB();
}

void CollisionObject::CheckOBB()
{

}

void CollisionObject::SetBoundingBox()
{
	//random Set
	for (int i = 0; i < 8; ++i)
	{
		printf("vertices[%d]: %fl, %fl, %fl \n", i, m_vertices[i][0], m_vertices[i][1], m_vertices[i][2]);
	}

	m_boundingBox.minPos[0] = m_vertices[0][0];
	m_boundingBox.minPos[1] = m_vertices[0][1];
	m_boundingBox.minPos[2] = m_vertices[0][2];
	m_boundingBox.maxPos[0] = m_vertices[0][0];
	m_boundingBox.maxPos[1] = m_vertices[0][1];
	m_boundingBox.maxPos[2] = m_vertices[0][2];

	for (int i = 0; i < 8; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			m_boundingBox.minPos[j] = __min(m_boundingBox.minPos[j], m_vertices[i][j]);
			m_boundingBox.maxPos[j] = __max(m_boundingBox.maxPos[j], m_vertices[i][j]);
		}
	}

	printf("minPos : %fl %fl %fl\n", m_boundingBox.minPos[0], m_boundingBox.minPos[1], m_boundingBox.minPos[2]);
	printf("maxPos : %fl %fl %fl\n", m_boundingBox.maxPos[0], m_boundingBox.maxPos[1], m_boundingBox.maxPos[2]);
}

void CollisionObject::Release()
{
	if (m_pIB)
	{
		m_pIB->Release();
		m_pIB = NULL;
	}

	if (m_pVB)
	{
		m_pVB->Release();
		m_pVB = NULL;
	}

	if (m_pFont)
	{
		m_pFont->Release();
		m_pFont = NULL;
	}
}

HRESULT CollisionObject::InitFont()
{
	if( SUCCEEDED (D3DXCreateFont(D3DApp::GetInstance()->g_pd3dDevice, 15, 0, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
		L"돋음체", &m_pFont)))
	{
		return S_OK;
	}
	return E_FAIL;
}

void CollisionObject::RenderText()
{
	RECT rt;
	SetRect(&rt, 10, 10, 0, 0);

	if (aabbCollide)
		m_pFont->DrawText(NULL, L"AABB: 충돌", -1, &rt, DT_NOCLIP, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	else
		m_pFont->DrawText(NULL, L"AABB: 비충돌", -1, &rt, DT_NOCLIP, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
}

