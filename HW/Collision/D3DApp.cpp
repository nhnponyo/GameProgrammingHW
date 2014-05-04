#include "D3DApp.h"
#include "CollisionObject.h"
#include <stdio.h>

D3DApp::D3DApp()
{
}


D3DApp::~D3DApp()
{
	Cleanup();
}


//////////////////////////////////////////////////////////////////////////
// D3D 디바이스 생성
//////////////////////////////////////////////////////////////////////////
HRESULT D3DApp::InitD3D(HWND hWnd)
{
	if (NULL == (g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)))
		return E_FAIL;

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp, &g_pd3dDevice)))
	{
		return E_FAIL;
	}

	g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);

	g_pd3dDevice->SetRenderState(D3DRS_AMBIENT, 0xffffffff);

	return S_OK;
}

VOID D3DApp::Cleanup()
{
	if (Object1)
	{
		Object1->Release();
	}

	if (Object2)
	{
		Object2->Release();
	}

	if (g_pd3dDevice != NULL)
	{
		g_pd3dDevice->Release();
	}

	if (g_pD3D != NULL)
	{
		g_pD3D->Release();
	}
}

VOID D3DApp::SetupMatrices()
{
	D3DXMATRIXA16 matWorld;
	D3DXMatrixIdentity(&matWorld);
	//D3DXMatrixRotationY(&matWorld, timeGetTime() / 1000.0f);
	g_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);

	D3DXVECTOR3 vEyePt(0.0f, 3.0f, -5.0f);
	D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);

	D3DXMATRIXA16 matView;
	D3DXMatrixLookAtLH(&matView, &vEyePt, &vLookatPt, &vUpVec);
	g_pd3dDevice->SetTransform(D3DTS_VIEW, &matView);

	D3DXMATRIXA16 matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 4, 1.0f, 1.0f, 100.0f);
	g_pd3dDevice->SetTransform(D3DTS_PROJECTION, &matProj);
}

VOID D3DApp::Render()
{
	g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_XRGB(100, 100, 100), 1.0f, 0);

	if (SUCCEEDED(g_pd3dDevice->BeginScene()))
	{
		SetupMatrices();

		//RenderBox
		if (Object1)
		{
			printf("Object1\n");
			Object1->RenderBox();
			Object1->RenderText();
		}
		if (Object2)
		{
			printf("Object2\n");
			Object2->RenderBox();
		}

		g_pd3dDevice->EndScene();
	}

	g_pd3dDevice->Present(NULL, NULL, NULL, NULL);
}

void D3DApp::InitObjects()
{
	Object1 = new CollisionObject();
	Object1->InitObject();
	Object2 = new CollisionObject();
	Object2->InitObject();
}

VOID D3DApp::Update()
{
	if (Object1)
	{
		Object1->UpdateBox(Object2->m_boundingBox);
	}
	if (Object2)
	{
		Object2->UpdateBox(Object1->m_boundingBox);
	}
}

void D3DApp::MoveBox1(float x, float y, float z)
{
	Object1->m_boundingBox.vTans[0] += x;
	Object1->m_boundingBox.vTans[1] += y;
	Object1->m_boundingBox.vTans[2] += z;

	Object1->m_boundingBox.centerPos[0] += x;
	Object1->m_boundingBox.centerPos[1] += y;
	Object1->m_boundingBox.centerPos[2] += z;

	D3DXMATRIXA16 matMov;
	D3DXMatrixIdentity(&matMov);
	D3DXMatrixTranslation(&matMov, x, y, z);

	for (int i = 0; i < 8; ++i)
	{
		D3DXVec3TransformCoord(&Object1->m_vertices[i], &Object1->m_vertices[i], &matMov);
	}
}

void D3DApp::RotateBox1()
{
	Object1->m_boundingBox.RotationY += 0.1;

	D3DXMATRIXA16 matRot,matMov;
	D3DXMatrixIdentity(&matRot);
	D3DXMatrixIdentity(&matMov);
	D3DXMatrixTranslation(&matMov, -Object1->m_boundingBox.vTans[0], -Object1->m_boundingBox.vTans[1], -Object1->m_boundingBox.vTans[2]);
	D3DXMatrixRotationY(&matRot, 0.1);
	D3DXMatrixMultiply(&matRot, &matMov, &matRot);
	D3DXMatrixTranslation(&matMov, Object1->m_boundingBox.vTans[0], Object1->m_boundingBox.vTans[1], Object1->m_boundingBox.vTans[2]);
	D3DXMatrixMultiply(&matRot, &matRot, &matMov);

	for (int i = 0; i < 8; ++i)
	{
		D3DXVec3TransformCoord(&Object1->m_vertices[i], &Object1->m_vertices[i], &matRot);
	}

	for (int i = 0; i < 3; ++i)
	{
		D3DXVec3TransformNormal(&Object1->m_boundingBox.axis[i], &Object1->m_boundingBox.axis[i], &matRot);
	}
}
