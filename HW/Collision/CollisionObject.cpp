#include "CollisionObject.h"
#include "D3DApp.h"

CollisionObject::CollisionObject()
{
	
}

CollisionObject::~CollisionObject()
{
}

HRESULT CollisionObject::InitVB()
{
	// �ڽ��� 8�� vertex�� �����
	D3DXVECTOR3 Vertices[8] =
	{
		{ - 1, + 1, + 1 },		// v0
		{ + 1, + 1, + 1 },		// v1
		{ + 1, + 1, - 1 },		// v2
		{ - 1, + 1, - 1 },		// v3
		{ - 1, - 1, + 1 },		// v4
		{ + 1, - 1, + 1 },		// v5
		{ + 1, - 1, - 1 },		// v6
		{ - 1, - 1, - 1 },		// v7
	};

	if (FAILED(D3DApp::GetInstance()->g_pd3dDevice->CreateVertexBuffer(8 * sizeof(D3DXVECTOR3),
		0, D3DFVF_CUSTOMVERTEX,
		D3DPOOL_DEFAULT, &m_pVB, NULL)))
	{
		return E_FAIL;
	}

	VOID* pVertices;
	if (FAILED(m_pVB->Lock(0, sizeof(Vertices), (void**)&pVertices, 0)))
		return E_FAIL;
	memcpy(pVertices, Vertices, sizeof(Vertices));
	m_pVB->Unlock();

	SetBoundingBox(Vertices);

	return S_OK;
}

HRESULT CollisionObject::InitIB()
{
	// �ڽ��� 12�� ���� �����
	MYINDEX	indices[] =
	{
		{ 0, 1, 2 }, { 0, 2, 3 },	/// ����
		{ 4, 6, 5 }, { 4, 7, 6 },	/// �Ʒ���
		{ 0, 3, 7 }, { 0, 7, 4 },	/// �޸�
		{ 1, 5, 6 }, { 1, 6, 2 },	/// ������
		{ 3, 2, 6 }, { 3, 6, 7 },	/// �ո�
		{ 0, 4, 5 }, { 0, 5, 1 }	/// �޸�
	};

	/// �ε������� ����
	/// WORD�� �ε��� ���۸� ���������Ƿ� D3DFMT_INDEX16 �÷��׸� ����.
	if (FAILED(D3DApp::GetInstance()->g_pd3dDevice->CreateIndexBuffer(12 * sizeof(MYINDEX), 0, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &m_pIB, NULL)))
	{
		return E_FAIL;
	}

	/// �ε������۸� ������ ä���.
	/// �ε��������� Lock()�Լ��� ȣ���Ͽ� �����͸� ���´�.
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

	D3DXVec3TransformCoord(&m_boundingBox.maxPos, &m_boundingBox.maxPos, &matWorld);
	D3DXVec3TransformCoord(&m_boundingBox.minPos, &m_boundingBox.minPos, &matWorld);

	/// ���������� �ﰢ���� �׸���.
	/// 1. ���������� ����ִ� �������۸� ��� ��Ʈ������ �Ҵ��Ѵ�.
	D3DApp::GetInstance()->g_pd3dDevice->SetStreamSource(0, m_pVB, 0, sizeof(D3DXVECTOR3));
	/// 2. D3D���� �������̴� ������ �����Ѵ�. ��κ��� ��쿡�� FVF�� �����Ѵ�.
	D3DApp::GetInstance()->g_pd3dDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
	/// 3. �ε������۸� �����Ѵ�.
	D3DApp::GetInstance()->g_pd3dDevice->SetIndices(m_pIB);
	D3DApp::GetInstance()->g_pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	/// 4. DrawIndexedPrimitive()�� ȣ���Ѵ�.
	D3DApp::GetInstance()->g_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 8, 0, 12);
}

void CollisionObject::CheckAABB(BoundingBox targetBox)
{
	if (m_boundingBox.minPos[0] <= targetBox.maxPos[0] && m_boundingBox.maxPos[0] >= targetBox.minPos[0] &&
		m_boundingBox.minPos[1] <= targetBox.maxPos[1] && m_boundingBox.maxPos[1] >= targetBox.minPos[1] &&
		m_boundingBox.minPos[2] <= targetBox.maxPos[2] && m_boundingBox.maxPos[2] >= targetBox.minPos[2])
	{
		aabbCollide = true;
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

void CollisionObject::SetBoundingBox( D3DXVECTOR3* Vertices )
{
	//random Set
	m_boundingBox.minPos[0] = Vertices[0][0];
	m_boundingBox.minPos[1] = Vertices[0][1];
	m_boundingBox.minPos[2] = Vertices[0][2];

	for (int i = 0; i < 8; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			m_boundingBox.minPos[j] = __min(m_boundingBox.minPos[j], Vertices[i][j]);
			m_boundingBox.maxPos[j] = __max(m_boundingBox.maxPos[j], Vertices[i][j]);
		}
	}

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

