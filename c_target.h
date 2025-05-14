/*
* Helper class to read memory from Infra target entities.
*/

#pragma once

#include <cstdint>
#include <string.h>

#include "c_entity.h"

enum E_TargetType : char
{
	INFRA_CAMERA_TARGET,
	INFRA_REPAIR_TARGET,
	INFRA_MISTAKE_TARGET,
	INFRA_CORRUPTION_TARGET,
	INFRA_WATER_FLOW_METER_TARGET,
	INFRA_GEOCACHE,
	NONE,
};

struct S_BrokenTarget
{
	E_TargetType eType;
	const char* szName;
};

// Targets that are unobtainable.
const S_BrokenTarget brokenTargets[] = {
	{ INFRA_CORRUPTION_TARGET, "berg_tape_corruption_1" } // infra_c7_m2_bunker - Unused projector cutscene corruption target that has no inputs.
};

class C_Target : public C_Entity
{
protected:
	E_TargetType m_eTargetType;

public:
	C_Target( void* pEnt ) : C_Entity( pEnt )
	{
		const char* szClassName = GetClass();

		if (strcmp( szClassName, "infra_camera_target" ) == 0)
			m_eTargetType = INFRA_CAMERA_TARGET;
		else if (strcmp( szClassName, "infra_repair_target" ) == 0)
			m_eTargetType = INFRA_REPAIR_TARGET;
		else if (strcmp( szClassName, "infra_mistake_target" ) == 0)
			m_eTargetType = INFRA_MISTAKE_TARGET;
		else if (strcmp( szClassName, "infra_corruption_target" ) == 0)
			m_eTargetType = INFRA_CORRUPTION_TARGET;
		else if (strcmp( szClassName, "infra_water_flow_meter_target" ) == 0)
			m_eTargetType = INFRA_WATER_FLOW_METER_TARGET;
		else if (strcmp( szClassName, "infra_geocache" ) == 0)
			m_eTargetType = INFRA_GEOCACHE; // BUG: Geocaches don't appear on the list until you get close enough to them once. I think this is because the engine is culling them.
		else
			m_eTargetType = NONE;
	}

	// Some are unobtainable for some reason.
	const bool IsObtainable() const
	{
		const char* szName = GetName();

		for (auto target : brokenTargets)
		{
			if (m_eTargetType == target.eType && strcmp( szName, target.szName ) == 0)
			{
				return false;
			}
		}

		return true;
	}

	const bool IsCompleted() const
	{
		switch (m_eTargetType)
		{
		case INFRA_CAMERA_TARGET:
		case INFRA_REPAIR_TARGET:
		case INFRA_MISTAKE_TARGET:
		case INFRA_WATER_FLOW_METER_TARGET:
			return *reinterpret_cast<const bool*>(m_pEnt + 0xA80);
		case INFRA_CORRUPTION_TARGET:
			return *reinterpret_cast<const bool*>(m_pEnt + 0xA88);
		case INFRA_GEOCACHE:
			return false; // Geocaches delete themselves once they're collected. If they exist, then they haven't been collected.
		default:
			return false;
		}
	}

	inline const E_TargetType GetTargetType() const
	{
		return m_eTargetType;
	}
};

