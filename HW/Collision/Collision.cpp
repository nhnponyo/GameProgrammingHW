#include <Windows.h>
#include <mmsystem.h>
#include <d3dx9.h>
#pragma warning( disable : 4996 )
#include <strsafe.h>
#pragma warning( default : 4996 )
#include "list"
#include "D3DApp.h"
#include "CollisionObject.h"


LRESULT WINAPI MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
    switch( msg )
    {
        case WM_DESTROY:
            PostQuitMessage( 0 );
            return 0;
    }

    return DefWindowProc( hWnd, msg, wParam, lParam );
}

INT WINAPI wWinMain( HINSTANCE hInst, HINSTANCE, LPWSTR, INT )
{
    UNREFERENCED_PARAMETER( hInst );

    WNDCLASSEX wc =
    {
        sizeof( WNDCLASSEX ), CS_CLASSDC, MsgProc, 0L, 0L,
        GetModuleHandle( NULL ), NULL, NULL, NULL, NULL,
        L"Collision", NULL
    };
    RegisterClassEx( &wc );

    HWND hWnd = CreateWindow( L"Collision", L"D3D Collision",
                              WS_OVERLAPPEDWINDOW, 100, 100, 700, 500,
                              NULL, NULL, wc.hInstance, NULL );

	if ( SUCCEEDED( D3DApp::GetInstance()->InitD3D(hWnd) ) )
	{
		CollisionObject g_obj1;
		CollisionObject g_obj2;

		ShowWindow(hWnd, SW_SHOWDEFAULT);
		UpdateWindow(hWnd);

		MSG msg;
		ZeroMemory(&msg, sizeof(msg));
		while (msg.message != WM_QUIT)
		{
			if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				D3DApp::GetInstance()->Render();
			}
		}
	};

    return 0;
}



