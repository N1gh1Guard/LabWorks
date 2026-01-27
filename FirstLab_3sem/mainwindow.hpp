#pragma once

#include <QMainWindow>
#include <QTabWidget>
#include <QLabel>

#include "Boyer-widget.hpp"
#include "SpaceRemover-widget.hpp"
#include "SpellChecker-widget.hpp"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void onRunAllTests();
    void onAbout();

private:
    void setupUI();
    void createMenuBar();

    QTabWidget *tabWidget = nullptr;
    BoyerWidget *boyerWidget = nullptr;
    SpaceRemoverWidget *spaceRemoverWidget = nullptr;
    SpellCheckerWidget *spellCheckerWidget = nullptr;
    QLabel *statusLabel = nullptr;
};
