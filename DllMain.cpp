#include <Windows.h>
#include <iostream>

#include "UltimateProxyDLL.h"
#include "ModStuff.h"

using namespace mod_stuff;

DWORD WINAPI NewThread(LPVOID lpParam)
{
	uintptr_t address = aob_scan("56 8b f1 8b ? ? 85 c0 0f 84 ? ? ? ? 83 ? ? ? ? ? ? 0f");
	if (!address)
	{
		show_error_popup("Did not find signature!");
		return FALSE;
	}
	if (!replace_bytes_at_address(address, "56", "c3"))
	{
		show_error_popup("Failed to apply patch!");
		return FALSE;
	}
	return TRUE;
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	if (fdwReason == DLL_PROCESS_ATTACH)
	{
		try
		{
			upd::create_proxy_and_thread(hinstDLL, &NewThread);
		}
		catch (std::runtime_error e)
		{
			std::cout << e.what() << std::endl;
			return FALSE;
		}
	}
	return TRUE;
}