#include "mainwindow.h"
#include "keyboardhandler.h"
#include <QKeyEvent>
#include <QShowEvent>
#include <QApplication>
#include <QDesktopWidget>
#include <QTimer>
#include <QClipboard>
#include <QMimeData>
#include <QDebug>

// X11 headers must be included after Qt headers to avoid conflicts
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#undef None
#undef KeyPress
#undef KeyRelease
#undef FocusIn
#undef FocusOut

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_templateManager(std::make_unique<TemplateManager>())
    , m_clipboardHandler(std::make_unique<ClipboardHandler>())
    , m_previousWindow(0)
{
    setupUI();
    setupShortcuts();
    loadTemplates();
    rememberActiveWindow();

    // Setup clipboard monitoring and quit timer (for safe restoration on X11 without managers)
    m_quitTimer = new QTimer(this);
    m_quitTimer->setSingleShot(true);
    connect(m_quitTimer, &QTimer::timeout, []() {
        qDebug() << "[clip-template] Quit timer elapsed; exiting.";
        QApplication::quit();
    });

    QClipboard *cb = QApplication::clipboard();
    connect(cb, &QClipboard::changed, this, &MainWindow::onClipboardChanged);
}

MainWindow::~MainWindow() = default;

void MainWindow::setupUI()
{
    // Create central widget
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    // Create layout
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);
    
    // Create search box
    m_searchBox = new QLineEdit(this);
    m_searchBox->setPlaceholderText("検索 (/ キーでフォーカス)...");
    connect(m_searchBox, &QLineEdit::textChanged, this, &MainWindow::onSearchTextChanged);
    layout->addWidget(m_searchBox);
    
    // Create template list
    m_templateList = new QListWidget(this);
    m_templateList->setFocusPolicy(Qt::StrongFocus);
    connect(m_templateList, &QListWidget::itemActivated, this, &MainWindow::onItemActivated);
    layout->addWidget(m_templateList);
    
    // Window settings
    setWindowTitle("Template Selector");
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);
    
    // Set window size and position
    resize(400, 500);
    QRect screenGeometry = QApplication::desktop()->screenGeometry();
    int x = (screenGeometry.width() - width()) / 2;
    int y = (screenGeometry.height() - height()) / 2;
    move(x, y);
    
    // Style
    setStyleSheet(R"(
        QMainWindow {
            background-color: rgba(30, 30, 30, 240);
            border: 2px solid #3daee9;
            border-radius: 8px;
        }
        QLineEdit {
            background-color: rgba(50, 50, 50, 255);
            color: white;
            border: 1px solid #3daee9;
            border-radius: 4px;
            padding: 8px;
            font-size: 14px;
        }
        QListWidget {
            background-color: rgba(40, 40, 40, 255);
            color: white;
            border: none;
            border-radius: 4px;
            padding: 4px;
            font-size: 14px;
        }
        QListWidget::item {
            padding: 8px;
            border-radius: 4px;
        }
        QListWidget::item:selected {
            background-color: #3daee9;
        }
        QListWidget::item:hover {
            background-color: rgba(61, 174, 233, 100);
        }
    )");
}

void MainWindow::setupShortcuts()
{
    // Escape to quit
    QShortcut *escShortcut = new QShortcut(QKeySequence(Qt::Key_Escape), this);
    connect(escShortcut, &QShortcut::activated, this, &MainWindow::onEscapePressed);
    
    // Slash for search
    QShortcut *searchShortcut = new QShortcut(QKeySequence(Qt::Key_Slash), this);
    connect(searchShortcut, &QShortcut::activated, [this]() {
        m_searchBox->setFocus();
        m_searchBox->selectAll();
    });
    
    // Number keys for quick selection
    for (int i = 1; i <= 9; ++i) {
        QShortcut *numShortcut = new QShortcut(QKeySequence(Qt::Key_0 + i), this);
        connect(numShortcut, &QShortcut::activated, [this, i]() {
            handleNumberKey(i);
        });
    }
}

void MainWindow::loadTemplates()
{
    m_templates = m_templateManager->loadTemplates();
    m_filteredTemplates = m_templates;
    
    m_templateList->clear();
    for (const auto &tmpl : m_filteredTemplates) {
        QString displayText = QString("[%1] %2").arg(tmpl.shortcut).arg(QString::fromStdString(tmpl.name));
        if (!tmpl.category.empty()) {
            displayText = QString("[%1] %2 (%3)").arg(tmpl.shortcut).arg(QString::fromStdString(tmpl.name)).arg(QString::fromStdString(tmpl.category));
        }
        m_templateList->addItem(displayText);
    }
    
    if (m_templateList->count() > 0) {
        m_templateList->setCurrentRow(0);
    }
}

