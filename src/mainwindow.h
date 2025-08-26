#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QShortcut>
#include <memory>
#include <vector>
#include "templatemanager.h"
#include "clipboardhandler.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void showEvent(QShowEvent *event) override;

private slots:
    void onItemActivated(QListWidgetItem *item);
    void onSearchTextChanged(const QString &text);
    void onEscapePressed();
    void handleNumberKey(int number);

private:
    void setupUI();
    void setupShortcuts();
    void loadTemplates();
    void filterTemplates(const QString &filter);
    void copyAndPaste();
    void rememberActiveWindow();
    
    QListWidget *m_templateList;
    QLineEdit *m_searchBox;
    std::unique_ptr<TemplateManager> m_templateManager;
    std::unique_ptr<ClipboardHandler> m_clipboardHandler;
    std::vector<Template> m_templates;
    std::vector<Template> m_filteredTemplates;
    Window m_previousWindow;
};

#endif // MAINWINDOW_H