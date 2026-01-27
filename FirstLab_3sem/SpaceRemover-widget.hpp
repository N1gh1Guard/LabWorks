#pragma once

#include <QWidget>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QLabel>

#include "SpaceRemover.hpp"

class SpaceRemoverWidget : public QWidget {
    Q_OBJECT

public:
    explicit SpaceRemoverWidget(QWidget *parent = nullptr);

private slots:
    void onProcessText();
    void onClear();

private:
    void setupUI();

    QTextEdit *inputText = nullptr;
    QTextEdit *outputText = nullptr;

    QPushButton *processBtn = nullptr;
    QPushButton *clearBtn = nullptr;

    QLabel *statusLabel = nullptr;
};
