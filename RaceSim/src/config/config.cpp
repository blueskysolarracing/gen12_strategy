#include <config.h>
#include <string.h>
#include <utilities.h>
#include <cstring>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <Luts.h>
#include <Globals.h>
#include <custom_time.h>
#include <unordered_map>
#include "pugixml.hpp"
#include "spdlog/spdlog.h"
#include "yaml-cpp/yaml.h"

Config* Config::instance_ptr = nullptr;
YAML::Node Config::config;
std::unordered_map<std::string, YAML::Node> Config::key_values;

Config* Config::get_instance() {
    if (instance_ptr == NULL) {
		/* Parse config file */
		try {
			config = YAML::LoadFile(CONFIG_FILE_PATH);
			get_config_leaf_nodes(config);
		} catch (const YAML::ParserException& e) {
			spdlog::critical("Config file could not be parsed. Check yaml for correct syntax. Exiting");
			exit(0);
		} catch (const YAML::BadFile& e) {
			spdlog::critical("Config file could not be loaded. Check path relative to executable. Exiting");
			exit(0);
		} catch (const std::exception& e) {
			spdlog::critical("Config file could not be loaded. Exiting");
			exit(0);
		}
        instance_ptr = new Config(CONFIG_FILE_PATH);
    } 
    return instance_ptr;

}

// Config::Config(std::string config_file_path) {
// 	pugi::xml_document doc;
// 	pugi::xml_parse_result parsed_config = doc.load_file((const char*) config_file_path.c_str());
//     size_t ind = config_file_path.find("/config");
//     std::string luts = config_file_path.substr(0, ind) + "/luts/";

// 	if (!parsed_config) {
//         spdlog::critical("Config file could not be loaded. Exiting");
// 		exit(0);
// 	}

// 	pugi::xml_node struct_node = doc.child("struct");

// 	// control_stop_indices = create_control_stops_set(struct_node.child("Route").child("ControlStops").text().get());
// 	// luts_base_dir = luts + struct_node.child("LutPaths").child("BaseDir").text().get();
// 	// base_route_path = luts_base_dir + struct_node.child("LutPaths").child("BaseRoute").text().get();
// 	// speed_limit_path = luts_base_dir + struct_node.child("LutPaths").child("SpeedLimits").text().get();
// 	// traffic_signal_path = luts_base_dir + struct_node.child("LutPaths").child("TrafficSignals").text().get();
// 	// dni_path = luts_base_dir + struct_node.child("LutPaths").child("dni").text().get();
// 	// dhi_path = luts_base_dir + struct_node.child("LutPaths").child("dhi").text().get();
// 	// wind_dir_path = luts_base_dir + struct_node.child("LutPaths").child("WindDirection").text().get();
//     // wind_speed_path = luts_base_dir + struct_node.child("LutPaths").child("WindSpeed").text().get();
// 	// power_factor_path = luts_base_dir + struct_node.child("LutPaths").child("PowerFactor").text().get();
// 	// roll_res_slope_path = luts_base_dir + struct_node.child("LutPaths").child("rr2").text().get();
// 	// roll_res_yint_path = luts_base_dir + struct_node.child("LutPaths").child("rr1").text().get();
// 	// drive_eff_path = luts_base_dir + struct_node.child("LutPaths").child("DrivingEfficiency").text().get();
// 	// regen_eff_path = luts_base_dir + struct_node.child("LutPaths").child("RegenEfficiency").text().get();
// 	// battery_eff_path = luts_base_dir + struct_node.child("LutPaths").child("BatteryEfficiency").text().get();
//     // control_stop_charge_time = std::stoi(struct_node.child("Route").child("ControlStopChargeTime").text().get()) / 60.0; 
//     // utc_adjustment = std::stod(struct_node.child("Telemetry").child("UTCAdjustment").text().get());
//     // double current_soc_percentage = std::stod(struct_node.child("Telemetry").child("CurrentSOC").text().get());
//     // max_soc = std::stod(struct_node.child("Car").child("MaxBatteryCharge").text().get());
//     // starting_soc = max_soc * (current_soc_percentage / 100.0);
//     // double ending_soc_percentage = std::stod(struct_node.child("Telemetry").child("EndingSOC").text().get());
//     // ending_soc = max_soc * (ending_soc_percentage / 100.0);

//     // current_date_time = new Time(create_time(struct_node.child("Telemetry").child("CurrentDateTime").text().get()), utc_adjustment);
//     // end_date_time = new Time(create_time(struct_node.child("Telemetry").child("EndDateTime").text().get()), utc_adjustment);
//     // gps_coordinates = create_coord(struct_node.child("Telemetry").child("GpsCoordinates").text().get());

// 	// tire_pressure = std::stod(struct_node.child("Car").child("TirePressure").text().get());
// 	// array_area = std::stod(struct_node.child("Car").child("ArrayArea").text().get());
// 	// array_eff = std::stod(struct_node.child("Car").child("ArrayEfficiency").text().get());
// 	// car_mass = std::stod(struct_node.child("Car").child("CarMass").text().get());
// 	// air_density = std::stod(struct_node.child("Car").child("AirDensity").text().get());
// 	// passive_elec_loss = std::stod(struct_node.child("Car").child("PassiveElectricLoss").text().get());
// 	// motor_efficiency = std::stod(struct_node.child("Car").child("MotorEfficiency").text().get());
// 	// regen_efficiency = std::stod(struct_node.child("Car").child("RegenEfficiency").text().get());
// 	// battery_efficiency = std::stod(struct_node.child("Car").child("BatteryEfficiency").text().get());
// 	// cda = std::stod(struct_node.child("Car").child("cda").text().get());
//     // max_motor_power = std::stod(struct_node.child("Car").child("MaxMotorPower").text().get());

// 	// min_speed = std::stoi(struct_node.child("Strategy").child("MinSpeed").text().get());
// 	// max_speed = std::stoi(struct_node.child("Strategy").child("MaxSpeed").text().get());

//     // day_start = std::stoi(struct_node.child("Route").child("RaceStartTime").text().get());
// 	// day_end = std::stoi(struct_node.child("Route").child("RaceEndTime").text().get());

//     // stationary_sim = struct_node.child("Strategy").child("StationarySimulation").text().get() == "True" ? true : false;
//     // num_loops = std::stoi(struct_node.child("Strategy").child("NumLoops").text().get());
//     // array_power_max = std::stod(struct_node.child("Strategy").child("ArrayPowerMax").text().get());
//     // optimization_type = std::stoi(struct_node.child("Strategy").child("OptimizationType").text().get());
// 	// num_segments = std::stoi(struct_node.child("Strategy").child("NumSegments").text().get());
// 	// car_type = std::stoi(struct_node.child("Car").child("Model").text().get());

//     spdlog::info("Loaded config file: " + config_file_path);
// }

void Config::get_config_leaf_nodes(YAML::Node node) {
	if (node.IsMap()) {
		/* Iterate through all key/value pairs of the map */
        for (auto it = node.begin(); it != node.end(); ++it) {
			if (it->second.IsScalar()) {
				key_values[it->first.as<std::string>()] = it->second;
			} else {
				get_config_leaf_nodes(it->second);
			}
        }
    }
}

Config::~Config() {
	// if (current_date_time != nullptr) {
	// 	delete current_date_time;
	// }

	// if (end_date_time != nullptr) {
	// 	delete end_date_time;
	// }
}
