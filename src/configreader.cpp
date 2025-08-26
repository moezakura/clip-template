#include "configreader.h"
#include <yaml-cpp/yaml.h>
#include <fstream>
#include <iostream>

ConfigReader::ConfigReader() = default;
ConfigReader::~ConfigReader() = default;

std::vector<Template> ConfigReader::readConfig(const std::string &filepath)
{
    std::vector<Template> templates;
    
    try {
        YAML::Node config = YAML::LoadFile(filepath);
        
        if (config["templates"]) {
            for (const auto &node : config["templates"]) {
                Template tmpl;
                
                if (node["name"]) {
                    tmpl.name = node["name"].as<std::string>();
                }
                
                if (node["content"]) {
                    tmpl.content = node["content"].as<std::string>();
                }
                
                if (node["category"]) {
                    tmpl.category = node["category"].as<std::string>();
                }
                
                if (node["shortcut"]) {
                    try {
                        tmpl.shortcut = node["shortcut"].as<int>();
                    } catch (...) {
                        // If shortcut is a string, try to parse it
                        std::string shortcutStr = node["shortcut"].as<std::string>();
                        if (!shortcutStr.empty() && std::isdigit(shortcutStr[0])) {
                            tmpl.shortcut = shortcutStr[0] - '0';
                        }
                    }
                }
                
                templates.push_back(tmpl);
            }
        }
    } catch (const YAML::Exception &e) {
        std::cerr << "Error reading config file: " << e.what() << std::endl;
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    
    return templates;
}

bool ConfigReader::writeConfig(const std::string &filepath, const std::vector<Template> &templates)
{
    try {
        YAML::Emitter out;
        out << YAML::BeginMap;
        out << YAML::Key << "templates";
        out << YAML::Value << YAML::BeginSeq;
        
        for (const auto &tmpl : templates) {
            out << YAML::BeginMap;
            out << YAML::Key << "name" << YAML::Value << tmpl.name;
            out << YAML::Key << "content" << YAML::Value << tmpl.content;
            
            if (!tmpl.category.empty()) {
                out << YAML::Key << "category" << YAML::Value << tmpl.category;
            }
            
            if (tmpl.shortcut > 0) {
                out << YAML::Key << "shortcut" << YAML::Value << tmpl.shortcut;
            }
            
            out << YAML::EndMap;
        }
        
        out << YAML::EndSeq;
        out << YAML::EndMap;
        
        std::ofstream file(filepath);
        if (file.is_open()) {
            file << out.c_str();
            file.close();
            return true;
        }
    } catch (const std::exception &e) {
        std::cerr << "Error writing config file: " << e.what() << std::endl;
    }
    
    return false;
}