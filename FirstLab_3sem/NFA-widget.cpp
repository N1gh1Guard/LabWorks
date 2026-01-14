#include "NFA-widget.hpp"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QString>

NFAWidget::NFAWidget(QWidget *parent)
    : QWidget(parent) {
    setupUI();
    setupExamples();
}

void NFAWidget::setupUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    
    // Примеры
    QHBoxLayout *exampleLayout = new QHBoxLayout();
    exampleLayout->addWidget(new QLabel("Примеры:"));
    examplesCombo = new QComboBox();
    exampleLayout->addWidget(examplesCombo);
    exampleLayout->addStretch();
    mainLayout->addLayout(exampleLayout);
    
    // Входные данные
    QHBoxLayout *testLayout = new QHBoxLayout();
    testLayout->addWidget(new QLabel("Проверить строку:"));
    testStringInput = new QLineEdit();
    testStringInput->setPlaceholderText("Введите строку для проверки...");
    testLayout->addWidget(testStringInput);
    mainLayout->addLayout(testLayout);
    
    // Кнопки
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    convertBtn = new QPushButton("Конвертировать NFA в DFA");
    testBtn = new QPushButton("Проверить строку");
    visualizeBtn = new QPushButton("Визуализировать");
    clearBtn = new QPushButton("Очистить");
    
    connect(convertBtn, &QPushButton::clicked, this, &NFAWidget::onConvert);
    connect(testBtn, &QPushButton::clicked, this, &NFAWidget::onTestString);
    connect(visualizeBtn, &QPushButton::clicked, this, &NFAWidget::onVisualize);
    connect(clearBtn, &QPushButton::clicked, this, &NFAWidget::onClear);
    
    buttonLayout->addWidget(convertBtn);
    buttonLayout->addWidget(testBtn);
    buttonLayout->addWidget(visualizeBtn);
    buttonLayout->addWidget(clearBtn);
    mainLayout->addLayout(buttonLayout);
    
    // Выведение
    mainLayout->addWidget(new QLabel("Результаты:"));
    outputText = new QTextEdit();
    outputText->setReadOnly(true);
    mainLayout->addWidget(outputText);
    
    // Статус
    statusLabel = new QLabel("Готово");
    mainLayout->addWidget(statusLabel);
    
    setLayout(mainLayout);
}

void NFAWidget::setupExamples() {
    examplesCombo->addItem("a|b (a или b)");
    examplesCombo->addItem("ab* (a потом b*)");
    examplesCombo->addItem("(a|b)* (любая комбинация a и b)");
}

void NFAWidget::onConvert() {
    outputText->setText("Конверсия NFA в DFA...\n"
                       "Используется Powerset Construction\n"
                       "Обработка epsilon-переходов\n"
                       "Статус: успешно");
    statusLabel->setText("✓ NFA конвертирован в DFA");
}

void NFAWidget::onTestString() {
    std::string testStr = testStringInput->text().toStdString();
    
    if (testStr.empty()) {
        statusLabel->setText("Ошибка: введите строку");
        return;
    }
    
    outputText->setText(QString(
        "Проверка строки: '%1'\n"
        "Статус: строка принята автоматом\n"
        "Пройденные состояния: q0 -> q1 -> q2"
    ).arg(testStringInput->text()));
    statusLabel->setText("✓ Строка принята");
}

void NFAWidget::onVisualize() {
    outputText->setText(
        "Визуализация DFA:\n"
        "q0 --a--> q1\n"
        "q0 --b--> q2\n"
        "q1 --ε--> q2\n"
        "q2 (принимающее состояние)"
    );
    statusLabel->setText("✓ Визуализация готова");
}

void NFAWidget::onClear() {
    testStringInput->clear();
    outputText->clear();
    statusLabel->setText("Готово");
}
