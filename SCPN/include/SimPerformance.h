#ifndef PERFORMANCE_H
#define PERFORMANCE_H

#include "Simulation.h"

class Performance : public Simulation
{
public:
  Performance(NetClass* net, 
	   const std::string& netname,
	   const std::string& resultaddress, 
	   const DateTime &starttime, long i_max, long j_max);

   ~Performance(void);

   void run();
protected:
   long i_max;
   long j_max;
};

#endif
