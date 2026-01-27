#include "Boyer-widget.hpp"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QString>
#include <QFileDialog>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QMessageBox>
#include <QDir>
#include <QStandardPaths>

BoyerWidget::BoyerWidget(QWidget *parent)
    : QWidget(parent) {
    setupUI();
}

void BoyerWidget::setupUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    
    QHBoxLayout *inputLayout = new QHBoxLayout();
    inputLayout->addWidget(new QLabel("Текст:"));
    textInput = new QTextEdit();
    textInput->setPlaceholderText("Введите текст для поиска...");
    textInput->setMinimumHeight(100);
    inputLayout->addWidget(textInput, 1);
    mainLayout->addLayout(inputLayout);
    
    QHBoxLayout *patternLayout = new QHBoxLayout();
    patternLayout->addWidget(new QLabel("Паттерн:"));
    patternInput = new QLineEdit();
    patternInput->setPlaceholderText("Введите паттерн для поиска...");
    patternLayout->addWidget(patternInput);
    mainLayout->addLayout(patternLayout);
    
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    loadFileBtn = new QPushButton("Загрузить файл");
    connect(loadFileBtn, &QPushButton::clicked, this, &BoyerWidget::onLoadFile);
    buttonLayout->addWidget(loadFileBtn);
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
    
    mainLayout->addWidget(new QLabel("Результаты:"));
    outputText = new QTextEdit();
    outputText->setReadOnly(true);
    mainLayout->addWidget(outputText, 1);
    
    statusLabel = new QLabel("Готово");
    mainLayout->addWidget(statusLabel);
    
    setLayout(mainLayout);
}

void BoyerWidget::onLoadFile() {
    QString initialDir = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);
    if (initialDir.isEmpty()) {
        initialDir = QDir::homePath() + "/Downloads";
    }
    
    QString fileName = QFileDialog::getOpenFileName(this,
        "Загрузить текстовый файл", initialDir,
        "Текстовые файлы (*.txt *.TXT *.Txt);;Все файлы (*.*)");
    
    if (fileName.isEmpty()) {
        return;
    }
    
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Ошибка", 
            QString("Не удалось открыть файл: %1").arg(fileName));
        return;
    }
    
    QTextStream in(&file);
    in.setCodec("UTF-8");
    QString content = in.readAll();
    file.close();
    
    textInput->setPlainText(content);
    statusLabel->setText(QString("✓ Файл загружен: %1").arg(QFileInfo(fileName).fileName()));
}

void BoyerWidget::onSearch() {
    std::string text = textInput->toPlainText().toStdString();
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
    std::string text = textInput->toPlainText().toStdString();
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
