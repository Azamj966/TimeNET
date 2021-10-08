/*===============> NEEDED BY count.script */

/****************************************************************************************/
/*                                                                                      */
/* some functions which implement timer to measure the efficiency of program parts      */
/*                                                                                      */
/****************************************************************************************/

#include <time.h>

/* The following line is needed in case CLOCKS_PER_SEC is not defined in time.h as we   */
/* remarked with NEXTSTEP 3.3. If this is also the case with your system you may need   */
/* to change the constant appropriately. */
#ifndef CLOCKS_PER_SEC 
#define CLOCKS_PER_SEC 60
#endif

#define RUNNING 1
#define NOT_RUNNING 0
#define NO_TIMERS 25

struct timer_type
       {int start, stop; double delay; int running;};
typedef struct timer_type timer_type;

timer_type timer [NO_TIMERS]; /* implementation of NO_TIMERS simultaneous timers */

/****************************************************************************************/

void start_timer (int no)
   /* starts the timer if it is not yet running */
   
{ 
   if (!(timer [no].running))
   {
     timer [no].start = clock ();
      timer [no].running = RUNNING;
   }
}

/****************************************************************************************/

void stop_timer (int no)
   /* stops the timer if it is running and adds the passed time to its variable delay */

{
   if (timer [no].running)
   {  timer [no].stop = clock ();
      timer [no].running = NOT_RUNNING;
      timer [no].delay +=   ((double) (timer [no].stop - timer [no].start))
                          / CLOCKS_PER_SEC;
   }
}
   
/****************************************************************************************/

void reset_timer (int no)
   /* stops the timer and resets its variable delay to 0 */
   
{
   timer [no].delay = 0;
   timer [no].delay = NOT_RUNNING;
}

/****************************************************************************************/

void reset_all_timer ()

{  int i;
   
   for (i = 0; i < NO_TIMERS; i++) reset_timer (i);
}

/****************************************************************************************/

double time_passed (int no)
   /* returns the passed time of the timer; if the timer is still running, the actually */
   /* passed time is added. */

{ 
   if (timer [no].running)
      return ( ((double) (clock () - timer [no].start))
                          / CLOCKS_PER_SEC    + timer [no].delay);
   else
      return (timer [no].delay);
}

/****************************************************************************************/
