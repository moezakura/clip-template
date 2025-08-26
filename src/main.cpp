#include <QApplication>
#include <QFile>
#include <QDir>
#include <iostream>
#include "mainwindow.h"

void ensureConfigExists()
{
    QString homeConfig = QDir::homePath() + "/.config/clip-template/templates.yaml";
    QFile configFile(homeConfig);
    
    if (!configFile.exists()) {
        // Check if local config exists
        QFile localConfig("config/templates.yaml");
        if (localConfig.exists()) {
            // Copy local config to user config
            QDir().mkpath(QDir::homePath() + "/.config/clip-template");
            localConfig.copy(homeConfig);
        }
    }
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("clip-template");
    app.setOrganizationName("ClipTemplate");
    
    // Ensure config exists
    ensureConfigExists();
    
    // Create and show main window
    MainWindow window;
    window.show();
    
    return app.exec();
}