void MainWindow::filterTemplates(const QString &filter)
{
    m_filteredTemplates.clear();
    QString lowerFilter = filter.toLower();
    
    for (const auto &tmpl : m_templates) {
        QString name = QString::fromStdString(tmpl.name).toLower();
        QString category = QString::fromStdString(tmpl.category).toLower();
        QString content = QString::fromStdString(tmpl.content).toLower();
        
        if (name.contains(lowerFilter) || 
            category.contains(lowerFilter) || 
            content.contains(lowerFilter)) {
            m_filteredTemplates.push_back(tmpl);
        }
    }
    
    m_templateList->clear();
    for (const auto &tmpl : m_filteredTemplates) {
        QString displayText = QString("[%1] %2").arg(tmpl.shortcut).arg(QString::fromStdString(tmpl.name));
        if (!tmpl.category.empty()) {
            displayText = QString("[%1] %2 (%3)").arg(tmpl.shortcut).arg(QString::fromStdString(tmpl.name)).arg(QString::fromStdString(tmpl.category));
        }
        m_templateList->addItem(displayText);
    }
    
    if (m_templateList->count() > 0) {
        m_templateList->setCurrentRow(0);
    }
}

void MainWindow::copyAndPaste()
{
    int currentRow = m_templateList->currentRow();
    if (currentRow >= 0 && currentRow < static_cast<int>(m_filteredTemplates.size())) {
        const Template &tmpl = m_filteredTemplates[currentRow];

        // Remember current clipboard data to restore later
        QClipboard *clipboard = QApplication::clipboard();
        const QMimeData *orig = clipboard->mimeData(QClipboard::Clipboard);
        if (m_savedClipboardData) {
            delete m_savedClipboardData;
            m_savedClipboardData = nullptr;
        }
        if (orig) {
            m_savedClipboardData = new QMimeData();
            const QStringList formats = orig->formats();
            for (const QString &fmt : formats) {
                m_savedClipboardData->setData(fmt, orig->data(fmt));
            }
            qDebug() << "[clip-template] Saved clipboard formats:" << formats;
        } else {
            qDebug() << "[clip-template] No original clipboard data present.";
        }

        // Copy to clipboard
        m_clipboardHandler->copyToClipboard(tmpl.content);
        qDebug() << "[clip-template] Set clipboard to template (length)" << (int)tmpl.content.size();

        // Hide window
        hide();

        // Restore focus to previous window and paste
        QTimer::singleShot(100, [this]() {
            m_clipboardHandler->pasteToWindow(m_previousWindow);
            // After paste keystrokes are sent, restore clipboard data shortly after
            QTimer::singleShot(150, [this]() {
                QClipboard *cb = QApplication::clipboard();
                if (m_savedClipboardData) {
                    m_ignoreNextClipboardChange = true; // ignore our own change signal
                    qDebug() << "[clip-template] Restoring previous clipboard data.";
                    cb->setMimeData(m_savedClipboardData, QClipboard::Clipboard); // ownership transferred
                    m_savedClipboardData = nullptr;
                    // Monitor for changes and quit when someone else takes over, or timeout
                    m_monitorClipboard = true;
                    m_quitTimer->start(10000); // 10s safety timeout
                } else {
                    qDebug() << "[clip-template] No saved clipboard data to restore; exiting.";
                    QApplication::quit();
                }
            });
        });
    }
}

void MainWindow::rememberActiveWindow()
{
    Display *display = XOpenDisplay(nullptr);
    if (display) {
        Window focusWindow;
        int revertTo;
        XGetInputFocus(display, &focusWindow, &revertTo);
        
        if (focusWindow != 0 && focusWindow != PointerRoot) {
            m_previousWindow = focusWindow;
        }
        
        XCloseDisplay(display);
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
        if (m_searchBox->hasFocus()) {
            m_templateList->setFocus();
        } else {
            // Let the default behavior (itemActivated signal) handle it
            event->ignore();
            return;
        }
    } else if (event->key() == Qt::Key_Up || event->key() == Qt::Key_Down) {
        if (m_searchBox->hasFocus()) {
            m_templateList->setFocus();
        }
    } else if (event->key() == Qt::Key_Tab) {
        if (m_searchBox->hasFocus()) {
            m_templateList->setFocus();
        } else {
            m_searchBox->setFocus();
        }
    } else {
        QMainWindow::keyPressEvent(event);
    }
}

void MainWindow::showEvent(QShowEvent *event)
{
    QMainWindow::showEvent(event);
    m_templateList->setFocus();
}

void MainWindow::onItemActivated(QListWidgetItem *item)
{
    Q_UNUSED(item)
    copyAndPaste();
}

void MainWindow::onSearchTextChanged(const QString &text)
{
    filterTemplates(text);
}

void MainWindow::onEscapePressed()
{
    QApplication::quit();
}

void MainWindow::handleNumberKey(int number)
{
    // Find template with this shortcut
    for (size_t i = 0; i < m_filteredTemplates.size(); ++i) {
        if (m_filteredTemplates[i].shortcut == number) {
            m_templateList->setCurrentRow(i);
            copyAndPaste();
            break;
        }
    }
}

void MainWindow::onClipboardChanged(QClipboard::Mode mode)
{
    if (mode != QClipboard::Clipboard) return;
    if (!m_monitorClipboard) return;
    if (m_ignoreNextClipboardChange) {
        // This change was caused by our own restore
        m_ignoreNextClipboardChange = false;
        qDebug() << "[clip-template] Clipboard changed (self-restore). Monitoring for external change.";
        return;
    }

    qDebug() << "[clip-template] Clipboard changed by external owner; exiting.";
    QApplication::quit();
}
