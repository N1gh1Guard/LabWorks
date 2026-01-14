#include "mainwindow.hpp"
#include "tests.hpp"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QMessageBox>
#include <QSplitter>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    setWindowTitle("Stream Lab - Алгоритмы и потоки (C++17)");
    setGeometry(100, 100, 1200, 700);
    
    setupUI();
    createMenuBar();
    
    setStyleSheet(R"(
        QMainWindow {
            background-color: #000000;
        }
        QTabWidget::pane {
            border: 1px solid #cccccc;
        }
        QTabBar::tab {
            padding: 8px 20px;
            margin-right: 2px;
        }
    )");
}

MainWindow::~MainWindow() {
}

void MainWindow::setupUI() {
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    
    // Создаем вкладки
    tabWidget = new QTabWidget();
    boyerWidget = new BoyerWidget();
    nfaWidget = new NFAWidget();
    spellCheckerWidget = new SpellCheckerWidget();
    
    tabWidget->addTab(boyerWidget, "Boyer-Moore-Horspool");
    tabWidget->addTab(nfaWidget, "NFA → DFA");
    tabWidget->addTab(spellCheckerWidget, "Spell Checker");
    
    mainLayout->addWidget(tabWidget);
    
    // Нижняя панель статуса
    statusLabel = new QLabel("Готово");
    mainLayout->addWidget(statusLabel);
    
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);
}

void MainWindow::createMenuBar() {
    QMenuBar *menuBar = new QMenuBar(this);
    setMenuBar(menuBar);
    
    // Меню File
    QMenu *fileMenu = menuBar->addMenu("&Файл");
    QAction *exitAction = fileMenu->addAction("&Выход");
    connect(exitAction, &QAction::triggered, this, &QWidget::close);
    
    // Меню Tools
    QMenu *toolsMenu = menuBar->addMenu("&Инструменты");
    QAction *testsAction = toolsMenu->addAction("&Запустить все тесты");
    connect(testsAction, &QAction::triggered, this, &MainWindow::onRunAllTests);
    
    // Меню Help
    QMenu *helpMenu = menuBar->addMenu("&Справка");
    QAction *aboutAction = helpMenu->addAction("&О программе");
    connect(aboutAction, &QAction::triggered, this, &MainWindow::onAbout);
}

void MainWindow::onRunAllTests() {
    statusLabel->setText("Запуск тестов...");
    try {
        AlgorithmTests::RunAllTests();
        QMessageBox::information(this, "Тесты", "✓ Все тесты пройдены успешно!");
        statusLabel->setText("Тесты завершены");
    } catch (const std::exception &e) {
        QMessageBox::critical(this, "Ошибка тестирования", 
                            QString("Ошибка: %1").arg(e.what()));
        statusLabel->setText("Ошибка при тестировании");
    }
}

void MainWindow::onAbout() {
    QMessageBox::about(this, "О программе",
        "<h2>Stream Lab v1.0</h2>"
        "<p>Демонстрация алгоритмов на потоках строк</p>"
        "<p><b>Алгоритмы:</b></p>"
        "<ul>"
        "<li>Boyer-Moore-Horspool - оптимизированный поиск подстроки</li>"
        "<li>NFA to DFA - конверсия автоматов</li>"
        "<li>Spell Checker - проверка орфографии</li>"
        "</ul>"
        "<p><b>Язык:</b> C++17</p>"
        "<p><b>ОС:</b> Arch Linux</p>");
}
