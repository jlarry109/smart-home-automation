#include "ConfigLoader.hpp"
#include <fstream>
#include <iostream>

MqttConfig ConfigLoader::load(const std::string& filename) {
    try {
        std::ifstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open config file: " + filename);
        }

        nlohmann::json j;
        file >> j;

        // Debug: print raw JSON
        std::cout << "[DEBUG] Loaded config JSON: " << j.dump(2) << std::endl;

        MqttConfig config;

        if (!j.contains("serverURI") || j["serverURI"].is_null())
            throw std::runtime_error("Missing or null 'serverURI' in config");

        if (!j.contains("clientId") || j["clientId"].is_null())
            throw std::runtime_error("Missing or null 'clientId' in config");

        if (!j.contains("certs") || !j["certs"].is_object())
            throw std::runtime_error("Missing or invalid 'certs' section in config");

        const auto& certs = j["certs"];
        if (!certs.contains("ca") || certs["ca"].is_null())
            throw std::runtime_error("Missing or null 'ca' in certs");
        if (!certs.contains("cert") || certs["cert"].is_null())
            throw std::runtime_error("Missing or null 'cert' in certs");
        if (!certs.contains("key") || certs["key"].is_null())
            throw std::runtime_error("Missing or null 'key' in certs");

        config.serverURI = j["serverURI"];
        config.clientId = j["clientId"];
        config.caPath = certs["ca"];
        config.certPath = certs["cert"];
        config.keyPath = certs["key"];

        return config;
    } catch (const nlohmann::json::exception& e) {
        throw std::runtime_error(std::string("JSON parsing error: ") + e.what());
    } catch (const std::exception& e) {
        throw std::runtime_error(std::string("Config loading failed: ") + e.what());
    }
}
