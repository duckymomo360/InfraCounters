/*
* Interfaces from the Source Engine.
*/

#pragma once

#include <Windows.h>

template <typename T>
T* GetInterface( const char* szModuleName, const char* szInterfaceName )
{
    const auto hModule = GetModuleHandleA( szModuleName );

    if (!hModule)
        return nullptr;

    const auto pFuncAddress = GetProcAddress( hModule, "CreateInterface" );

    if (!pFuncAddress)
        return nullptr;

    using Fn = T * (*)(const char* pName, int* pReturnCode);
    const auto CreateInterface = reinterpret_cast<Fn>(pFuncAddress);

    return CreateInterface( szInterfaceName, nullptr );
}

typedef void IClientNetworkable;
typedef void IClientUnknown;
typedef void IClientEntity;

typedef unsigned long CBaseHandle;

class IClientEntityList
{
public:
    // Get IClientNetworkable interface for specified entity
    virtual IClientNetworkable* GetClientNetworkable( int entnum ) = 0;
    virtual IClientNetworkable* GetClientNetworkableFromHandle( CBaseHandle hEnt ) = 0;
    virtual IClientUnknown* GetClientUnknownFromHandle( CBaseHandle hEnt ) = 0;

    // NOTE: This function is only a convenience wrapper.
    // It returns GetClientNetworkable( entnum )->GetIClientEntity().
    virtual IClientEntity* GetClientEntity( int entnum ) = 0;
    virtual IClientEntity* GetClientEntityFromHandle( CBaseHandle hEnt ) = 0;

    // Returns number of entities currently in use
    virtual int					NumberOfEntities( bool bIncludeNonNetworkable ) = 0;

    // Returns highest index actually used
    virtual int					GetHighestEntityIndex( void ) = 0;

    // Sizes entity list to specified size
    virtual void				SetMaxEntities( int maxents ) = 0;
    virtual int					GetMaxEntities() = 0;
};
