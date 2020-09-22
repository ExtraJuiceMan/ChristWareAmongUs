#pragma once

#include <d3d11.h>

typedef HRESULT(__stdcall* D3D_PRESENT_FUNCTION)(IDXGISwapChain* pThis, UINT SyncInterval, UINT Flags);

const D3D_DRIVER_TYPE DRIVER_TYPE_LIST[4] = { D3D_DRIVER_TYPE_REFERENCE, D3D_DRIVER_TYPE_SOFTWARE, D3D_DRIVER_TYPE_HARDWARE, D3D_DRIVER_TYPE_WARP };

#define SAFE_RELEASE(p) if (p) { p->Release(); p = nullptr; } 

bool GetD3D11SwapchainDeviceContext(IDXGISwapChain **pSwapchain, ID3D11Device **pDevice,
    ID3D11DeviceContext **pContextTable);

D3D_PRESENT_FUNCTION GetD3D11PresentFunction();