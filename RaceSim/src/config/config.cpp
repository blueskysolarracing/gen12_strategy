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
