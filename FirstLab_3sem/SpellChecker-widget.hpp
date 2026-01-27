#pragma once

#include <QWidget>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QSpinBox>
#include <QLabel>

#include "SpellChecker.hpp"

#if __has_include(<hunspell/hunspell.hxx>)
#include <hunspell/hunspell.hxx>
#define HUNSPELL_AVAILABLE
#elif __has_include(<hunspell.hxx>)
#include <hunspell.hxx>
#define HUNSPELL_AVAILABLE
#endif

class SpellCheckerWidget : public QWidget {
    Q_OBJECT

public:
    explicit SpellCheckerWidget(QWidget *parent = nullptr);
    ~SpellCheckerWidget();

private slots:
    void onLoadDictionary();
    void onLoadFile();
    void onCheckWord();
    void onCheckText();
    void onAddWord();
    void onClear();

private:
    void setupUI();
    void loadDefaultDictionary();
    bool checkWordWithHunspell(const std::string& word) const;

    SpellChecker checker;
#ifdef HUNSPELL_AVAILABLE
    Hunspell* hunspell = nullptr;
#endif

    QLineEdit *wordInput = nullptr;
    QTextEdit *textInput = nullptr;
    QTextEdit *outputText = nullptr;

    QSpinBox *maxDistanceSpinBox = nullptr;

    QPushButton *checkWordBtn = nullptr;
    QPushButton *checkTextBtn = nullptr;
    QPushButton *addWordBtn = nullptr;
    QPushButton *loadDictBtn = nullptr;
    QPushButton *loadFileBtn = nullptr;
    QPushButton *clearBtn = nullptr;

    QLabel *statusLabel = nullptr;
    QLabel *dictSizeLabel = nullptr;
};
