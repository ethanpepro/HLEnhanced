#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "Weapons.h"

#include "CMap.h"

CMap* CMap::m_pInstance = nullptr;

CMap* CMap::CreateInstance()
{
	ASSERT( !m_pInstance );

	m_pInstance = new CMap();

	m_pInstance->Create();

	return m_pInstance;
}

CMap* CMap::GetInstance()
{
	ASSERT( m_pInstance );

	return m_pInstance;
}

void CMap::DestroyInstance()
{
	ASSERT( m_pInstance );

	delete m_pInstance;

	m_pInstance = nullptr;
}

bool CMap::Exists()
{
	return m_pInstance != nullptr;
}

CMap* CMap::CreateIfNeeded()
{
	if( !Exists() )
		CreateInstance();

	return GetInstance();
}

CMap::CMap()
{
}

CMap::~CMap()
{
}

void CMap::Create()
{
	//Ammo types have to be registered ahead of time in all cases.
	RegisterAmmoTypes();
}