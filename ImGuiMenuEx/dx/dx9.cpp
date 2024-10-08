#include "dx9.h"
#pragma comment(lib, "Detour4.0//libs//x86_//detours.lib")
void* o_present	= nullptr; void* o_reset = nullptr; void* render_frame = nullptr;
void** init_device();
HRESULT __stdcall present_detour(IDirect3DDevice9* p_device, RECT* p_source_rect, RECT* p_dest_rect, HWND h_dest_window_override, RGNDATA* p_dirty_region);
bool dx9::set_frame_render(void* render)
{
	render_frame = render;
	if (const auto table = init_device())
	{
		o_present = table[1]; o_reset = table[0];
		printf("present %p\n", o_present);
		printf("reset %p\n", o_reset);
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourAttach(&static_cast<void*&>(o_present), present_detour);
		//DetourAttach(&static_cast<void*&>(o_reset),		reset_detour);
		DetourTransactionCommit();

		return true;
	}
	return false;
}

void** init_device()
{
	static uintptr_t d_v_table[7] = { 0 };
	auto m_d_3d = Direct3DCreate9(D3D_SDK_VERSION);
	if (m_d_3d == nullptr)
		return nullptr;

	D3DPRESENT_PARAMETERS p_present_params;
	ZeroMemory(&p_present_params, sizeof(p_present_params));
	p_present_params.Windowed = true;
	p_present_params.BackBufferFormat = D3DFMT_UNKNOWN;
	p_present_params.SwapEffect = D3DSWAPEFFECT_DISCARD;

	LPDIRECT3DDEVICE9 p_device = nullptr;
	if (FAILED(m_d_3d->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		GetDesktopWindow(),
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&p_present_params,
		&p_device)))
		return nullptr;
	auto* v_table = reinterpret_cast<uintptr_t*>(p_device);
	v_table = reinterpret_cast<uintptr_t*>(v_table[0]);
	d_v_table[0] = v_table[16];		//Reset
	d_v_table[1] = v_table[17];		//Present
	d_v_table[2] = v_table[32];		//GetRenderTargetData
	d_v_table[3] = v_table[36];		//CreateOffscreenPlainSurface
	d_v_table[4] = v_table[65];		//SetTexture
	d_v_table[5] = v_table[100];	//SetStreamSource
	d_v_table[6] = v_table[42];		//SetStreamSource
	p_device->Release();
	m_d_3d->Release();
	return reinterpret_cast<void**>(d_v_table);
}
HRESULT __stdcall present_detour(IDirect3DDevice9* p_device, RECT* p_source_rect, RECT* p_dest_rect, const HWND h_dest_window_override, RGNDATA* p_dirty_region)
{
	reinterpret_cast<void(__stdcall*)(IDirect3DDevice9*)>(render_frame)(p_device);
	return reinterpret_cast<decltype(present_detour)*>(o_present)(p_device, p_source_rect, p_dest_rect, h_dest_window_override, p_dirty_region);
}