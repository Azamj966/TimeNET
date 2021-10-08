/***************************************************************************
                          DefaultGM.h  -  description
                             -------------------
    begin                : Fri Dec 7 2001
    copyright            : (C) 2001 by 
    email                : 
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef DEFAULTGM_H
#define DEFAULTGM_H

#include "gmnet.h"

class NetClass;

class DefaultGM
{
public:
   DefaultGM(NetClass* net);
   DefaultGM(const char* name, const char* type, NetClass* net);
   virtual ~DefaultGM();

   virtual bool operator < (const DefaultGM & X) const;
   virtual bool operator > (const DefaultGM & X) const;

   inline virtual const string& getName() const { return name; };
   inline virtual const string& getType() const { return type; };

   friend ostream& operator<< (ostream& strm, const DefaultGM & X);

protected:
   static int defaultCounter;
   string name;
   string type;
   NetClass* net;
};

#endif
