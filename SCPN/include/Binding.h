#ifndef BINDING_H
#define BINDING_H

#include "gmnet.h"
#include "Token.h"

#ifdef USE_BOEHM_GC
/*everything derived from this class will be
garbage collected*/
class Binding : public gc_cleanup
#else
class Binding
#endif
{
public:
   Binding(const DateTime& time);
   Binding(Transition* transition, const DateTime& time);
   Binding(TokenList* tokens, Transition* transition, const DateTime& time);

   // Constructor for RESTART
   Binding(long bindingID, Transition *bndTranstion, DateTime bndTime,
		   int bndPriority, double bndWeight, bool bndFireEvent,
		   TokenList *bndTokens);

   ~Binding(void);

   bool operator< (const Binding& binding2) const;
   bool operator== (const Binding& binding) const;
   Binding& operator= (const Binding& binding);
   friend ostream& operator<< (ostream& strm, const Binding& X);

   inline TokenList* getBoundTokens() const { return boundTokens; };
   inline Transition* getBoundTransition() const { return boundTransition; };
   inline const DateTime& getTime() const { return time; };
   inline int getPriority() const { return priority; };
   inline long getID() const { return ID; };
   inline double getWeight() const { return weight; };
   inline bool isFireEvent() const { return fireEvent; };
   inline bool isTempBinding() const { return (boundTokens == NULL); };
   inline void convertTempBinding(TokenList* tokens) { boundTokens = tokens; };

   /**
    * Sets the time of this Binding to:
    *   currentSimulationTime + firingDelay + TimeGuard()
    */
   void updateTime(const DateTime& time);

   /**
    * Executes this binding (which means firing).
    * This method is called from Simulation::fire() methods.
    */
   void executeEvent();

protected:
   TokenList* boundTokens;
   Transition* boundTransition;

   static long idCounter;

   DateTime time;
   int priority;
   long ID;
   double weight;
   bool fireEvent;

   void init(Transition* transition, const DateTime& time);
};

#endif
