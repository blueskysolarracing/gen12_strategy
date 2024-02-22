/* Singleton class that contains all parameters for a full scale session */

#ifndef CONFIG_H
#define CONFIG_H

#include <string.h>
#include <vector>
#include <units.h>
#include <Luts.h>
#include <unordered_set>
#include <Globals.h>
#include <custom_time.h>
#include <unordered_map>
#include <config_param.h>

/* Define all config parameters
   PARAM(<parameter name as it appears exactly in the configuration file>, <data type>, <default value>)
 */
#define CONFIG_PARAMETERS\
	PARAM(max_soc, double, 5.2)\
	PARAM(tire_pressure, double, 5.5)\
	PARAM(array_area, double, 3.98)\
	PARAM(car_mass, double, 305)\
	PARAM(air_density, double, 1.17)\
	PARAM(passive_electric_loss, double, 20)\
	PARAM(cda, double, 0.16) \
	PARAM(max_motor_power, double, 3.5) \
	PARAM(max_car_speed, double, 120) \
	PARAM(array_power_max, double, 950) \
	PARAM(array_efficiency, double, 0.252) \
	PARAM(motor_efficiency, double, 0.9) \
	PARAM(regen_efficiency, double, 0.8) \
	PARAM(battery_efficiency, double, 0.95) \
	PARAM(base_dir, std::string, "wsc_2023") \
	PARAM(base_route_path, std::string, "/static/baseroute.csv") \
	PARAM(speed_limits_path, std::string, "/static/longDistanceSpeedLimit.csv") \
	PARAM(traffic_signals_path, std::string, "/static/powerfactor.csv") \
	PARAM(power_factor_path, std::string, "/static/powerfactor.csv") \
	PARAM(roll_res_slope_path, std::string, "/static/rr2.csv") \
	PARAM(roll_res_yint_path, std::string, "/static/rr1.csv") \
	PARAM(dni_path, std::string, "/dynamic/dni.csv") \
	PARAM(dhi_path, std::string, "/dynamic/dhi.csv") \
	PARAM(wind_direction_path, std::string, "/dynamic/wind_direction_10m.csv") \
	PARAM(wind_speed_path, std::string, "/dynamic/wind_speed_10m.csv") \
	PARAM(control_stops, std::unordered_set<size_t>, convert_string_to_int_set("2962,5559,9462,11421,14439,16990,20832,23202,25987")) \
	PARAM(control_stop_charge_time, double, 30) \
	PARAM(race_start, Time*, new Time("2023-10-22 08:30:00", -9.5)) \
	PARAM(day_start_time, Time*, new Time("09:00:00")) \
	PARAM(day_end_time, Time*, new Time("17:00:00")) \
	PARAM(first_day, bool, true) \
	PARAM(current_soc, double, 100) \
	PARAM(gps_coordinates, Coord, Coord()) \
	PARAM(current_date_time, Time*, new Time("2023-10-22 08:30:00", -9.5)) \
	PARAM(utc_adjustment, double, -9.5) \
	PARAM(model, std::string, "Gen 11.5") \
	PARAM(optimizer, std::string, "Constant") \
	PARAM(min_speed, double, 40) \
	PARAM(max_speed, double, 100) \
	PARAM(num_segments, double, 1) \
	PARAM(save_csv, bool, true) \

class Config {
private:
	/* Path to the config file */
	std::string file_path;

	/* Parsed config */
	static YAML::Node config;

	/* Map with all parsed key-value pairs from the yaml config file */
	static std::unordered_map<std::string, YAML::Node> key_values;

	/* Singleton pointer */
	static Config* instance_ptr;

	/* Declare all parameters */
	#define PARAM(name, type, default_value) \
		Config_Param<type> name;
		
	CONFIG_PARAMETERS

	#undef PARAM

	/* Initialize all parameters */
	#define PARAM(name, type, default_value)\
		name(Config_Param<type>(#name, default_value, key_values)),

	/* Load all parameters from yaml file. Should only be called from get_instance() */
	Config(std::string config_file_path) : CONFIG_PARAMETERS file_path(config_file_path){}

	#undef PARAM

	/* Extract all values from the config */
	static void get_config_leaf_nodes(YAML::Node node);

public:

	/* Public singleton constructor */
	static Config* get_instance();

	~Config();

	/* Getters */
	#define PARAM(name, type, default_value) \
		inline type get_##name() {return name.get_value();}

	CONFIG_PARAMETERS
	#undef PARAM

	/* No setters since config parameters should never change after initialization */
};

#endif
