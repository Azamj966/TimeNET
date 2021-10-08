/* Generated by Together */

#ifndef TRANSITION_H
#define TRANSITION_H

#include "DefaultGM.h"
#include "Simulation.h"
#include "Token.h"
#include "Binding.h"
#include "Delay.h"
#include "NetClass.h"
#include "RandomWrapper.h"

// New includes for RESTART
#include "BindingDummy.h"


class Transition : public DefaultGM
{
public:
   /**
    * Constructors of this transition.
    */
   Transition(int uniqueid,
              const char* name,
              NetClass* net,
              const AttributeMap& attributes,
	      const PlaceIdentMap& placeidents,
	      string PagePath,
	      double firingWeight = 0,
	      int prio = 0,
	      bool exp = false,
	      bool takeFirst = true,
	      bool isInfinite = false,
	      bool hasLocalGuard = false);


   Transition(int uniqueid,
              const char* name,
              NetClass* net,
	      const PlaceIdentMap& placeidents,
	      string PagePath,
	      double firingWeight = 0,
	      int prio = 0,
	      bool exp = false,
	      bool takeFirst = true,
	      bool isInfinite = false,
	      bool hasLocalGuard = false);


   /**
    * virtual Destructor
    */
   virtual ~Transition();

   /**
    * Returns the unique id of this place.
    */
   inline int getID() const { return id; }

   /**
    * Fires this transition with the given binding.
    * INVARIANT: after firing the eventlist is complete and accurate.
    */
   // void fire(Binding* binding);
   // RESTART Changes, parameter 'weight' was added
   void fire(Binding* binding, double weight);

   /**
    * Will be called from the simulation in tokengame mode (if this transition was manually
    * selected for firing) and returns the next possible binding of this transition.
    */
   Binding* getNextEvent() const;
   EventList& getEvents() { return bindings; }

   /**
    * This method is called once at the beginning of the simulation and initializes
    * the container "affectedTransitions", collects the measures of all neighbor places,
    * initializes the global guard and searches for a possible configuration (binding)
    * which is added to the event list immediatly.
    */
   virtual void startup();

   inline virtual int getPriority() const { return priority; };
   inline virtual Seconds_T getFiringDelay(TokenList &binding) const { return (long)0; };
   inline virtual double getFiringWeight() const { return firingWeight; };
   inline virtual int getFireCount() const { return fireCount; };

   inline PlaceList const& getInPlaces() const { return inPlaces; };
   inline PlaceList const& getOutPlaces() const { return outPlaces; };

   inline virtual void setDisplaying(bool disp) { displaying = disp; };
   inline virtual void setWatch(bool wtch) { watch = wtch; };
   inline virtual void setImmediate(bool imm) { immediate = imm; };
   virtual void setLogfile(string filename, string description);

   void addInPlace(Place* const place);
   void addOutPlace(Place* const place);

   void addMeasure(MeasureObj* measure);

   void addNotBoundToken(SMARTPTR(Token) token);
   void removeNotBoundToken(SMARTPTR(Token) token);
   void addBinding(Binding* binding);
   void removeBinding(Binding* binding, bool notifyTransitions);

   /**
    * Inserts all affected transitions into the list "affectedTransitions".
    */
   void updateAffectedTransitionList();

   /**
    * This is called from a bound token (a token that is in a binding of this transition)
    * to inform this transition that it was fired from another transition and
    * has to be removed from the binding of this transition.
    * PRECOND:
    *	 - token is in the current binding of this transition
    * INVARIANT: After a call to this method this transition has no binding anymore
    */
   void tokenNotify(SMARTPTR(Token) token);

   /**
    * This method is called if an other transition has fired and this transition is affected
    * and could be enabled. This transition has to search for a new binding.
    * INVARIANT: possibly new bindings of this transitions are in the eventlist
    */
   void couldEnableNotify();

   /**
    * This method is called if an other transition has fired and this transition is affected
    * and could be disabled. This transition has to search if there is currently enough
    * space in the out-places. If not, then the transition have to remove all bindings from
    * the eventlist.
    * INVARIANT: no changes if there is enough space or all bindings removed from eventlist
    */
   void couldDisableNotify();

   /**
   * This method is called if the number of tokens in a place has changed and
   * the place is in the "affectedPlaces" list of this transition.
   * This methode will only be called from places with global guards.
    */
   void placeNotify();

   /**
    * This method returns whether the Transition has got a LocalGuard or not
     */
    bool hasLocalGuard(){return gotLocalGuard;};

   /**
    * Dumps all input and output places of this transition to the standard output (cout).
    */
   void printPlaces();

   friend ostream& operator << (ostream& strm, const Transition & X);

   /**
   * Returns the seconds when the transition is able to fire,
   * based on the current simulation time.
   * This method will be overwritten by transitions with a
   * manual defined timeguard function.
   */
   virtual Seconds_T Timeguard(const DateTime& now) { return 0.0; };

