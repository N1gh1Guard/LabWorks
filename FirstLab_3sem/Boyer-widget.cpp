#include "Boyer-widget.hpp"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QString>
#include <QSplitter>

BoyerWidget::BoyerWidget(QWidget *parent)
    : QWidget(parent) {
    setupUI();
}

void BoyerWidget::setupUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    
    // Входные поля
    QHBoxLayout *inputLayout = new QHBoxLayout();
    inputLayout->addWidget(new QLabel("Текст:"));
    textInput = new QLineEdit();
    textInput->setPlaceholderText("Введите текст для поиска...");
    inputLayout->addWidget(textInput);
    
    QHBoxLayout *patternLayout = new QHBoxLayout();
    patternLayout->addWidget(new QLabel("Паттерн:"));
    patternInput = new QLineEdit();
    patternInput->setPlaceholderText("Введите паттерн для поиска...");
    patternLayout->addWidget(patternInput);
    
    mainLayout->addLayout(inputLayout);
    mainLayout->addLayout(patternLayout);
    
    // Кнопки
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    searchFirstBtn = new QPushButton("Найти первое");
    searchAllBtn = new QPushButton("Найти все");
    clearBtn = new QPushButton("Очистить");
    
    connect(searchFirstBtn, &QPushButton::clicked, this, &BoyerWidget::onSearch);
    connect(searchAllBtn, &QPushButton::clicked, this, &BoyerWidget::onSearchAll);
    connect(clearBtn, &QPushButton::clicked, this, &BoyerWidget::onClear);
    
    buttonLayout->addWidget(searchFirstBtn);
    buttonLayout->addWidget(searchAllBtn);
    buttonLayout->addWidget(clearBtn);
    buttonLayout->addStretch();
    mainLayout->addLayout(buttonLayout);
    
    // Выходной текст
    mainLayout->addWidget(new QLabel("Результаты:"));
    outputText = new QTextEdit();
    outputText->setReadOnly(true);
    mainLayout->addWidget(outputText);
    
    // Статус
    statusLabel = new QLabel("Готово");
    mainLayout->addWidget(statusLabel);
    
    setLayout(mainLayout);
}

void BoyerWidget::onSearch() {
    std::string text = textInput->text().toStdString();
    std::string pattern = patternInput->text().toStdString();
    
    if (text.empty() || pattern.empty()) {
        statusLabel->setText("Ошибка: заполните оба поля");
        return;
    }
    
    int pos = BoyerMooreHorspool::SearchFirst(text, pattern);
    
    if (pos != -1) {
        outputText->setText(QString("Найдено на позиции: %1").arg(pos));
        statusLabel->setText("✓ Паттерн найден");
    } else {
        outputText->setText("Паттерн не найден");
        statusLabel->setText("⨯ Паттерн не найден");
    }
}

void BoyerWidget::onSearchAll() {
    std::string text = textInput->text().toStdString();
    std::string pattern = patternInput->text().toStdString();
    
    if (text.empty() || pattern.empty()) {
        statusLabel->setText("Ошибка: заполните оба поля");
        return;
    }
    
    auto results = BoyerMooreHorspool::SearchAll(text, pattern);
    displayResults(results);
}

void BoyerWidget::onClear() {
    textInput->clear();
    patternInput->clear();
    outputText->clear();
    statusLabel->setText("Готово");
}

void BoyerWidget::displayResults(const std::vector<int>& results) {
    if (results.empty()) {
        outputText->setText("Паттерн не найден");
        statusLabel->setText("⨯ Паттерн не найден");
        return;
    }
    
    QString output = QString("Найдено %1 вхождений на позициях:\n").arg(results.size());
    for (size_t i = 0; i < results.size(); ++i) {
        output += QString::number(results[i]);
        if (i < results.size() - 1) output += ", ";
    }
    
    outputText->setText(output);
    statusLabel->setText(QString("✓ Найдено %1 вхождений").arg(results.size()));
}
