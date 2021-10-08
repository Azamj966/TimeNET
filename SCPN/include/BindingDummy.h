#ifndef BINDING_DUMMY_H
#define BINDING_DUMMY_H

#include "gmnet.h"
#include "Token.h"

// This class was created for RESTART to save the most
// properties of a Binding in it
// The difference to Binding class is, that there is a list of token ids
// instead of a token list
class BindingDummy
{
public:
	BindingDummy(long ID, Transition *boundTranstion, DateTime time, int priority,
			     double weight, bool fireEvent, vector_tokensID tokensID);
   ~BindingDummy();

   vector_tokensID getTokensID() { return tokensID; };
   Transition* getBoundTransition() { return boundTransition; };
   DateTime getTime() { return time; };
   int getPriority() { return priority; };
   long getID() { return ID; };
   double getWeight() { return weight; };
   bool isFireEvent() { return fireEvent; };

   bool operator<(const BindingDummy& binding2) const;
   bool operator== (const BindingDummy& binding) const;

private:
   Transition* boundTransition;

   DateTime time;
   int priority;
   long ID;
   double weight;
   bool fireEvent;
   vector_tokensID tokensID; // the vector of ids of tokens, which were saved in original binding
};

#endif
