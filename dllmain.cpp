#include "dllmain.h"

#include <Windows.h>
#include <cstdio>
#include <unordered_set>
#include <imgui.h>

#include "kiero/kiero.h"
#include "kiero/d3d9_impl.h"

#include "interfaces.h"
#include "engine.h"
#include "c_target.h"
#include "cstring_helpers.h"

IClientEntityList* pClientEntityList = GetInterface<IClientEntityList>( "client.dll", "VClientEntityList003" );

int targetsMax[E_TargetType::NONE] = { 0 };
int targetsMin[E_TargetType::NONE] = { 0 };

void CountTargets()
{
    // Reset counter lists
    memset( targetsMax, 0, sizeof( targetsMax ) );
    memset( targetsMin, 0, sizeof( targetsMin ) );

    std::unordered_set<const char*, cstr_hash, cstr_equal> names;

    for (int i = 0; i <= pClientEntityList->GetHighestEntityIndex(); i++)
    {
        IClientEntity* pEntity = pClientEntityList->GetClientEntity( i );
        if (pEntity == nullptr)
            continue;
        
        C_Target pTarget( pEntity );
        E_TargetType eType = pTarget.GetTargetType();

        // If it's not a target, just some other entity, skip it.
        if (eType == E_TargetType::NONE)
            continue;

        // If it's not possible to obtain.
        if (!pTarget.IsObtainable())
            continue;

        const char* szName = pTarget.GetName();

        // The game groups target entities by name.
        // Targets without a name aren't grouped.
        if (strlen( szName ) == 0 || !names.contains( szName ))
        {
            targetsMax[eType]++;

            if (pTarget.IsCompleted())
                targetsMin[eType]++;

            names.insert( szName );
        }
    }
}

inline const ImVec4 GreenIf( bool bGreen )
{
    return bGreen
        ? ImVec4( 0.0f, 1.0f, 0.0f, 1.0f )
        : ImVec4( 1.0f, 1.0f, 1.0f, 1.0f );
}

void DrawTextForStat( E_TargetType eType, const char* szName )
{
    if (targetsMax[eType] <= 0)
        return;

    ImGui::TextColored(
        GreenIf( targetsMin[eType] >= targetsMax[eType] ),
        szName);

    ImGui::SameLine( 110.0F );

    ImGui::TextColored(
        GreenIf( targetsMin[eType] >= targetsMax[eType] ),
        "%d / %d",
        targetsMin[eType],
        targetsMax[eType] );
}

void ConfigureImGui()
{
    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = NULL;
    io.Fonts->AddFontFromFileTTF( "C:/Windows/Fonts/Arial.ttf", 16 );
}

void DrawImGui()
{
    if (IsInMainMenu() || IsLoadingScreenVisible())
        return;

    CountTargets();

    ImGui::SetNextWindowBgAlpha( 0.1f );

    ImGui::Begin( "Infra Counters", nullptr,
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoSavedSettings |
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_NoScrollWithMouse );

    // Stats
    DrawTextForStat( INFRA_CAMERA_TARGET, "Pictures:" );
    DrawTextForStat( INFRA_CORRUPTION_TARGET, "Documents:" );
    DrawTextForStat( INFRA_REPAIR_TARGET, "Repairs:" );
    DrawTextForStat( INFRA_WATER_FLOW_METER_TARGET, "Flow Meters:" );

    // Geocaches
    // We use Max to represent remaining Geocaches because they get deleted once collected.
    ImGui::TextColored( GreenIf( targetsMax[INFRA_GEOCACHE] <= 0 ), "Geocaches:" );
    ImGui::SameLine( 110.0F );
    ImGui::TextColored( GreenIf( targetsMax[INFRA_GEOCACHE] <= 0 ), "%d left", targetsMax[INFRA_GEOCACHE] );

    // Mistakes
    // Only show if you messed up.
    if (targetsMin[INFRA_MISTAKE_TARGET] > 0)
    {
        ImGui::TextColored( ImVec4( 1.0f, 0.0f, 0.0f, 1.0f ), "Mistakes:" );
        ImGui::SameLine( 110.0F );
        ImGui::TextColored( ImVec4( 1.0f, 0.0f, 0.0f, 1.0f ), "%d", targetsMin[INFRA_MISTAKE_TARGET] );
    }

    ImGui::End();
}

void WINAPI MainThread( HMODULE hModule )
{
    if (kiero::init( kiero::RenderType::D3D9 ) == kiero::Status::Success)
    {
        impl::d3d9::init();
    }
}

BOOL APIENTRY DllMain( HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)MainThread, hModule, 0, NULL );
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

