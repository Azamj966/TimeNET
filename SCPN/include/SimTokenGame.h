#ifndef TOKENGAME_H
#define TOKENGAME_H


#include "Simulation.h"
#include "Breakpoint.h"



class TokenGame : public Simulation
{
public:
   TokenGame(NetClass* net,
	   const std::string& netname,
	   const std::string& resultaddress,
	   const DateTime &starttime);
   ~TokenGame(void);

   bool isSingleStep() const;
   void run();
   void shutdown();

protected:
   vector<string> couldFire();
   void fire(string transName);
   void writePlaceContent(socket_t* sock, string placename);
   void writeFireList(socket_t* sock, bool newlineSeparating);
   void writeChangedPlacesAndTransitions(socket_t* sock);
   void writeBinding(socket_t* socket, Binding* binding);

   void applyBreakpoints(string command);
   void writeBreakpoints(socket_t* socket);
   MeasureObj* findMeasure(string name);
   void split(string source, vector<string> *dest, string delimiter);

   BreakpointList *breakpoints;
};

#endif
