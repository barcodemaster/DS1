#include "DS1Singleton.h"

std::atomic< bool > DS1Singleton::m_bCreated;

bool DS1Singleton::IsCreated()
{
	return m_bCreated;
}

DS1Singleton::DS1Singleton()
{
	m_bCreated = true;
}

DS1Singleton::~DS1Singleton()
{
	m_bCreated = false;
}
