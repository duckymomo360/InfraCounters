/*
* Helper class to read properties of Source Engine entities.
*/

#pragma once

class C_Entity
{
protected:
	uintptr_t m_pEnt;

public:
	C_Entity( void* pEnt ) : m_pEnt( reinterpret_cast<uintptr_t>(pEnt) )
	{
	}

	inline const char* GetClass() const
	{
		return reinterpret_cast<const char*>(m_pEnt + 0x24C);
	}

	inline const char* GetName() const
	{
		return reinterpret_cast<const char*>(m_pEnt + 0x148);
	}
};

