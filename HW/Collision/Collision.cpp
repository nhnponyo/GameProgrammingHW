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
		D3DApp::GetInstance()->InitObjects();

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
				D3DApp::GetInstance()->Update();
			}

			if (msg.message == WM_KEYDOWN)
			{
				switch (static_cast<unsigned char>(msg.wParam))
				{
				case 65: //a
					D3DApp::GetInstance()->MoveBox1(-0.1,0,0);
					break;
				case 68: //d
					D3DApp::GetInstance()->MoveBox1(0.1,0,0);
					break;
				}
			}

			if (msg.message == WM_KEYUP)
			{
				//D3DApp::GetInstance()->ResetBox1Moving();
			}
		}
	};

    return 0;
}



