/* Represents a config parameter */

#ifndef CONFIG_PARAM_H
#define CONFIG_PARAM_H
#include <stdlib.h>
#include <string.h>
#include <utilities.h>
#include <custom_time.h>
#include <yaml-cpp/yaml.h>

template<typename T>
class Config_Param {
private:
    T value;
    std::string name;
public:
    T get_value() {return value;}
    Config_Param(std::string param_name, T default_value, std::unordered_map<std::string, YAML::Node> key_values) {
        name = param_name;
        if (key_values.find(name) != key_values.end()) {
            if constexpr (std::is_same<Coord, T>::value) {
                value = create_coord(key_values[name].as<std::string>());
            } else if constexpr (std::is_same<std::unordered_set<size_t>, T>::value) {
                value = convert_string_to_int_set(key_values[name].as<std::string>());
            } else if constexpr (std::is_same<Time*, T>::value) {
                value = new Time(key_values[name].as<std::string>(), key_values["utc_adjustment"].as<double>());
            } else {
                value = key_values[name].as<T>();
            }
        } else {
            value = default_value;
        }
    }
    Config_Param<T>() {}
};

#endif /* CONFIG_PARAM_H */

