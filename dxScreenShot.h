#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include <d3d11.h>
#include <d3dx11.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

class dxScreenShot
{
public:
	dxScreenShot(HWND);
	~dxScreenShot(void);
		
	HRESULT ScreenShot(LPCTSTR);
private:
	IDirect3D9*			m_pD3D;
	IDirect3DDevice9*	m_pd3dDevice;
	IDirect3DSurface9*	m_pSurface;
	HWND				m_pWND;

private:
	void CleanupD3D();
	HRESULT InitD3D(HWND);
};

