#pragma once

#include <QWidget>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QComboBox>
#include <QLabel>

#include "NFA-to-DFA.hpp"

class NFAWidget : public QWidget {
    Q_OBJECT

public:
    explicit NFAWidget(QWidget *parent = nullptr);

private slots:
    void onConvert();
    void onTestString();
    void onVisualize();
    void onClear();

private:
    void setupUI();
    void setupExamples();

    QLineEdit *testStringInput = nullptr;
    QTextEdit *outputText = nullptr;

    QComboBox *examplesCombo = nullptr;

    QPushButton *convertBtn = nullptr;
    QPushButton *testBtn = nullptr;
    QPushButton *visualizeBtn = nullptr;
    QPushButton *clearBtn = nullptr;

    QLabel *statusLabel = nullptr;
};
