/*************************************************************************

  filename:	spec_var.h

  purpose:	Constants and types will be defined for spec_var.c

  author:       Christian Kelling
  		Technical University of Berlin
		Computer Science Department
		Institute for Technical Computer Science

  date:		May 1991, November 1992, Aug 93

*************************************************************************/

#ifndef SPEC_VAR_H_
#define SPEC_VAR_H_

#include "sim_master_types.h"

#define STEADY_STATE 	0

/* Constant values for initializing parameters */
#define VARSEQLEN_DEF	100
#define SAFE_DEF_STEADY	1.5
#define EXCOEFF_DEF	0.5
#define ALPHAT_DEF	(1.0-(double)conf_level/100.0)
#define FREEDOM_DEF_STEADY	7 /* Variante 3 und 16*/

#define	M0		100


/* Environment used by the spectral variance analysis for each opened
   STATISTICS */
typedef struct t_SPECStatisticsEnvironment
{
  char          name[30];
  int           kindOfMeasure;              /* 1:probability   0:other */
  bool		StopSimulation;					/* true: stop collecting samples for this measure */
  bool          StopSendedToBackend;
  bool		NoPrecision;					/* true: measure did not reach the predefined accuracy */
  int		StatStatus;
  double 	AnalysedSequence[2*M0+1];
  double  	Epsilon;						/* relative size of convergence interval */
  double  	MeanValue;						/* mean Value of collected samples */
  double  	sum;							/* temporary sum of samples before batching */
  double	DeltaX;							/* absolute size of convergence interval */
  long     	SampleCount;
  int           SendFactor;
  long     	CheckNumber;
  long     	CheckLocLast;
  long     	CheckLocNext;					/* compute convergence interval when SampleCount reaches this number */
  int     	BatchSize;
  long     	BatchMeanNumber;
  sample_type	***SampleBlock;
  int		*BlockAdminSum;
  int		*BlockAdminFirst;
  int		InitLength;
  double	TransTime;
  double	EndTime;
  int		ReplicationCounter;
  //progressbar
  int ID;									/* Index in measure array */
  double ExpectedSampleCount;				/* Estimated Overall Sample Count */
  long ExpectedSampleBorder;				/* Sample Count used for progress estimation */
  double EstimatedProgress;					/* Samplecount / ExpectedSampleBorder */
  long LastSampleBorder;					/* exponential increasing border used for computing CheckLocNext */
} SpecStat;

#define     a_seq MeasureStat->AnalysedSequence

#endif
