#include "stdafx.h"
#include "hud_messaging.h"

#include "Blam/Engine/interface/hud.h"
#include "Util/Hooks/Hook.h"

// Pointer to the crosshair and text scale for the below hook
float* p_text_scale_factor;

__declspec(naked) void ui_hud_left_messaging_top_scale()
{
	__asm
	{
		// sp: 1938h
		// mov     dl, [esp + 27h]
		// add     esp, 12

		fild[esp + 18h]
		push eax
		mov eax, [p_text_scale_factor]
		fmul[eax]
		pop eax
		fisttp[esp + 18h]

		// original code
		mov     ecx, ebx
		imul    ecx, 4E0h
		retn
	}
}

void hud_messaging_apply_hooks()
{
	p_text_scale_factor = get_secondary_hud_scale();
	Codecave(Memory::GetAddress(0x22D29E), ui_hud_left_messaging_top_scale, 3);
}