#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QShortcut>
#include <QClipboard>
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
    void onClipboardChanged(QClipboard::Mode mode);

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

    // Clipboard restore support
    QMimeData *m_savedClipboardData = nullptr; // owned until restored via setMimeData
    bool m_monitorClipboard = false;
    bool m_ignoreNextClipboardChange = false;
    QTimer *m_quitTimer = nullptr;
};

#endif // MAINWINDOW_H
