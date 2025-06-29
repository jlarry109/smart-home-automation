#include "ConfigLoader.hpp"
#include <fstream>

MqttConfig ConfigLoader::load(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open config file");
    }
    nlohmann::json j;
    file >> j;

    MqttConfig config;
    config.serverURI = j["serverURI"];
    config.clientId = j["clientID"];
    config.caPath = j["certs"]["ca"];
    config.certPath = j["certs"]["cert"];
    config.keyPath = j["certs"]["key"];
    return config;
}