   inline virtual bool hasTimeguard() const { return false; };

   /*** Timeguard-Methods ***/
   void addTimeguardBinding(const DateTime& time);
   bool convertTimeguardBinding(Binding* tempBinding);

   // RESTART Methods
   void removeCurrentBindings();
   void addRestoredBinding(Binding *restoredBinding);

   const TokenList& getNotBoundTokens() const { return notboundTokens; };
   void addRestoredNotBoundToken(SMARTPTR(Token) token);
   void removeCurrentNotBoundTokens();

   virtual void updateEventList();

   TransitionList getCouldEnableTransitions(){return couldEnableTransitions;};
   void setCouldEnableTransitions(TransitionList newCouldEnableTransitions)
   {
	   couldEnableTransitions = newCouldEnableTransitions;
   };

   TransitionList getCouldDisableTransitions(){return couldDisableTransitions;};
   void setCouldDisableTransitions(TransitionList newCouldDisableTransitions)
   {
	   couldDisableTransitions = newCouldDisableTransitions;
   };

   bool getInEventList(){return inEventList;};
   void setInEventList(bool savedInEventList){inEventList = savedInEventList;};


   static long globalFireCount;

protected:
   Delay delay;
   RandomWrapper bindRandGen;

/************************* CONTAINER *******************************/

   /**
    * All incoming and outgoing places.
    */
   PlaceList inPlaces;
   PlaceList outPlaces;

   /**
    * These transition are affected if this transition fires.
    */
   TransitionList couldEnableTransitions;
   TransitionList couldDisableTransitions;

   PlaceList affectedPlaces;

   const PlaceIdentMap places;

   /**
    * all tokens in the inPlaces which are not bound in a current binding
    * only for IS transitions and only for optimization
    */
   TokenList notboundTokens;

   /**
    * Current sorted list of bindings of this transition.
    * Only one binding is stored if the transition is ES.
    * All bindings are stored if the transition is IS.
    */
   EventList bindings;

   /**
    * List with all measure objects which are affected if this transition fires.
    */
   MeasureSet measures;

/********************** ATTRIBUTES ********************************/
   int fireCount;
   bool inEventList;
   bool optimizingTimeguard;
   bool isInfinite;
   bool expo;
   bool watch;
   bool firing;   // true if this transition is firing at the moment
   int priority;
   double firingWeight;
   bool takeFirst;
   bool immediate;
   bool displaying;
   bool translogging;  // if true the transition logs each firing (depending on the log-expression)
   fstream translogfile;
   string translogfilename;
   int id;
   bool gotLocalGuard;

   string pagePath;

   virtual void log(TokenList &binding) {};
   virtual Seconds_T display(TokenList &binding) { return 0.0; };

/******************************* METHODS *****************************/

   /**
    * Notifies all transitions of the "affectedTransition" list that something
    * has changed, because this transition has fired.
    */
   void notifyTransitions();


   /**
    * Return number of maximal possible bindings in an IS Transition
    */
   int getEnablingDegree();

   /**
    * Searches for a new possible binding for this transition.
    * This binding will be automatically added to the eventlist.
    * If this transition is a IS transition then all possible bindings
    * are added to the eventlist.
    */
   void searchConfiguration();

   /**
    * Tests if this transition has enough space in all out-places and therefore if this
    * transition can fire.
    */
   virtual bool checkSpace() const;

   /**
    * generate a list of all bindings
    */
   virtual TempBindingList* generateBindingList() = 0;

   void clearTempBindingList(TempBindingList* bindings);

   TempBindingList* getAvailableBindingList();

   TokenList* extractRandomBinding(TempBindingList* bindings);

   void removeRandomBindingFromEventList();

   /**
    * the global guard
    */
   virtual bool globalGuard() { return true; };

   /*
    * Example:
    * net->getPlace("PLACE_A")->addGlobalGuardTransition(this);
    * net->getPlace("PLACE_B")->addGlobalGuardTransition(this);
    */
   virtual void initGlobalGuards() {};

   /**
    * Removes the tokens from the inPlaces and sets fired tokens to the outPlaces.
    * If one of the outPlacec is not stored on the current node, all new tokens
    * for this place are added to the list tokensToSend.
    */
   virtual void moveTokens(TokenList &binding) = 0;

   /**
    * Removes all bindings of this transition from the event list
    */
   void removeAllBindingsFromEventList();

   inline void setTimeguardOptimization(bool optimize) { optimizingTimeguard = optimize; };
   bool isOptimizingTimeguardTransition() const;

   /**
    * Returns a list of all tokens of the in-places.
    */
   const TokenList& getPlaceMarking(Place& p) const;

   void notifyMeasures(void);

   // Parameter 'weight' was added
   // void accumMeasures(const Binding *binding);
   void accumMeasures(const Binding *binding, double weight);

   void notifyPlaceWatch(void);
};

#endif