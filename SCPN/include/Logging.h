#ifndef LOGGING_H
#define LOGGING_H

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class Logging
{
public:
   Logging();
   ~Logging(void);

   void open();
   void close();

   void setLogFolder(const string& logfolder);
   void setLogIndex(int index);

   inline bool isEventLogging() const { return eventlogging; };
   inline bool isSimLogging() const { return simlogging; };
   inline bool isTransLogging() const { return translogging; };
   inline bool isTokenLogging() const { return tokenlogging; };
   inline bool isServerLogging() const { return serverlogging; };
   inline bool isClientLogging() const { return clientlogging; };

   inline string getLogDir() const { return folder; };
   inline fstream* getEventLog() { return &eventlog; };
   inline fstream* getSimLog() { return &simlog; };
   inline fstream* getTransLog() { return &translog; };
   inline fstream* getTokenLog() { return &tokenlog; };
   inline fstream* getServerLog() { return &serverlog; };
   inline fstream* getClientLog() { return &clientlog; };

protected:
   string folder;
   string logindex;

   bool simlogging;
   bool eventlogging;
   bool translogging;
   bool tokenlogging;
   bool serverlogging;
   bool clientlogging;
   fstream simlog;
   fstream eventlog;
   fstream translog;
   fstream tokenlog;
   fstream serverlog;
   fstream clientlog;

   bool openlog(string logname, fstream* log);
   void closelog(bool* logging, fstream* log);
};

#endif
