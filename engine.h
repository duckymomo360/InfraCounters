/*
* Source Engine globals.
*/

#pragma once

#include <Windows.h>

const auto hEngine = GetModuleHandleA( "engine.dll" );

inline bool IsInMainMenu()
{
	return *reinterpret_cast<bool*>(reinterpret_cast<uintptr_t>(hEngine) + 0x63FD86);
}

inline bool IsLoadingScreenVisible()
{
	return *reinterpret_cast<bool*>(reinterpret_cast<uintptr_t>(hEngine) + 0x5F9269);
}
