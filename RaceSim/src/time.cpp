#include <units.h>
#include <string>
#include <cstdint>
#include <cmath>
#include <ctime>
#include <chrono>

/* Note that mktime() followed by gmtime() will convert a local time to utc time */
Time::Time() : m_milliseconds(0) {
	std::cout << "default time constructor" << std::endl;
	time_t now = time(0);
	tm* datetime = gmtime(&now);
	m_datetime_local = *datetime;
}

Time::Time(tm local_time_point) {
	m_datetime_local = local_time_point;
	time_t local_time_t = mktime(&m_datetime_local);
	m_datetime_utc = *gmtime(&local_time_t);
}

void Time::update_time_seconds(double seconds) {
	double total_seconds = seconds + 1000 * m_milliseconds;
	m_datetime_local.tm_sec += (int)seconds;
	m_datetime_utc.tm_sec += (int)seconds;

	/* mktime() automatically fixes the tm structs if they violate any ranges */
	mktime(&m_datetime_local);
	mktime(&m_datetime_utc);
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

std::string Time::get_readable_time() {
	std::string year_s = std::to_string(m_datetime_local.tm_year + 1900);

	std::string month_s = std::to_string(m_datetime_local.tm_mon+1);
	month_s = month_s.size() == 1 ? "0" + month_s : month_s;

	std::string day_s = std::to_string(m_datetime_local.tm_mday);
	day_s = day_s.size() == 1 ? "0" + day_s : day_s;

	std::string hour_s = std::to_string(m_datetime_local.tm_hour);
	hour_s = hour_s.size() == 1 ? "0" + hour_s : hour_s;

	std::string min_s = std::to_string(m_datetime_local.tm_min);
	min_s = min_s.size() == 1 ? "0" + min_s : min_s;

	std::string sec_s = std::to_string(m_datetime_local.tm_sec);
	sec_s = sec_s.size() == 1 ? "0" + sec_s : sec_s;

	return year_s + "/" + month_s + "/" + day_s + "/" + hour_s + "/" + min_s + "/" + sec_s;
}