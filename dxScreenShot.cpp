#include "StdAfx.h"
#include <Windows.h>
#include "dxScreenShot.h"



dxScreenShot::dxScreenShot(HWND hWnd)
{
	if(FAILED(InitD3D(hWnd)))	return ;
	m_pWND = hWnd;
}

HRESULT dxScreenShot::ScreenShot(LPCTSTR fileName)
{
	// Get the screen data
    if(D3D_OK != m_pd3dDevice->GetFrontBufferData (0, m_pSurface))
	{
		MessageBox(m_pWND, _T("Unable to GetFrontBufferData "), _T("Error"), MB_OK|MB_ICONERROR);
		return E_FAIL;
	}

	RECT *rect = NULL ;
	WINDOWINFO windowInfo ;
	windowInfo.cbSize = sizeof(WINDOWINFO) ;
 
	if(!m_pWND) return E_FAIL;
	
	// capture window
	::GetWindowInfo(m_pWND, &windowInfo) ;
	rect = &windowInfo.rcWindow ;
    if(D3DERR_INVALIDCALL == D3DXSaveSurfaceToFile (fileName, D3DXIFF_BMP, m_pSurface, NULL, rect))
	{
		MessageBox(m_pWND, _T("Unable to D3DXSaveSurfaceToFile "), _T("Error"), MB_OK|MB_ICONERROR);
		return E_FAIL;
	}
	return  S_OK;
}

HRESULT dxScreenShot::InitD3D(HWND hWnd)
{
	D3DDISPLAYMODE	ddm;
	D3DPRESENT_PARAMETERS	d3dpp;

	if((m_pD3D = Direct3DCreate9(D3D_SDK_VERSION))==NULL)
	{
		MessageBox(m_pWND, _T("Unable to Create Direct3D "), _T("Error"), MB_OK|MB_ICONERROR);
		return E_FAIL;
	}

	if(FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT,&ddm)))
	{
		MessageBox(m_pWND, _T("Unable to Get Adapter Display Mode"),_T("Error"), MB_OK|MB_ICONERROR);
		return E_FAIL;
	}

	ZeroMemory(&d3dpp,sizeof(D3DPRESENT_PARAMETERS));

	d3dpp.Windowed = TRUE;
	d3dpp.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;
	d3dpp.BackBufferFormat = ddm.Format;
	d3dpp.BackBufferHeight = ddm.Height;
	d3dpp.BackBufferWidth =  ddm.Width;
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = hWnd;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;

	if(FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,hWnd,D3DCREATE_SOFTWARE_VERTEXPROCESSING ,&d3dpp,&m_pd3dDevice)))
	{
		MessageBox(m_pWND, _T("Unable to Create Device"),_T("Error"), MB_OK|MB_ICONERROR);
		return E_FAIL;
	}

	if(FAILED(m_pd3dDevice->CreateOffscreenPlainSurface(ddm.Width, ddm.Height, D3DFMT_A8R8G8B8, D3DPOOL_SCRATCH, &m_pSurface, NULL)))
	{
		MessageBox(m_pWND, _T("Unable to Create Surface"),_T("Error"), MB_OK|MB_ICONERROR);
		return E_FAIL;
	}

	return S_OK;
}

void dxScreenShot::CleanupD3D()
{
	if(m_pSurface)
	{
		m_pSurface->Release();
		m_pSurface=NULL;
	}
	if(m_pd3dDevice)
	{
		m_pd3dDevice->Release();
		m_pd3dDevice=NULL;
	}
	if(m_pD3D)
	{
		m_pD3D->Release();
		m_pD3D=NULL;
	}
}

dxScreenShot::~dxScreenShot(void)
{
	CleanupD3D();
}
