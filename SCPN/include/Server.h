#ifndef SERVER_H
#define SERVER_H

#include "SimTokenGame.h"
#include "SimPerformance.h"
#include "ImplementedNetClass.h"

/*
 * PDV-Project 2007
 *
 *
 *
 * Author:			Enrico Stephan, Alexej Tuchscherer
 *
 * Date:			December 2007 - February 2008
 * */
class Server
{
public:
   Server();
   ~Server(void);

   inline const NetClass* getNet() const { return &net; };
   inline Simulation* getSim() const { return sim; };

   void start(int argc, char *argv[]);

   double alphat_def;
protected:

   bool dateTime_format;
   // RESTART changes, sim_mode is now of type integer to have more
   // simulation modes
   int sim_mode;
   unsigned int secmax;
   int no_intervals;

   ImplementedNetClass net;
   Simulation* sim;
};

#endif
