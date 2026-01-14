#include "SpellChecker-widget.hpp"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QString>
#include <QGroupBox>

SpellCheckerWidget::SpellCheckerWidget(QWidget *parent)
    : QWidget(parent) {
    setupUI();
    loadDefaultDictionary();
}

void SpellCheckerWidget::setupUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    
    // Информация о словаре
    QHBoxLayout *dictLayout = new QHBoxLayout();
    dictLayout->addWidget(new QLabel("Размер словаря:"));
    dictSizeLabel = new QLabel("0");
    dictLayout->addWidget(dictSizeLabel);
    loadDictBtn = new QPushButton("Загрузить словарь");
    connect(loadDictBtn, &QPushButton::clicked, this, &SpellCheckerWidget::onLoadDictionary);
    dictLayout->addWidget(loadDictBtn);
    dictLayout->addStretch();
    mainLayout->addLayout(dictLayout);
    
    // Проверка слова
    QGroupBox *wordGroup = new QGroupBox("Проверка слова");
    QVBoxLayout *wordLayout = new QVBoxLayout();
    QHBoxLayout *wordInputLayout = new QHBoxLayout();
    wordInputLayout->addWidget(new QLabel("Слово:"));
    wordInput = new QLineEdit();
    wordInput->setPlaceholderText("Введите слово...");
    wordInputLayout->addWidget(wordInput);
    checkWordBtn = new QPushButton("Проверить");
    connect(checkWordBtn, &QPushButton::clicked, this, &SpellCheckerWidget::onCheckWord);
    wordInputLayout->addWidget(checkWordBtn);
    addWordBtn = new QPushButton("Добавить");
    connect(addWordBtn, &QPushButton::clicked, this, &SpellCheckerWidget::onAddWord);
    wordInputLayout->addWidget(addWordBtn);
    wordLayout->addLayout(wordInputLayout);
    
    QHBoxLayout *distanceLayout = new QHBoxLayout();
    distanceLayout->addWidget(new QLabel("Макс. расстояние:"));
    maxDistanceSpinBox = new QSpinBox();
    maxDistanceSpinBox->setValue(2);
    maxDistanceSpinBox->setRange(1, 5);
    distanceLayout->addWidget(maxDistanceSpinBox);
    distanceLayout->addStretch();
    wordLayout->addLayout(distanceLayout);
    
    wordGroup->setLayout(wordLayout);
    mainLayout->addWidget(wordGroup);
    
    // Проверка текста
    QGroupBox *textGroup = new QGroupBox("Проверка текста");
    QVBoxLayout *textLayout = new QVBoxLayout();
    textLayout->addWidget(new QLabel("Текст:"));
    textInput = new QTextEdit();
    textInput->setPlaceholderText("Введите текст для проверки...");
    textInput->setMaximumHeight(100);
    textLayout->addWidget(textInput);
    checkTextBtn = new QPushButton("Проверить текст");
    connect(checkTextBtn, &QPushButton::clicked, this, &SpellCheckerWidget::onCheckText);
    textLayout->addWidget(checkTextBtn);
    textGroup->setLayout(textLayout);
    mainLayout->addWidget(textGroup);
    
    // Результаты
    mainLayout->addWidget(new QLabel("Результаты:"));
    outputText = new QTextEdit();
    outputText->setReadOnly(true);
    mainLayout->addWidget(outputText);
    
    // Кнопки
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    clearBtn = new QPushButton("Очистить");
    connect(clearBtn, &QPushButton::clicked, this, &SpellCheckerWidget::onClear);
    buttonLayout->addStretch();
    buttonLayout->addWidget(clearBtn);
    mainLayout->addLayout(buttonLayout);
    
    // Статус
    statusLabel = new QLabel("Готово");
    mainLayout->addWidget(statusLabel);
    
    setLayout(mainLayout);
}

void SpellCheckerWidget::loadDefaultDictionary() {
    std::set<std::string> dict = {
        "hello", "world", "test", "program", "algorithm",
        "computer", "string", "function", "class", "object",
        "variable", "pointer", "reference", "template", "library"
    };
    checker.LoadDictionary(dict);
    dictSizeLabel->setText(QString::number(checker.GetDictionarySize()));
}

void SpellCheckerWidget::onLoadDictionary() {
    loadDefaultDictionary();
    statusLabel->setText(QString("✓ Загружено %1 слов").arg(checker.GetDictionarySize()));
}

void SpellCheckerWidget::onCheckWord() {
    std::string word = wordInput->text().toStdString();
    
    if (word.empty()) {
        statusLabel->setText("Ошибка: введите слово");
        return;
    }
    
    int maxDist = maxDistanceSpinBox->value();
    auto result = checker.CheckWord(word, maxDist);
    
    QString output;
    if (result.isCorrect) {
        output = QString("✓ Слово '%1' написано правильно").arg(QString::fromStdString(word));
    } else {
        output = QString("⨯ Слово '%1' написано неправильно\n\nПодсказки:\n").arg(QString::fromStdString(word));
        for (size_t i = 0; i < result.suggestions.size(); ++i) {
            output += QString("  %1. %2\n").arg(i+1).arg(QString::fromStdString(result.suggestions[i]));
        }
    }
    
    outputText->setText(output);
    statusLabel->setText(result.isCorrect ? "✓ Слово правильное" : "⨯ Слово неправильное");
}

void SpellCheckerWidget::onCheckText() {
    std::string text = textInput->toPlainText().toStdString();
    
    if (text.empty()) {
        statusLabel->setText("Ошибка: введите текст");
        return;
    }
    
    auto result = checker.CheckText(text);
    
    QString output = QString(
        "Всего слов: %1\n"
        "Ошибок: %2\n"
        "Процент ошибок: %3%\n\n"
    ).arg(result.totalWords).arg(result.errorCount).arg(result.errorPercentage, 0, 'f', 1);
    
    if (result.errorCount > 0) {
        output += "Неправильные слова:\n";
        for (const auto& res : result.results) {
            if (!res.isCorrect) {
                output += QString("  - %1\n").arg(QString::fromStdString(res.word));
            }
        }
    } else {
        output += "✓ Все слова написаны правильно!";
    }
    
    outputText->setText(output);
    statusLabel->setText(QString("✓ Проверено %1 слов").arg(result.totalWords));
}

void SpellCheckerWidget::onAddWord() {
    std::string word = wordInput->text().toStdString();
    
    if (word.empty()) {
        statusLabel->setText("Ошибка: введите слово");
        return;
    }
    
    checker.AddWord(word);
    dictSizeLabel->setText(QString::number(checker.GetDictionarySize()));
    outputText->setText(QString("✓ Слово '%1' добавлено в словарь").arg(QString::fromStdString(word)));
    statusLabel->setText("✓ Слово добавлено");
    wordInput->clear();
}

void SpellCheckerWidget::onClear() {
    wordInput->clear();
    textInput->clear();
    outputText->clear();
    statusLabel->setText("Готово");
}
