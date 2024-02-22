#include <units.h>
#include <string>
#include <cstdint>
#include <cmath>
#include <ctime>
#include <chrono>
#include <date.h>
#include "custom_time.h"

Time::Time(std::string local_time_point, double utc_adjustment) {
	local_time = local_time_point;
	std::istringstream iss(local_time_point);
	std::string date_str, time_str;
	iss >> date_str >> time_str;
	if (time_str.empty()) {
		/* The timestamp is in HH:MM:SS format, call the other constructor */
		HH_MM_SS_constructor(local_time_point);
		return;
	} 

	std::istringstream rss(local_time_point);
    date::sys_time<std::chrono::seconds> epoch_time;
    rss >> date::parse("%F %T", epoch_time);

    // Convert sys_time to time_t
    time_t local_time_t = std::chrono::system_clock::to_time_t(epoch_time);

    t_datetime_local = local_time_t;

	t_datetime_utc = t_datetime_local + hours2secs(utc_adjustment);

	m_datetime_local = *gmtime(&t_datetime_local);
	m_datetime_utc = *gmtime(&t_datetime_utc);

	m_milliseconds = 0;

	hh_mm_ss_only = false;
	return;
}

Time::Time(std::string local_time_point) {
	HH_MM_SS_constructor(local_time_point);
}

void Time::HH_MM_SS_constructor(std::string local_time_point) {
	std::istringstream iss(local_time_point);
	int hours, minutes, seconds;

	char delimiter;
	iss >> hours >> delimiter >> minutes >> delimiter >> seconds;

    time_t now = time(nullptr);
    m_datetime_local = *localtime(&now);
	m_datetime_local.tm_hour = hours;
	m_datetime_local.tm_min = minutes;
	m_datetime_local.tm_sec = seconds;

	hh_mm_ss_only = true;
	return;
}

/* Compare each member of the tm struct */
bool Time::operator>(const Time& other) const {

	if (!this->hh_mm_ss_only && !other.hh_mm_ss_only) {
		if (this->m_datetime_local.tm_year > other.m_datetime_local.tm_year) {
			return true;
		} else if (this->m_datetime_local.tm_year < other.m_datetime_local.tm_year) {
			return false;
		}

		if (this->m_datetime_local.tm_mon > other.m_datetime_local.tm_mon) {
			return true;
		} else if (this->m_datetime_local.tm_mon < other.m_datetime_local.tm_mon) {
			return false;
		}

		if (this->m_datetime_local.tm_mday > other.m_datetime_local.tm_mday) {
			return true;
		} else if (this->m_datetime_local.tm_mday < other.m_datetime_local.tm_mday) {
			return false;
		}
	}

	if (this->m_datetime_local.tm_hour > other.m_datetime_local.tm_hour) {
		return true;
	} else if (this->m_datetime_local.tm_hour < other.m_datetime_local.tm_hour) {
		return false;
	}

	if (this->m_datetime_local.tm_min > other.m_datetime_local.tm_min) {
		return true;
	} else if (this->m_datetime_local.tm_min < other.m_datetime_local.tm_min) {
		return false;
	}

	if (this->m_datetime_local.tm_sec > other.m_datetime_local.tm_sec) {
		return true;
	} else if (this->m_datetime_local.tm_sec < other.m_datetime_local.tm_sec) {
		return false;
	}

	if (this->m_milliseconds > other.m_milliseconds) {
		return true;
	} else if (this->m_milliseconds < other.m_milliseconds) {
		return false;
	}

	return false;
	
}

bool Time::operator<(const Time& other) const {

	if (!this->hh_mm_ss_only && !other.hh_mm_ss_only) {
		if (this->m_datetime_local.tm_year < other.m_datetime_local.tm_year) {
			return true;
		} else if (this->m_datetime_local.tm_year > other.m_datetime_local.tm_year) {
			return false;
		}

		if (this->m_datetime_local.tm_mon < other.m_datetime_local.tm_mon) {
			return true;
		} else if (this->m_datetime_local.tm_mon > other.m_datetime_local.tm_mon) {
			return false;
		}

		if (this->m_datetime_local.tm_mday < other.m_datetime_local.tm_mday) {
			return true;
		} else if (this->m_datetime_local.tm_mday > other.m_datetime_local.tm_mday) {
			return false;
		}
	}

	if (this->m_datetime_local.tm_hour < other.m_datetime_local.tm_hour) {
		return true;
	} else if (this->m_datetime_local.tm_hour > other.m_datetime_local.tm_hour) {
		return false;
	}

	if (this->m_datetime_local.tm_min < other.m_datetime_local.tm_min) {
		return true;
	} else if (this->m_datetime_local.tm_min > other.m_datetime_local.tm_min) {
		return false;
	}

	if (this->m_datetime_local.tm_sec < other.m_datetime_local.tm_sec) {
		return true;
	} else if (this->m_datetime_local.tm_sec > other.m_datetime_local.tm_sec) {
		return false;
	}

	if (this->m_milliseconds < other.m_milliseconds) {
		return true;
	} else if (this->m_milliseconds > other.m_milliseconds) {
		return false;
	}

	return false;
}

std::string Time::get_local_readable_time() {
	if (hh_mm_ss_only) {
		std::string time_s = std::to_string(m_datetime_local.tm_hour) + ":" 
							 + std::to_string(m_datetime_local.tm_min) + ":" 
							 + std::to_string(m_datetime_local.tm_sec);
		return time_s;
	}

	std::string time = asctime(&m_datetime_local); 
	return time.erase(time.size()-1);
};

void Time::update_time_seconds(double seconds) {
	double total_seconds = seconds + 1000 * m_milliseconds;
	t_datetime_local += (int)seconds;
	t_datetime_utc += (int)seconds;

	m_datetime_local = *gmtime(&t_datetime_local);
	m_datetime_utc = *gmtime(&t_datetime_utc);
	m_milliseconds = total_seconds - floor(total_seconds);

}

std::string Time::get_utc_readable_time() {
	if (hh_mm_ss_only) {
		std::string time_s = std::to_string(m_datetime_utc.tm_hour) + ":" 
							 + std::to_string(m_datetime_utc.tm_min) + ":" 
							 + std::to_string(m_datetime_utc.tm_sec);
		return time_s;
	}
	std::string time = asctime(&m_datetime_utc); 
	return time.erase(time.size()-1);
};

uint64_t Time::get_forecast_csv_time() {
	std::string year = std::to_string(m_datetime_local.tm_year - 100);
	std::string month = m_datetime_local.tm_mon >= 9 ? std::to_string(m_datetime_local.tm_mon+1) : "0" + std::to_string(m_datetime_local.tm_mon+1);
	std::string day = m_datetime_local.tm_mday >= 10 ? std::to_string(m_datetime_local.tm_mday) : "0" + std::to_string(m_datetime_local.tm_mday);
	std::string hour = m_datetime_local.tm_hour >= 10 ? std::to_string(m_datetime_local.tm_hour) : "0" + std::to_string(m_datetime_local.tm_hour);
	std::string minute = m_datetime_local.tm_min >= 10 ? std::to_string(m_datetime_local.tm_min) : "0" + std::to_string(m_datetime_local.tm_min);

	return std::stoull(year+month+day+hour+minute+"00");
}
