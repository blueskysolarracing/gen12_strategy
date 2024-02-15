#include <units.h>
#include <string>
#include <cstdint>
#include <cmath>
#include <ctime>
#include <chrono>
#include <date.h>
#include "custom_time.h"

Time::Time() : m_milliseconds(0) {
	time_t now = time(0);
	tm* datetime = gmtime(&now);
	m_datetime_local = *datetime;
}

Time::Time(time_t local_time_point, double utc_adjustment) {
	t_datetime_local = local_time_point;

	/* Make manual utc adjustment */
	t_datetime_utc = t_datetime_local + hours2secs(utc_adjustment);

	m_datetime_local = *gmtime(&t_datetime_local);
	m_datetime_utc = *gmtime(&t_datetime_utc);
}

void Time::update_time_seconds(double seconds) {
	double total_seconds = seconds + 1000 * m_milliseconds;
	t_datetime_local += (int)seconds;
	t_datetime_utc += (int)seconds;

	m_datetime_local = *gmtime(&t_datetime_local);
	m_datetime_utc = *gmtime(&t_datetime_utc);
	m_milliseconds = total_seconds - floor(total_seconds);

}

uint64_t Time::get_forecast_csv_time() {
	std::string year = std::to_string(m_datetime_local.tm_year - 100);
	std::string month = m_datetime_local.tm_mon >= 9 ? std::to_string(m_datetime_local.tm_mon+1) : "0" + std::to_string(m_datetime_local.tm_mon+1);
	std::string day = m_datetime_local.tm_mday >= 10 ? std::to_string(m_datetime_local.tm_mday) : "0" + std::to_string(m_datetime_local.tm_mday);
	std::string hour = m_datetime_local.tm_hour >= 10 ? std::to_string(m_datetime_local.tm_hour) : "0" + std::to_string(m_datetime_local.tm_hour);
	std::string minute = m_datetime_local.tm_min >= 10 ? std::to_string(m_datetime_local.tm_min) : "0" + std::to_string(m_datetime_local.tm_min);

	return std::stoull(year+month+day+hour+minute+"00");
}
