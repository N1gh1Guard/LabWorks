#include "NFA-widget.hpp"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QString>

NFAWidget::NFAWidget(QWidget *parent)
    : QWidget(parent), currentDFA(nullptr) {
    setupUI();
    setupExamples();
}

NFAWidget::~NFAWidget() {
    delete currentDFA;
}

void NFAWidget::setupUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    
    QHBoxLayout *exampleLayout = new QHBoxLayout();
    exampleLayout->addWidget(new QLabel("Примеры:"));
    examplesCombo = new QComboBox();
    exampleLayout->addWidget(examplesCombo);
    exampleLayout->addStretch();
    mainLayout->addLayout(exampleLayout);
    
    QHBoxLayout *testLayout = new QHBoxLayout();
    testLayout->addWidget(new QLabel("Проверить строку:"));
    testStringInput = new QLineEdit();
    testStringInput->setPlaceholderText("Введите строку для проверки...");
    testLayout->addWidget(testStringInput);
    mainLayout->addLayout(testLayout);
    
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
    
    mainLayout->addWidget(new QLabel("Результаты:"));
    outputText = new QTextEdit();
    outputText->setReadOnly(true);
    mainLayout->addWidget(outputText, 1);
    
    statusLabel = new QLabel("Готово");
    mainLayout->addWidget(statusLabel);
    
    setLayout(mainLayout);
}

void NFAWidget::setupExamples() {
    examplesCombo->addItem("a|b (a или b)");
    examplesCombo->addItem("ab* (a потом b*)");
    examplesCombo->addItem("(a|b)* (любая комбинация a и b)");
    connect(examplesCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &NFAWidget::onExampleChanged);
}

NFAtoDFAConverter::NFA NFAWidget::createNFAForExample(int index) {
    NFAtoDFAConverter::NFA nfa;
    
    switch (index) {
        case 0: {
            nfa.states = {0, 1, 2, 3};
            nfa.alphabet = {'a', 'b'};
            nfa.initialState = 0;
            nfa.acceptingStates = {3};
            nfa.epsilonTransitions[0] = {1, 2};
            nfa.transitions[{1, 'a'}] = {3};
            nfa.transitions[{2, 'b'}] = {3};
            break;
        }
        case 1: {
            nfa.states = {0, 1, 2};
            nfa.alphabet = {'a', 'b'};
            nfa.initialState = 0;
            nfa.acceptingStates = {2};
            nfa.transitions[{0, 'a'}] = {1};
            nfa.transitions[{1, 'b'}] = {1};
            nfa.epsilonTransitions[1] = {2};
            break;
        }
        case 2: {
            nfa.states = {0, 1, 2};
            nfa.alphabet = {'a', 'b'};
            nfa.initialState = 0;
            nfa.acceptingStates = {0, 2};
            nfa.epsilonTransitions[0] = {1};
            nfa.transitions[{1, 'a'}] = {2};
            nfa.transitions[{1, 'b'}] = {2};
            nfa.epsilonTransitions[2] = {1};
            break;
        }
    }
    
    return nfa;
}

void NFAWidget::onExampleChanged(int /*index*/) {
    delete currentDFA;
    currentDFA = nullptr;
    outputText->clear();
    statusLabel->setText("Выбран новый пример");
}

void NFAWidget::onConvert() {
    int exampleIndex = examplesCombo->currentIndex();
    if (exampleIndex < 0) {
        statusLabel->setText("Ошибка: выберите пример");
        return;
    }
    
    NFAtoDFAConverter::NFA nfa = createNFAForExample(exampleIndex);
    
    try {
        NFAtoDFAConverter::DFA dfa = NFAtoDFAConverter::Convert(nfa);
        delete currentDFA;
        currentDFA = new NFAtoDFAConverter::DFA(dfa);
        
        auto info = NFAtoDFAConverter::GetDFAInfo(dfa);
        QString output = QString(
            "✓ Конверсия NFA в DFA завершена успешно!\n\n"
            "Метод: Powerset Construction\n"
            "Обработка epsilon-переходов: ✓\n\n"
            "Статистика DFA:\n"
            "  Состояний: %1\n"
            "  Переходов: %2\n"
            "  Принимающих состояний: %3\n\n"
            "Используйте 'Проверить строку' для тестирования."
        ).arg(info.statesCount).arg(info.transitionsCount).arg(info.acceptingStatesCount);
        
        outputText->setText(output);
        statusLabel->setText(QString("✓ NFA конвертирован в DFA (%1 состояний)").arg(info.statesCount));
    } catch (const std::exception& e) {
        outputText->setText(QString("Ошибка конверсии: %1").arg(e.what()));
        statusLabel->setText("⨯ Ошибка конверсии");
    }
}

void NFAWidget::onTestString() {
    std::string testStr = testStringInput->text().toStdString();
    
    if (testStr.empty()) {
        statusLabel->setText("Ошибка: введите строку");
        return;
    }
    
    if (!currentDFA) {
        statusLabel->setText("Ошибка: сначала конвертируйте NFA в DFA");
        outputText->setText("Сначала нажмите 'Конвертировать NFA в DFA' для выбранного примера.");
        return;
    }
    
    bool accepted = NFAtoDFAConverter::AcceptsString(*currentDFA, testStr);
    
    QString output = QString("Проверка строки: '%1'\n\n").arg(testStringInput->text());
    
    if (accepted) {
        output += "✓ Строка ПРИНЯТА автоматом\n";
        output += "Строка соответствует регулярному выражению.";
        statusLabel->setText("✓ Строка принята");
    } else {
        output += "⨯ Строка ОТКЛОНЕНА автоматом\n";
        output += "Строка не соответствует регулярному выражению.";
        statusLabel->setText("⨯ Строка отклонена");
    }
    
    outputText->setText(output);
}

void NFAWidget::onVisualize() {
    if (!currentDFA) {
        statusLabel->setText("Ошибка: сначала конвертируйте NFA в DFA");
        outputText->setText("Сначала нажмите 'Конвертировать NFA в DFA' для выбранного примера.");
        return;
    }
    
    QString visualization = NFAtoDFAConverter::VisualizeDFA(*currentDFA).c_str();
    outputText->setText(visualization);
    statusLabel->setText("✓ Визуализация готова");
}

void NFAWidget::onClear() {
    testStringInput->clear();
    outputText->clear();
    statusLabel->setText("Готово");
}
