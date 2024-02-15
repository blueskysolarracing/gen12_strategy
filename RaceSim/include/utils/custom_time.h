/* Wraps the c++ time data structures 
   Note: We use the date and chrono libraries in order to make conversions between tm and time_t 
   representations since  mktime in c++ implicitly does a timezone conversion based on the user's machine.
*/

#ifndef CUSTOM_TIME_H
#define CUSTOM_TIME_H

#include <chrono>
#include <date.h>
#include <ctime>
#include <iostream>

class Time {
private:
	/* C++ tm structs and unix epoch times for both the local time and utc timepoint */
    tm m_datetime_local;
	tm m_datetime_utc;

	time_t t_datetime_local;
	time_t t_datetime_utc;

	/* Special field for milliseconds since the tm struct's resolution is up to seconds */
    double m_milliseconds;
public:
	/* Use the current time as the starting point */
    Time();

	/** Specify a starting time
     * @param local_time_point epoch time representing local time
     * @param utc_adjustment Adjustment in hours to utc e.g. If the time being represented is
     * 10:30am in Alice Springs which is 9.5 hours ahead of UTC, then this should be -9.5
    */
	Time(time_t local_time_point, double utc_adjustment);

	/** Get current hour in 24 hour format */
    inline int get_local_hours() { return m_datetime_local.tm_hour; };

    /** Get an epoch timestamp representing the utc time */
	inline time_t get_utc_time_point() { return t_datetime_utc; }

	/** Get an epoch timestamp representing the local time */
	inline time_t get_local_time_point() { return t_datetime_local; }

	/** Get c++ tm struct of the current utc time */
	inline tm get_utc_tm() { return m_datetime_utc; }

	/** Get c++ tm struct of the current local time */
	inline tm get_local_tm() { return m_datetime_local; }

    /** Get the current local time formatted according to the string in SolCast CSVs YYYYMMDDHHMM00 */
	uint64_t get_forecast_csv_time();

	/** Advances the current time by a certain number of seconds */
    void update_time_seconds(double seconds);

	/** Set the tm structs - no checks are performed to ensure validity
     * @param curr_time_local new local time
     * @param curr_time_utc corresponding utc time
    */
	void update_current_time(tm curr_time_local, tm curr_time_utc) {m_datetime_local = curr_time_local; m_datetime_utc = curr_time_utc;} 

	/** Print human readable local time */
	void print_local_readable_time() {std::cout << get_local_readable_time();}

	/** Print human readable utc time */
	void print_utc_readable_time() {std::cout << get_utc_readable_time() << std::endl;}
	
	/** Get human readable local time as a string */
	inline std::string get_local_readable_time() {std::string time = asctime(&m_datetime_local); return time.erase(time.size()-1);};

	/** Get human readable utc time as a string */
	inline std::string get_utc_readable_time() {return asctime(&m_datetime_utc);}
};

#endif /* CUSTOM_TIME_H */