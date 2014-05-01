#pragma once
#include <d3dx9.h>

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ)
#define SINGLETON(type) \
	public: \
	static type* GetInstance() { static type s; return &s; }

class CollisionObject;

class D3DApp
{
public:
	SINGLETON(D3DApp);
	D3DApp();
	~D3DApp();

// 	static D3DApp* GetInstance() 
// 	{
// 		if (!pInstance)
// 		{
// 			pInstance = new D3DApp();
// 		}
// 		return pInstance;
// 	}
// 	
// 	void Release()
// 	{
// 		if (pInstance)
// 		{
// 			delete pInstance;
// 			pInstance = nullptr;
// 		}
// 	}

	LPDIRECT3D9         g_pD3D = NULL;
	LPDIRECT3DDEVICE9   g_pd3dDevice = NULL;

	HRESULT InitD3D(HWND hWnd);
	VOID Cleanup();
	VOID SetupMatrices();
	VOID Render();

	void InitObjects();

	CollisionObject* Object1 = nullptr;
	CollisionObject* Object2 = nullptr;

private:
	//static D3DApp* pInstance;
};
