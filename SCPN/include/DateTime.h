/**
 * This file is part of PB-Lib C/C++ Library
 *
 * Copyright (c) 1995, 1996 Branislav L. Slantchev
 * A Product of Silicon Creations, Inc.
 *
 * This class is hereby donated to the SNIPPETS collection (maintained
 * by Bob Stout). You are granted the right to use the code contained
 * herein free of charge as long as you keep this copyright notice intact.
 *
 * Contact: 73023.262@compuserve.com
 *
 * @author Michael Knoke
 * @version 1.0
 * 
 * Changed in TimeNET project 2007
 * 
 * We added some functions and corrected comparisons.
 * @author Enrico Stephan, Alexej Tuchscherer
 * @version 1.1  
*/

#ifndef __DATE_INC
#define __DATE_INC

#include "gmnet.h"

typedef unsigned long long ulonglong;
#include <math.h>
#include <time.h>
#include <iostream>
#include <sstream>

using namespace std;

#ifdef USE_BOEHM_GC
/*everything derived from this class will be 
garbage collected*/
class DateTime : public gc
#else
class DateTime
#endif
{
public:
   /** month names */
   enum month {
      jan = 1, feb, mar, apr, may, jun, jul, aug, sep, oct, nov, dec
   };

   /** weekday names */
   enum week_day {
      mon = 1, tue, wed, thu, fri, sat, sun
   };

   /** moon phase names */
   enum moon_phase {
      new_moon, waxing_crescent, first_quater, waxing_gibbous,
      full_moon, waning_gibbous, third_quater, waning_crescent
   };


   DateTime();
   DateTime(month month, int day, int year, int hour, int minute, int second);
   DateTime(month month, int day, int year);
   DateTime(int dayOfYear, int year, int hour, int minute, int second);
   DateTime(int dayOfYear, int year);
   DateTime(const DateTime &aDate);
   DateTime(ulonglong nDayNumber, int hour, int minute, int second);
   DateTime(ulonglong nSeconds);
   DateTime(double value);
   DateTime(const struct tm *date);
   DateTime(const char* datestring);
   DateTime(const std::string &datestring);

   void              AddDays(int nDays);
   void              AddHours(int nHours);
   void              AddMinutes(int nMinutes);
   void              AddMonths(int nMonths);
   void              AddSeconds(int nSeconds);
   void              AddTicks(double nTicks);
   void              AddWeeks(int nWeeks);
   void              AddYears(int nYears);
   int               Age(const DateTime &aDate) const;
   DateTime          BeginDST() const;
   static DateTime   BeginDST(int aYear);
   static bool       Check(const std::string &datestring);
   int               Day() const;
   ulonglong         DayNumber() const;
   week_day          DayOfWeek() const;
   int               DayOfYear() const;
   int               DaysInMonth() const;
   static int        DaysInMonth(month aMonth, int aYear);
   int               DaysInYear() const;
   static int        DaysInYear(int year);
   DateTime          Easter() const;
   static DateTime   Easter(int year);
   DateTime          EndDST() const;
   static DateTime   EndDST(int aYear);
   double            Fraction() const;
   ulonglong         FullSeconds() const;
   double            FullDateTime() const;
   int               Hour() const;
   bool              IsDST() const;
   static bool       IsDST(const DateTime &date);
   bool              IsLeapYear() const;
   static bool       IsLeapYear(int year);
   bool              IsValid() const;
   static bool       IsValid(month aMonth, int aDay, int aYear, int aHour, int aMinute, int aSecond);
   static bool       IsValid(int aHour, int aMinute, int aSecond);
   static bool       IsValid(month aMonth, int aDay, int aYear);
   int               Minute() const;
   month             Month() const;
   moon_phase        MoonPhase() const;
   static moon_phase MoonPhase(const DateTime &date);
   DateTime          NextWeekDay(week_day weekday, int hour, int minute, int second) const;
                     operator long() const;
                     operator double() const;
   bool              operator!=(const DateTime &aDate) const;
//   ulonglong          operator+(const DateTime &aDate) const;
   double            operator+(const DateTime &aDate) const;
   DateTime          operator+(int nSeconds) const;
   DateTime          operator+(double nTicks) const;
   DateTime&         operator++();
   DateTime          operator++(int);
   DateTime&         operator+=(int nSeconds);
   DateTime&         operator+=(double nTicks);
   DateTime&         operator+=(const DateTime &aDate);
//   ulonglong          operator-(const DateTime &aDate) const;
   double            operator-(const DateTime &aDate) const;
   DateTime          operator-(int nSeconds) const;
   DateTime&         operator--();
   DateTime          operator--(int);
   DateTime&         operator-=(int nSeconds);
   DateTime&         operator-=(double nTicks);
   DateTime&         operator-=(const DateTime &aDate);
   bool              operator<(const DateTime &aDate) const;
   bool              operator<=(const DateTime &aDate) const;
   DateTime&         operator=(const DateTime &aDate);
   bool              operator==(const DateTime &aDate) const;
   bool              operator>(const DateTime &aDate) const;
   bool              operator>=(const DateTime &aDate) const;
   char              operator[](int index) const;
   friend ostream&   operator<<(ostream &stream, const DateTime &date);
   int               Second() const;
   static void       SetBeginDST(month aMonth, week_day aWeekDay);
   void              SetDate(const DateTime &aDate);
   static void       SetEndDST(month aMonth, week_day aWeekDay);
   void              SetFraction(double aFraction);
   void              SetHour(int aHour);
   void              SetMinute(int aMinute);
   void              SetSecond(int aSecond);
   static DateTime   Today();
   string            toString() const;
   int               WeekOfMonth() const;
   int               WeekOfYear() const;
   int               WeeksInYear() const;
   static int        WeeksInYear(int year);
   int               Year() const;

   // Pope Gregor XIII's reform cancelled 10 days:
   // the day after Oct 4 1582 was Oct 15 1582
   static const int        ReformYear;
   static const month      ReformMonth;
   static const ulonglong  ReformDayNumber;

   static const DateTime MAX;
   static const DateTime MIN;

   // Timeguard functionality
   double NextWorktime(int fromHour, int toHour) const;

protected:
   // Daylight Savings Time Month and Day of Week
   static month    BeginDSTMonth;
   static week_day BeginDSTDay;
   static month    EndDSTMonth;
   static week_day EndDSTDay;

protected:
   void              Set(month aMonth, int aDay, int aYear, int aHour, int aMinute, int aSecond);
   void              Set(month aMonth, int aDay, int aYear, int aHour, int aMinute, int aSecond, double aFraction);
   static ulonglong  MakeDayNumber(month aMonth, int aDay, int aYear);

private:
   double m_ticks;   ///< second ticks since 00:00:00 midnight, January 1, 0001
};

#endif /* __DATE_INC */
