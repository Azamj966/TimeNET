#ifndef SIMULATION_H
#define SIMULATION_H

#include <iostream>
#include "gmnet.h"
#include "task.h"
#include "sockio.h"
#include "EventList.h"
#include "DateTime.h"
#include "MainClass.h"
#include "RandomWrapper.h"
#include <fstream>
#include <ctime>

// RESTART includes
#include "SimSavedState.h"

class DefaultGM;
class Binding;
class NetClass;
class Server;

class Simulation
{
 public:
   static const int SUCCESSFUL=0;
   static const int SECMAX_TIMEOUT=1;
   static const int ENDTIME_TIMEOUT=2;

   // constants for simulation modes
   static const int TRANSIENT = 0;
   static const int STATIONARY = 1;
   static const int RESTART = 2;

   static const bool DATEFORMAT = false;
   static const bool NUMBERFORMAT = true;

   static const bool PER_RULE = true;
   static const int EPS_MAX = 5;
   static const int INT_LENGTH = 50;   // standard size of sampling interval (transient)
   static const int BATCH_SIZE = 100;  // number of samples for one batch (stationary)
   static const int MIN_BATCHES = 5;   // minimum number of batches/samples to collect before confidence interval calculation
   int no_sampling;  // number of sampling points for transient simulation (plot and estimation points); 1 for stationary?
   int loops; // number of finished simulations in transient simulation
   int statOutput;

   Simulation(NetClass* net,
	   const std::string& netname,
	   const std::string& resultaddress,
	   const DateTime& from,
	   const DateTime& to,
	   int no_sampling,
	   int sim_mode,
	   bool dateTime_format,
	   bool percent_rule,
	   double eps_max,
	   double interval_length,
	   unsigned int secmax);

   virtual ~Simulation();

   void displayOut(DefaultGM* object, double value);

   inline const DateTime& getCurrentSimulationTime() const { return current_time; };
   static Simulation *getSimulation() {return sim;}

   /** Fills the event list with all enabled transitions and timeguard events */
   virtual void init();
   void initMeasureList( bool first_running );
   void initTransitions();

   inline virtual bool isSingleStep() const { return false; };

   void measureOut(DefaultGM* object, double value);
   void measureOut(DefaultGM* object, MeasureObj *m_object);

   void resetCurrentTime();
   virtual Binding* resolve_conflicts(BindingList& events);
   virtual void run_transient( double &alphat_def );
   virtual void run_stationary( double &alphat_def );

   // call for RESTART simulation
   virtual void run_restart( double &alphat_def, int restart_repeats, double thr_min, double thr_max, int thr_number);

   /** writes end results of all measures to a file */
   virtual void shutdown();

   // Test of RESTART
   void testState();

protected:
   time_t start_seconds;   // real time of simulation start
   time_t lastEpsilonTime; // last real time the relative error has been printed
   double alphat_def;

   bool dateTime_format, percent_rule;
   bool no_endtime; // flag stating that no model stop time has been set
   bool firstValue; // mark that this is the first time value
   bool no_secmax;  // flag stating that NO max CPU time has been specified
   unsigned int secmax;  // max CPU time seconds
   int sim_mode;    // mode enumeration: Transient / Stationary / RESTART
   double restartSimTime;

   double eps_max;
   double interval_length; // length in model time between two sampling points
   fstream simlog; //file for data-output

   static string printEventList(BindingList& bindings);

   void getFirstBindingsOfAllTransitions(BindingList* bindings, bool disregardTime);
   void executeEvent(Binding* binding);

   void updateSimulationTime(const DateTime &eventTime);
   void updateAllMeasures( Binding *fireBinding, bool accumulate, bool slt_et );
   bool updateSamplingPoints( bool analysis );

   inline bool hasMeasureSocket() const { return (measureSocket != NULL); };
   inline socket_t* getMeasureSocket() { return measureSocket; };

   virtual void initMeasureSocket();

   NetClass* net;
   std::string netname;
   std::string resultaddress;
   //event_time is the time of the next event
   //current_time is the current simulation time
   //sampling_time is the time of the next sampling point
   DateTime current_time, sampling_time, event_time;
   const DateTime& start_time, end_time;
   std::string lastFiredName;
   bool initialized;
   socket_t* measureSocket;

   RandomWrapper randGen;

   // RESTART CHANGES
   vector_savedStates simSavedStates; // vector with saved states
   void save_simulation_state(int current_level);
   void load_last_simulation_state();
   void delete_last_simulation_state();
   bool run_restart_iterative( int restart_repeats, double thr_min, double thr_max, int thr_number);
   int level(double value_from, double value_to, double value_now, int thr_number);

   MeasureObj* importanceMeasure;
   double simulationWeight;

   static Simulation *sim;
};

#endif
