#pragma once

#ifndef _DS1SINGLETON_STATIC_H
#define _DS1SINGLETON_STATIC_H

#include "DS1Singleton.h"

namespace Singleton
{
	template < class T >
	class DS1Static : public DS1Singleton
	{
	protected:

		DS1Static() {};

	public:

		static T& Instantiate()
		{
			static T kInstance;
			return kInstance;
		}
	};
};

#endif // #ifndef _DS1SINGLETON_STATIC_H