#ifndef NETCLASS_H
#define NETCLASS_H

#include "gmnet.h"
#include "Place.h"
#include "Transition.h"
#include "MeasureObj.h"

// RESTART caused includes
#include "Token_int.h"
#include "Token_bool.h"
#include "Token_real.h"
#include "Token_string.h"
#include "Token_DateTime.h"

class NetClass
{
public:
   NetClass();
   virtual ~NetClass();

   // virtual functions which are implemented in buildnet.cpp
   virtual void buildNet() = 0;
   virtual void createTokens() = 0;

   inline void setSimulation(Simulation* simu) { sim = simu; };
   inline Simulation* getSimulation() const { return sim; };
   inline const PlaceMap& getPlaces() const { return placeByName; };
   inline const TransitionMap& getTransitions() const { return transitionByName; };

   void addPlace(Place*);
   void addTransition(Transition*);
   void addMeasure(MeasureObj*);

   void linkPlaceToTransition(const char*, const char*,const int);
   void linkTransitionToPlace(const char*, const char*,const int);
   void linkIntoTransition(Place* src, Transition* dest,const int multiplicity);
   void linkOutOfTransition(Transition* src, Place* dest,const int multiplicity);

   Place* getPlace(const char*);
   Place* getPlace(const string&);
   Place* getPlace(int id);
   Place* getPlace(PlaceIdentMap* places, const char* name);
   Transition& getTransition(const char*);
   Transition* getTransition(int id);
   const MeasureList &getMeasureList () const {return measureList;}

   string writeMaps();

   void restoreNet(int mode);
   void storeMeasureList();
   void loadMeasureList(int mode);
   inline void clearMaps() {storeMeasureList();
   							placeByName.clear();
   							transitionByName.clear();
   							measureList.clear();}

   inline MeasureList& getMeasureList() {return measureList;}

   // SMARTPTR(Token) copyToken(SMARTPTR(Token) oldToken);

   virtual SMARTPTR(Token) copyToken(SMARTPTR(Token) oldToken);

   // SMARTPTR(Token) upcastToken(SMARTPTR(Token) oldToken);

   virtual SMARTPTR(Token) upcastToken(SMARTPTR(Token) oldToken);


private: // Private attributes
   PlaceMap placeByName;
   TransitionMap transitionByName;
   MeasureList measureList, s_measureList;
   Simulation* sim;
};

#endif
