#include "SpaceRemover-widget.hpp"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QString>
#include <QMessageBox>

SpaceRemoverWidget::SpaceRemoverWidget(QWidget *parent)
    : QWidget(parent) {
    setupUI();
}

void SpaceRemoverWidget::setupUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    
    mainLayout->addWidget(new QLabel("Удаление множественных пробелов с помощью конечного автомата"));
    
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    processBtn = new QPushButton("Обработать текст");
    clearBtn = new QPushButton("Очистить");
    
    connect(processBtn, &QPushButton::clicked, this, &SpaceRemoverWidget::onProcessText);
    connect(clearBtn, &QPushButton::clicked, this, &SpaceRemoverWidget::onClear);
    
    buttonLayout->addWidget(processBtn);
    buttonLayout->addWidget(clearBtn);
    buttonLayout->addStretch();
    mainLayout->addLayout(buttonLayout);
    
    mainLayout->addWidget(new QLabel("Исходный текст:"));
    inputText = new QTextEdit();
    inputText->setPlaceholderText("Введите текст с множественными пробелами...");
    inputText->setMinimumHeight(100);
    mainLayout->addWidget(inputText, 1);
    
    mainLayout->addWidget(new QLabel("Результат (множественные пробелы удалены):"));
    outputText = new QTextEdit();
    outputText->setReadOnly(true);
    outputText->setMinimumHeight(100);
    mainLayout->addWidget(outputText, 1);
    
    statusLabel = new QLabel("Готово. Введите текст и нажмите 'Обработать текст'");
    mainLayout->addWidget(statusLabel);
    
    setLayout(mainLayout);
}

void SpaceRemoverWidget::onProcessText() {
    QString qtext = inputText->toPlainText();
    
    if (qtext.isEmpty()) {
        statusLabel->setText("Ошибка: введите текст");
        QMessageBox::warning(this, "Ошибка", "Пожалуйста, введите текст для обработки.");
        return;
    }
    
    std::string text = qtext.toStdString();
    std::string result = SpaceRemover::RemoveMultipleSpaces(text);
    auto stats = SpaceRemover::ProcessWithStats(text);
    
    outputText->setPlainText(QString::fromStdString(result));
    QString statsInfo = QString(
        "✓ Обработка завершена!\n\n"
        "Статистика:\n"
        "  Исходная длина: %1 символов\n"
        "  Результат: %2 символов\n"
        "  Пробелов сохранено: %3\n"
        "  Пробелов удалено: %4\n"
        "  Сжатие: %5%%\n\n"
        "Алгоритм: Конечный автомат с 3 состояниями\n"
        "  - Normal: обычные символы\n"
        "  - SingleSpace: первый пробел (сохраняется)\n"
        "  - MultipleSpaces: множественные пробелы (удаляются)"
    ).arg(stats.originalLength)
     .arg(stats.resultLength)
     .arg(stats.spacesPreserved)
     .arg(stats.spacesRemoved)
     .arg(stats.originalLength > 0 ? 
          (100.0 * (stats.originalLength - stats.resultLength) / stats.originalLength) : 0.0, 
          0, 'f', 1);
    
    statusLabel->setText(QString("✓ Обработано: удалено %1 пробелов").arg(stats.spacesRemoved));
    outputText->append("\n\n" + statsInfo);
}

void SpaceRemoverWidget::onClear() {
    inputText->clear();
    outputText->clear();
    statusLabel->setText("Готово. Введите текст и нажмите 'Обработать текст'");
}
