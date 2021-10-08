/******************************************************************************

  filename :	sim_master_types.h

  purpose  :	type definitions for DSPN simulation engine

  author   :    Christian Kelling
  		Technical University Berlin
		Computer Science Department

  date     :	Jan 1994


******************************************************************************/

#ifndef MASTER_TYPES_H_
#define MASTER_TYPES_H_
typedef char bool;

#define false 0
#define true 1

#define  SUCCEED    0
#define  FAIL       1
#define  NO_ID		-1    /*usage in send/read commands as NULL id*/
#define  ALL            -1    /*usage in send/read commands as id
                                for all slaves */
#define  MAX_TRIES	    10

#define MIN_PROCESSES   3    /* number of processes when control
                                  variates or RESTART are used */

#define NO_TOK		0
#define ONE_TOK		1
#define FILE_NAME	1000    /* max. length of a filename */
#define	NEXT_LINE_BUF	500

#define MAX_BLOCK_DIFF	5	/* number of sample blocks to collect
				   before a slave is skipped */
#define MASTER_STOP  0
#define MASTER_RDY   1
#define MEASURE_STOP 2
#define PARTICLE_INCREASE 3

/* constants for measure_flag in places */
#define NO_MEASURE	0      /* no measure to estimate for this place */


/* constant for function check_enabled*/
#define NO_TRANS	-1	/* no transition is enabled -> dead */

/* constants for transition types */
#define IMM             0
#define EXP		1
#define DET		2
#define GEN		3

/* constants for firing policies */
#define RS             0
#define RA		1
#define RE		2

#define INF_SERVER	0
#define SING_SERVER	1

/* constants for arc types */
#define INP_ARC		0
#define OUTP_ARC	1
#define INH_ARC		2
#define INP_OR_INH	3

/* constants for measure types */
#define PROB_MEAS       1
#define OTHER_MEAS      0

/* constants for other connectivities */
#define ENA_CONN	4  /* no arc connection but influence via enabling
			      function*/
#define MDA_CONN	5  /* no arc connection but influence via m.-d.
			      arc multiplicity */
/* constants for selection of measure output in _ERC.c */
#define SEL_MEAN      	0
#define SEL_UP		1
#define SEL_LO		2

/* constants to determine the difference at the Conf.level for Probs. */
#define STEPS           999
#define Pi              3.1415927

/* type definitions */

typedef struct sample_type	/* type for a sample containing current time */
{
  double value;		/* the value itself */
  double s_time;	/* the sampling time of the value or the batching
   			   time if batches are used */
} sample_type;

typedef double str_sample_type; /* type for a sample containing current time */

typedef struct meas_list   /* list of measures a place/transition is involved in */
{
  int	 id;
  struct meas_list *next;
} meas_list;

typedef struct par_name		/* type for marking or delay parameter */
{
/*  char	name[30];*/
  char	*name;
  float value;
} par_name;

typedef struct trans_delay_list  /* type for list of Transitions and */
{                                /* and their delays */
  char name[30];
  double delay;
  struct trans_delay_list *next;
} trans_delay_list;

typedef struct trans_list        /* type for list of Transition-names */
{
  char name[30];
  struct trans_list *next;
} trans_list;

typedef struct meas_names_list
{
  char name[30];
  struct meas_names_list *next;
} meas_names_list;

typedef struct conn_list	/* type for list of connections */
{				/* of a transition*/
  int	multiplicity;
  int	element;
  int	id;
} conn_list;

typedef struct meas_trans_list    /* type for list of measures and their */
{                                /*  cv-Transitions */
  int meas_id;
  trans_list *trans;
  int	useCV; //indicated whether CV-method should be used or not
  struct meas_trans_list *next;
} meas_trans_list;


typedef struct cv_list_elem
{
  int meas_number;
  int cv_number;
  int recorded;
  struct cv_list_elem *next;
} cv_list_elem;


typedef struct aff_transition_list  /* type for list of affected transitions */
{				    /* this is a transition which can becomes*/
  int number;			    /* enabled when the effecting transition */
  struct aff_transition_list *next; /* fires */
} aff_list;

typedef struct place		/* type for all places */
{
  char	*name;
  int	token;
  int	no_measures;  /* denotes the kind of measure this place */
  		       /* is involved in */
 struct  meas_list *measure_list;
  		       /* a list of identifiers used as arguments when
		       calling the select_measure-functions from <net>_ERC.c*/
} place;
/** shorter version of Place needed for in sim trans */
typedef struct hiplace
{

	int tokens;
} hiplace;
/** for every hiPlace arrays there is the history info */
typedef struct history
{
	int passed; //0 not passed 1 passed
	double time;
} history;


typedef struct transition	/* type for all transitions */
{
  char	*name;
  int	type;
  int  serv_type;
  double  delay;
  int	prio;
  int 	fire_policy;	/* firing policy */
  int	ena_id;		/* identifikation of enabling function */
  int	mark_dep_id; 	/* identification
			   of marking dependent delay or weight */
  int	ok;             /* flag checks wether the m-d delay or weight could
			   determined by parsing .DEF-file */
  int no_outputs;
  int no_inputs;
  int no_inh;
  int no_aff;		/* number of affected transitions */
  int no_cv_meas;
  conn_list  *out_to_place;
  conn_list  *in_from_place;
  conn_list  *inh_from_place;
  aff_list *affected_trans;
  cv_list_elem *cv_list;
  int has_fired;
  int	no_measures;  /* denotes the number of measures this transition
  		        is involved in */
  struct  meas_list *measure_list;  		       /* a list of identifiers used as arguments when
		       calling the select_measure-functions from <net>_ERC.c*/
 } transition;


/* Informationen ueber die Rechner aus dem Environmentfile: */
typedef struct hostinfo {
  struct hostinfo *next;		/* naechster Eintrag */
  char *hostname;			/* Rechnername */
  unsigned short running_processes;	/* Anzahl laufender
					   sim_slave-Prozesse */
  long load;				/* Auslastung des Rechners */
  unsigned priority;			/* Prioritaet zur Benutzung dieses
					   Rechners */
  bool no_rusers;			/* rusers-Aufruf absetzen ? (Auf DECs
					   ist kein rusers-daemon vorhanden) */
  bool free;				/* Rechner unbenutzt ? */
} hostinfo;

/* Informationen uber einen remote laufenden sim_slave */
typedef struct slaveinfo {
  hostinfo *host;			/* Rechner, auf welchem der zugehoerige
					   rand-Prozess laeuft */
  int connection_fd;			/* Filedescriptor fuer die Socket-
					   Verbindung zum sim_slave-Prozess */
  FILE *connection_fp;			/* Filepointer zu socket_fd */
  enum {
    no_process, waiting_for_connection, connected, running, terminated
  } status;				/* Status des zugehoerigen rand-
					   Prozesses */
  double slave_local_time;		/* lokale Modellzeit am Slave */
  bool slave_local_min_flag;		/* lokales Flag fuer MIN_FIRINGS*/
  					/* aller Transitionen*/
  int PID;                              /* Prozessnummer des Slaves */
} slaveinfo;


#endif
