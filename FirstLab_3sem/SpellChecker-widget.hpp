#pragma once

#include <QWidget>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QSpinBox>
#include <QLabel>

#include "SpellChecker.hpp"

class SpellCheckerWidget : public QWidget {
    Q_OBJECT

public:
    explicit SpellCheckerWidget(QWidget *parent = nullptr);

private slots:
    void onLoadDictionary();
    void onCheckWord();
    void onCheckText();
    void onAddWord();
    void onClear();

private:
    void setupUI();
    void loadDefaultDictionary();

    SpellChecker checker;

    QLineEdit *wordInput = nullptr;
    QTextEdit *textInput = nullptr;
    QTextEdit *outputText = nullptr;

    QSpinBox *maxDistanceSpinBox = nullptr;

    QPushButton *checkWordBtn = nullptr;
    QPushButton *checkTextBtn = nullptr;
    QPushButton *addWordBtn = nullptr;
    QPushButton *loadDictBtn = nullptr;
    QPushButton *clearBtn = nullptr;

    QLabel *statusLabel = nullptr;
    QLabel *dictSizeLabel = nullptr;
};
