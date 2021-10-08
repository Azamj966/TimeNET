#ifndef DELAY_H
#define DELAY_H

#include "gmnet.h"
#include "RandomWrapper.h"
#include <math.h>

#define LOG log

class Delay
{
private:
	mutable RandomWrapper randGen;
public:
   Delay(void);
   ~Delay(void);

   Seconds_T Exp(double firingDelay) const;
   Seconds_T Det(double firingDelay) const;
   Seconds_T Uni(double a, double b) const;
   Seconds_T DUni(long a, long b) const;
   Seconds_T Triang(double a, double b) const;
   Seconds_T Norm(double a, double b) const;
   Seconds_T LogNorm(double a, double b) const;
   Seconds_T Wei(double a, double b) const;
};

#endif
