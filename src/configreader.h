#ifndef CONFIGREADER_H
#define CONFIGREADER_H

#include <string>
#include <vector>
#include "templatemanager.h"

class ConfigReader {
public:
    ConfigReader();
    ~ConfigReader();
    
    std::vector<Template> readConfig(const std::string &filepath);
    bool writeConfig(const std::string &filepath, const std::vector<Template> &templates);
    
private:
    Template parseTemplate(const void *node);
};

#endif // CONFIGREADER_H