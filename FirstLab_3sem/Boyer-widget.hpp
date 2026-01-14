#pragma once

#include <QWidget>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QLabel>

#include "Boyer-Moore-Horspool.hpp"

class BoyerWidget : public QWidget {
    Q_OBJECT

public:
    explicit BoyerWidget(QWidget *parent = nullptr);

private slots:
    void onSearch();
    void onSearchAll();
    void onClear();

private:
    void setupUI();
    void displayResults(const std::vector<int>& results);

    QLineEdit *textInput = nullptr;
    QLineEdit *patternInput = nullptr;
    QTextEdit *outputText = nullptr;

    QPushButton *searchFirstBtn = nullptr;
    QPushButton *searchAllBtn = nullptr;
    QPushButton *clearBtn = nullptr;

    QLabel *statusLabel = nullptr;
};
