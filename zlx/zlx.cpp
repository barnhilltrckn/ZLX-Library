#include "zlx.h"
#include "Draw.h"
#ifdef LIBXENON
struct XenosDevice _xe;
struct XenosSurface *fb = NULL;
#endif

namespace ZLX {
    ZLXVideoDevice * g_pVideoDevice = NULL;

    void Begin() {
#ifndef LIBXENON
        g_pVideoDevice->Clear(0, NULL, D3DCLEAR_ZBUFFER | D3DCLEAR_TARGET | D3DCLEAR_STENCIL, m_backColor, 1.0f, 0);
        g_pVideoDevice->BeginScene();
#else
        Xe_InvalidateState(g_pVideoDevice);
#endif
    }

    void End() {
#ifndef LIBXENON
        g_pVideoDevice->EndScene();
        g_pVideoDevice->Present(NULL, NULL, NULL, NULL);
#else
        Draw::Reset();
        
        Xe_Resolve(g_pVideoDevice);
        // wait for vsync
        while (!Xe_IsVBlank(g_pVideoDevice)); //slowdown ...
        Xe_Sync(g_pVideoDevice);
        
#endif
    }


#ifdef WIN32
    HWND hWnd;

    void W32Update() {
        ShowWindow(hWnd, SW_SHOWDEFAULT);
        UpdateWindow(hWnd);

        // Enter the message loop
        MSG msg;
        ZeroMemory(&msg, sizeof ( msg));
        //while( msg.message != WM_QUIT )
        {
            if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE)) {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
    }
#endif

}