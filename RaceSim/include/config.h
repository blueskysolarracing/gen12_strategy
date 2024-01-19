/* Singleton class that contains all parameters for a full scale session */

#ifndef CONFIG_H
#define CONFIG_H

#include <string.h>
#include <vector>
#include <units.h>
#include <Luts.h>
#include <set>
#include <Globals.h>

class Config {
private:
	/*------------------ All parameters specified by the configuration file ----------------------*/
	std::set<size_t> control_stop_indices;
	std::string luts_base_dir;
	std::string base_route_path;
	std::string speed_limit_path;
	std::string traffic_signal_path;
	std::string dni_path;
	std::string dhi_path;
	std::string wind_dir_path;
	std::string wind_speed_path;
	std::string power_factor_path;
	std::string roll_res_slope_path;
	std::string roll_res_yint_path;
	std::string drive_eff_path;
	std::string regen_eff_path;
	std::string battery_eff_path;
	int car_type;

	Coord gps_coordinates;
	Time* current_date_time;
	Time* end_date_time;
	bool stationary_sim;
	int optimization_type;
	int num_segments;
    double max_soc;
	double tire_pressure;
	double array_area;
	double array_eff;
	double car_mass;
	double air_density;
	double passive_elec_loss;
	int day_end;
	int day_start;
	double motor_efficiency;
	double regen_efficiency; 
	double battery_efficiency;
	double cda;
	double battery_temperature;
	double starting_soc;
	double ending_soc;
	double max_motor_power;
	double utc_adjustment;
	double control_stop_charge_time;

	int min_speed;
	int max_speed;
	int num_loops;
	double array_power_max;
	/*--------------------------------------------------------------------------------------------*/

	static Config* instance_ptr;

	/* Load all parameters from xml file. Should only be called from get_instance() */
	Config(std::string config_file_path);

	Config() {}

public:
	/* Public singleton constructor */
	static Config* get_instance();

	~Config();

	/* Getters */
	inline std::set<size_t> get_control_stop_indices() {return control_stop_indices;}
	inline std::string get_base_route_path() {return base_route_path;}
	inline std::string get_speed_limit_path() {return speed_limit_path;}
	inline std::string get_traffic_signal_path() {return traffic_signal_path;}
	inline std::string get_dni_path() {return dni_path;}
	inline std::string get_dhi_path() {return dhi_path;}
	inline std::string get_wind_dir_path() {return wind_dir_path;}
	inline std::string get_wind_speed_path() {return wind_speed_path;}
	inline std::string get_power_factor_path() {return power_factor_path;}
	inline std::string get_roll_res_slope_path() {return roll_res_slope_path;}
	inline std::string get_roll_res_yint_path() {return roll_res_yint_path;}
	inline std::string get_drive_eff_path() {return drive_eff_path;}
	inline std::string get_regen_eff_path() {return regen_eff_path;}
	inline std::string get_battery_eff_path() {return battery_eff_path;}
	inline Coord get_gps_coordinates() {return gps_coordinates;}
	inline Time* get_current_date_time() {return current_date_time;}
	inline Time* get_end_date_time() {return end_date_time;}
	inline bool get_stationary_sim() {return stationary_sim;}
	inline int get_opt_type() {return optimization_type;}
	inline int get_num_segments() {return num_segments;}
	inline double get_max_soc() {return max_soc;}
	inline double get_tire_pressure() {return tire_pressure;}
	inline double get_array_area() {return array_area;}
	inline double get_array_eff() {return array_eff;}
	inline double get_car_mass() {return car_mass;}
	inline double get_air_density() {return air_density;}
	inline double get_passive_elec_loss() {return passive_elec_loss;}
	inline int get_day_end() {return day_end;}
	inline int get_day_start() {return day_start;}
	inline double get_motor_efficiency() {return motor_efficiency;}
	inline double get_regen_efficiency() {return regen_efficiency;}
	inline double get_battery_efficiency() {return battery_efficiency;}
	inline double get_cda() {return cda;}
	inline double get_battery_temperature() {return battery_temperature;}
	inline double get_starting_soc() {return starting_soc;}
	inline double get_ending_soc() {return ending_soc;}
	inline double get_max_motor_power() {return max_motor_power;}
	inline double get_utc_adjustment() {return utc_adjustment;}
	inline double get_control_stop_charge_time() {return control_stop_charge_time;}
	inline int get_min_speed() {return min_speed;}
	inline int get_max_speed() {return max_speed;}
	inline int get_num_loops() {return num_loops;}
	inline double get_array_power_max() {return array_power_max;}
	inline int get_car_type() {return car_type;}

	/* No setters since these parameters should never change after initialization */
};

#endif
