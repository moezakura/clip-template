#include "templatemanager.h"
#include "configreader.h"
#include <QDir>
#include <QStandardPaths>
#include <QFile>
#include <fstream>

TemplateManager::TemplateManager()
{
    m_configPath = getConfigPath();
}

TemplateManager::~TemplateManager() = default;

std::vector<Template> TemplateManager::loadTemplates()
{
    ConfigReader reader;
    
    // Try user config first
    std::string userConfig = m_configPath + "/templates.yaml";
    if (QFile::exists(QString::fromStdString(userConfig))) {
        return reader.readConfig(userConfig);
    }
    
    // Try system config
    std::string systemConfig = "/usr/share/clip-template/templates.yaml";
    if (QFile::exists(QString::fromStdString(systemConfig))) {
        return reader.readConfig(systemConfig);
    }
    
    // Try local config
    std::string localConfig = "config/templates.yaml";
    if (QFile::exists(QString::fromStdString(localConfig))) {
        return reader.readConfig(localConfig);
    }
    
    // Return empty vector if no config found
    return std::vector<Template>();
}

bool TemplateManager::saveTemplates(const std::vector<Template> &templates)
{
    ConfigReader reader;
    std::string userConfig = m_configPath + "/templates.yaml";
    return reader.writeConfig(userConfig, templates);
}

std::string TemplateManager::getConfigPath()
{
    QString configPath = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation);
    configPath += "/clip-template";
    
    QDir dir;
    if (!dir.exists(configPath)) {
        dir.mkpath(configPath);
    }
    
    return configPath.toStdString();
}