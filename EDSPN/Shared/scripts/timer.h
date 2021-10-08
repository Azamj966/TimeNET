/*===============> NEEDED BY count.script */

#ifndef __TIMER_H__
#define __TIMER_H__

/****************************************************************************************/

void start_timer (int no);
   /* starts the timer if it is not yet running */
 
/****************************************************************************************/

void stop_timer (int no);
   /* stops the timer if it is running and adds the passed time to its variable delay */

/****************************************************************************************/

void reset_timer (int no);
   /* stops the timer and resets its variable delay to 0 */

/****************************************************************************************/

void reset_all_timer ();

/****************************************************************************************/

double time_passed (int no);
   /* returns the passed time of the timer; if the timer is still running, the actually */
   /* passed time is added. */
#endif
