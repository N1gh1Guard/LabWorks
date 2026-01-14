#pragma once

#include <QMainWindow>
#include <QTabWidget>
#include <QLabel>

#include "Boyer-widget.hpp"
#include "NFA-widget.hpp"
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
    NFAWidget *nfaWidget = nullptr;
    SpellCheckerWidget *spellCheckerWidget = nullptr;
    QLabel *statusLabel = nullptr;
};
