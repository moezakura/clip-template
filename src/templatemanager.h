#ifndef TEMPLATEMANAGER_H
#define TEMPLATEMANAGER_H

#include <string>
#include <vector>

struct Template {
    std::string name;
    std::string content;
    std::string category;
    int shortcut;
    
    Template() : shortcut(0) {}
};

class TemplateManager {
public:
    TemplateManager();
    ~TemplateManager();
    
    std::vector<Template> loadTemplates();
    bool saveTemplates(const std::vector<Template> &templates);
    
private:
    std::string getConfigPath();
    std::string m_configPath;
};

#endif // TEMPLATEMANAGER_H