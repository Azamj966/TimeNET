#ifndef MAINCLASS_H
#define MAINCLASS_H

#include "gmnet.h"
#include "DateTime.h"
#include "Logging.h"

class MainClass
{
 public:
   MainClass();
   ~MainClass();

   int main(int, char*[]);

   inline static Logging* getLog() { return &log; };

 protected:
   static Logging log;
};

#endif
