#pragma once
#include <windows.h>
#include <fstream>
#include <string>
#include <vector>
#include <d3d9.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "winmm.lib")
#include "../Detour4.0/include/detours.h"

namespace dx9
{
	bool set_frame_render(void* render);
}

