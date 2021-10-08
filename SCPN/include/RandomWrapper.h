#ifndef RANDOMWRAPPER_H
#define RANDOMWRAPPER_H

/*
 * Use L´Ecuyers random number generator
 * http://www.iro.umontreal.ca/~lecuyer/myftp/streams00/c++/streams4.pdf
 */
#include "RngStream.h"

class RandomWrapper
{
private:
	RngStream rngstream;
public:
	// set the 6 internal seed values (not necessary, uses defaults if not set)
	// Changed 09.12.2016: return-value to bool (for consistency with the
	//		method in RngStream)
	static bool SetPackageSeed (const unsigned long seed[6])
	{
		return RngStream::SetPackageSeed(seed);
	}

	// return a double-precision floating point random number
	double randu01()
	{
		return rngstream.RandU01();
	}

	// return an int random number between i and j
	long randint(long i, long j)
	{
		return rngstream.RandInt ( i,  j);
	}
};

#endif
