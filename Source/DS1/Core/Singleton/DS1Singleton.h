#pragma once

#ifndef _DS1SINGLETON_H
#define _DS1SINGLETON_H

#include <atomic>

class DS1_API DS1Singleton
{
public:
	static bool IsCreated();

protected:

	DS1Singleton();
	virtual ~DS1Singleton();

	static std::atomic< bool > m_bCreated;
};

#endif // #ifndef _DS1SINGLETON_H