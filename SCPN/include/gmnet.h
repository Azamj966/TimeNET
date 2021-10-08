#ifndef GMNET_H
#define GMNET_H

#include "MSVCC.h"

//#define USE_BOEHM_GC
#define USE_SMARTPTR
//#define USE_BOOST_SMARTPTR

/* ### External Smart Pointer / Garbage Collection package ###*/
#ifdef USE_SMARTPTR
/* Use smart pointers.
Does not work with cyclic references.
Objects within the cycle will never be deleted */
#include "SmartPointers.h"
#define SMARTPTR(type) Ptr<type >
#define UPCAST_MACRO(exp) Dynamic_Cast< exp >
#define NEW_MACRO(basetype, rvalue) new rvalue
#define GETRAW_MACRO( smartptr ) (smartptr).GetRawPtr()
#define NULL_PTR(type) Ptr<type >(0)
#endif

#ifdef USE_BOOST_SMARTPTR
/* Use smart pointers.
Does not work with cyclic references.
Objects within the cycle will never be deleted */
#include <boost/shared_ptr.hpp>
using namespace boost;
#define SMARTPTR(type) shared_ptr<type >
#define UPCAST_MACRO(exp) shared_dynamic_cast< exp >
#define NEW_MACRO(basetype, rvalue) SMARTPTR(basetype)(new rvalue )
#define GETRAW_MACRO( smartptr ) (smartptr).get()
#define NULL_PTR(type) shared_ptr<type >()
#endif

#ifdef USE_BOEHM_GC
/* Use the Boehm garbage collector
 This includes the garbage collection class gc_cleanup*/
#include "gc_cpp.h"
#include "gc_allocator.h"
#endif

#if ! defined(USE_SMARTPTR) && ! defined(USE_BOOST_SMARTPTR)  \
   || defined(USE_BOEHM_GC)
/* Raw pointers.
   Used with the Boehm gc or for no collection at all
 */
#define SMARTPTR(type) type*
#define UPCAST_MACRO(exp) dynamic_cast< exp* >
#define NEW_MACRO(basetype, rvalue) new rvalue
#define GETRAW_MACRO( ptr ) ptr
#define NULL_PTR(type) NULL
#endif


/* SHORTCUTS for LOGGING-EXPRESSIONS */
#define TAB "\t"
#define NOW Simulation::getSimulation()->getCurrentSimulationTime()
//...

/* ### STL ### */
#include <map>
#include <set>
#include <list>
#include <vector>
#include <string>
using namespace std;

/*### IO ### */
#include <fstream>

/* ### TESTING ### */
#include <assert.h>

/* ### type for seconds ### */
typedef double Seconds_T;

/* -- Forward Declarations -- */
class Place;
class Token;
class Transition;
class EventWrapper;
class MeasureObj;
class Binding;
class SimSavedState;

/* typedefs of STL Containers */

#ifdef _MSC_VER
//Microsoft VC++ version of standard allocator
#define STDALLOC(type) std::allocator<type >
#define MAPSTDALLOC(keytype, datatype) std::allocator<pair<const keytype, datatype > >
#else
  #if __GNUC__ <= 2
  //allocator for use with gcc2.9x
  #define STDALLOC(type) std::alloc
  #define MAPSTDALLOC(keytype, datatype) std::alloc
  #else
    #if __GNUC__ == 3 && __GNUC_MINOR__ <= 3 && __GNUC_PATCHLEVEL__ <= 2
    //allocator for use with gcc 3.0.0 till gcc3.3.2
    #define STDALLOC(type) std::__alloc
    #define MAPSTDALLOC(keytype, datatype) std::__alloc
    #else
    //allocator for use with gcc3.3.3 or higher
    #define STDALLOC(type) std::allocator<type >
    #define MAPSTDALLOC(keytype, datatype) std::allocator<pair<const keytype, datatype > >
    #endif
  #endif
#endif


#ifdef USE_BOEHM_GC
	/*use special allocator to track pointers
	to garbage collected objects.*/
	#define GCALLOC(type) gc_allocator<type >
	#define MAPGCALLOC(keytype, datatype) gc_allocator< std::pair<const keytype, datatype > >
#else //no boehm_gc
	#define GCALLOC(type) STDALLOC(type)
	#define MAPGCALLOC(keytype, datatype) MAPSTDALLOC(keytype, datatype)
#endif

typedef list<Place*,  GCALLOC(Place*) > PlaceList;
typedef map<string, Place*, less<string>, MAPGCALLOC(string, Place*) > PlaceMap;
typedef set<Place*, less<Place*>, GCALLOC(Place*) > PlaceSet;
typedef map<int, int, less<int>, MAPGCALLOC(int, int) > TransitionMultiplicityMap; //maps transition-ID to Multiplicity

typedef list<Transition*, GCALLOC(Transition*) > TransitionList;
typedef set<Transition*, less<Transition*>, GCALLOC(Transition*) > TransitionSet;
typedef map<string, Transition*, less<string>, MAPGCALLOC(string, Transition*) > TransitionMap;
typedef list<SMARTPTR(Token), GCALLOC(SMARTPTR(Token)) > TokenList;
typedef list<TokenList*, GCALLOC(TokenList*) > TempBindingList;
typedef list<Binding*, GCALLOC(Binding*) > BindingList;

typedef vector<MeasureObj*, GCALLOC(MeasureObj*) > MeasureVector;
typedef set<MeasureObj*, less<MeasureObj*>, GCALLOC(MeasureObj*) > MeasureSet;
typedef list<MeasureObj*, GCALLOC(MeasureObj*) > MeasureList;

/* maps string to string for places and its real names */
typedef map<string, string, less<string>, MAPGCALLOC(string, string) > PlaceIdentMap;

/* maps string to string for attributes */
typedef map<string, string, less<string>, MAPGCALLOC(string, string) > AttributeMap;

/* maps placename to int for number of tokens added/removed */
typedef map<string, int, less<string>, MAPGCALLOC(string, int) > PlaceContentDeltaMap;

// RESTART types
typedef vector<SimSavedState> vector_savedStates;
typedef map<int, SMARTPTR(Token), less<int>, MAPGCALLOC(int, SMARTPTR(Token))> map_tokens;
typedef vector<int> vector_tokensID;

#endif
