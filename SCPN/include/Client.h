#ifndef CLIENT_H
#define CLIENT_H

#include "ImplementedNetClass.h"

class Client
{
 public:
   Client(void);
   ~Client(void);

   inline const NetClass* getNet() const { return &net; };

   void start(int argc, char *argv[]);

 protected:
   ImplementedNetClass net;
};

#endif
