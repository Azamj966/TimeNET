#ifndef MEASUREOBJ_H
#define MEASUREOBJ_H

#include "DefaultGM.h"
#include "NetClass.h"
#include "DateTime.h"
#include <iostream>

class Simulation;
class Transition;

class MeasureObj : public DefaultGM
{
 public:
	static const unsigned int PREC_LIMIT=1000;

    MeasureObj(const char* name, NetClass* net, bool watch = true);
    void notify();

    int getRefCount() const { return reference_count; }
    void incrementRefCount() { reference_count++; }
    void decrementRefCount() { reference_count--; }
    list< list<double> > SampleBlocks;  // list for each sampling point with a list of double measures
    list< list<double> >::iterator s_iter; // iterator for current entry in SampleBlocks
    list< double >::iterator m_iter;
    list< double >::iterator d_iter;
    list< double >::iterator v_iter;
    list< double >::iterator e_iter;

    list< double > AveragedMean;
    list< double > Variance;
    list< double > DeltaX;
    list< double > Epsilon;
	
	//used only in transient simulation
	vector< double > mean_transient;
	vector< double >::iterator mean_transient_iter;
	vector< double > m2;
	vector< double >::iterator m2_iter;

    /** \brief Return the current value of the measure
     *
     * For instantaneous measures it computes the current rate reward.
     * For cumulative and time-average measures it returns the last
     * value computed by accum (optionally divided by the time).
     *
     * This method is called from notify after a transition fired.
     */
    //virtual double measure()=0;
    virtual double measure(double dt=-1)=0;
    // mode of computation: instantaneous / accumulative / time average
    virtual bool accumulative()=0;
    virtual bool timeaverage()=0;
    //double last_instMeasure;

    bool makeTransientAnalysis( double &alphat_def, double newest_sample, int sample_count, bool last );
    bool makeStationaryAnalysis( double &alphat_def, double start_time, double current_time);
    bool measure_reached_precision( unsigned int SampleCount, unsigned int SamplingPoints, bool mode );

    /** \brief Accumulate measure values
     *
     * This is a no op for instantaneous measures. For cumulative and
     * time-average measures it adds rate and impulse rewards to the
     * cumulative reward.
     *
     * This function is called from Transition::fire before the
     * transition fires.
     *
     * \param binding   pointer to the binding that is about to fire
     * \param weight    weight of simulation, for normal simulation is always 1
     */
    // RESTART changes, weight parameter added
    // virtual void accum(const Binding *binding)=0;
    virtual void accum(const Binding *binding, double weight)=0; // RESTART CHANGES

    void setPercentRule( bool b );
    bool PercentRule();

    void setPrecPortion( double portion );

    void setEpsMax( double eMax );

    // RESTART changes, sim_mode is now int, not bool!!!
    // void writeResult (std::ostream &stream1, std::ostream &stream2, bool sim_mode, const DateTime *start_time, double *interval_length);
    void writeResult (std::ostream &stream1, std::ostream &stream2, int sim_mode, const DateTime *start_time, double *interval_length);

    bool getWatch() {return watch;}

    bool precisionReached() {return precision_reached;}

    // reset the internal value to 0 for batch-means analysis
    // should be called in stationary simulation after a certain number of samples has been collected
    // and the current mean is stored in the batch list
    void reset(DateTime now);

    // RESTART method to reset time to a saved one
    void setLastTime(DateTime savedTime) {lastTime = savedTime;}

    string getName() { return name; }

    double firstTime, lastTime;
    double value;  // reward accumulated so far

    bool firstValue;

 private:
	unsigned int calls;
    int reference_count;
    double EpsMax;
    bool watch, percent_rule;
    double PrecPortion;
    bool precision_reached;
    string name;
};

#endif
