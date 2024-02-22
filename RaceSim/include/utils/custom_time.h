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

	/* Indication for a HH:MM:SS only timestamp */
	bool hh_mm_ss_only;

	std::string local_time;

	void HH_MM_SS_constructor(std::string local_time_point);
	
public:
    Time() {};

	/**
	 * Create time with specific starting time
	 * @param local_time_point string in YYYY-MM-DD HH:MM:SS local 24 hour time format
	 * @param utc_adjustment Adjustment in hours from local time to utc e.g. If the time being represented is
     * 10:30am in Alice Springs which is 9.5 hours ahead of UTC, then this should be -9.5
	*/
	Time(std::string local_time_point, double utc_adjustment);

	/** 
	 * Create time without YYYY/MM/DD information. This will only fill in the m_datetime_local struct
	 * @param local_time_point string in HH:MM:SS local 24 hour time format
	*/
	Time(std::string local_time_point);

	/** Returns true if the lhs local timestamp is ahead of the rhs local timestamp */
    bool operator>(const Time& other) const;

	/** Returns true if the rhs local timestamp is ahead of the lhs local timestamp */
	bool operator<(const Time& other) const;

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
	void print_local_readable_time() {std::cout << get_local_readable_time() << std::endl;}

	/** Print human readable utc time */
	void print_utc_readable_time() {std::cout << get_utc_readable_time() << std::endl;}
	
	/** Get human readable local time as a string */
	std::string get_local_readable_time();

	/** Get human readable utc time as a string */
	std::string get_utc_readable_time();
};

#endif /* CUSTOM_TIME_H */