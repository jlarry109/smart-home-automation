#ifndef CONFIG_LOADER_HPP
#define CONFIG_LOADER_HPP

#include <string>
#include <nlohmann/json.hpp>

struct MqttConfig {
    std::string serverURI;
    std::string clientId;
    std::string caPath;
    std::string certPath;
    std::string keyPath;
};

class ConfigLoader {
public:
    static MqttConfig load(const std::string& filename);
};
#endif // CONFIG_LOADER_HPP
