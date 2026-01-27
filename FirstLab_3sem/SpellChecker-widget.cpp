#include "SpellChecker-widget.hpp"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QSpinBox>
#include <QMessageBox>
#include <QFile>
#include <QFileInfo>
#include <QFileDialog>
#include <QTextStream>
#include <QTextCharFormat>
#include <QTextCursor>
#include <QString>
#include <QDir>
#include <QStandardPaths>
#include <QRegularExpression>
#include <fstream>

SpellCheckerWidget::SpellCheckerWidget(QWidget *parent)
    : QWidget(parent) {
    setupUI();
    loadDefaultDictionary();
}

SpellCheckerWidget::~SpellCheckerWidget() {
#ifdef HUNSPELL_AVAILABLE
    if (hunspell) {
        delete hunspell;
        hunspell = nullptr;
    }
#endif
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
    textInput->setMinimumHeight(100);
    textLayout->addWidget(textInput, 1);
    
    QHBoxLayout *textButtonLayout = new QHBoxLayout();
    loadFileBtn = new QPushButton("Загрузить файл");
    connect(loadFileBtn, &QPushButton::clicked, this, &SpellCheckerWidget::onLoadFile);
    textButtonLayout->addWidget(loadFileBtn);
    checkTextBtn = new QPushButton("Проверить текст");
    connect(checkTextBtn, &QPushButton::clicked, this, &SpellCheckerWidget::onCheckText);
    textButtonLayout->addWidget(checkTextBtn);
    textButtonLayout->addStretch();
    textLayout->addLayout(textButtonLayout);
    textGroup->setLayout(textLayout);
    mainLayout->addWidget(textGroup);

    // Результаты
    mainLayout->addWidget(new QLabel("Результаты:"));
    outputText = new QTextEdit();
    outputText->setReadOnly(true);
    mainLayout->addWidget(outputText, 1);

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

std::set<std::string> LoadRussianDictionaryFromHunspell() {
    std::set<std::string> dictionary;
    
    std::vector<std::string> possiblePaths = {
        "/usr/share/hunspell/ru_RU.dic",
        "/usr/share/dict/russian",
        "/usr/share/myspell/dicts/ru_RU.dic",
        "/etc/dictionaries-common/words/russian"
    };
    
    for (const auto& path : possiblePaths) {
        std::ifstream file(path, std::ios::binary);
        if (file.is_open()) {
            std::string line;
            int lineCount = 0;
            
            std::getline(file, line);
            
            while (std::getline(file, line) && lineCount < 500000) {
                if (!line.empty()) {
                    size_t slashPos = line.find('/');
                    if (slashPos != std::string::npos) {
                        line = line.substr(0, slashPos);
                    }
                    
                    QString qLine = QString::fromUtf8(line.c_str()).trimmed();
                    if (!qLine.isEmpty() && !qLine.startsWith('#')) {
                        qLine = qLine.toLower();
                        dictionary.insert(qLine.toUtf8().toStdString());
                        lineCount++;
                    }
                }
            }
            
            file.close();
            
            if (!dictionary.empty()) {
                return dictionary;
            }
        }
    }
    
    return dictionary;
}

void SpellCheckerWidget::loadDefaultDictionary() {
#ifdef HUNSPELL_AVAILABLE
    // Пытаемся инициализировать Hunspell библиотеку
    std::vector<std::string> possibleDicPaths = {
        "/usr/share/hunspell/ru_RU.dic",
        "/usr/share/myspell/dicts/ru_RU.dic"
    };
    std::vector<std::string> possibleAffPaths = {
        "/usr/share/hunspell/ru_RU.aff",
        "/usr/share/myspell/dicts/ru_RU.aff"
    };
    
    for (size_t i = 0; i < possibleDicPaths.size(); ++i) {
        QFile dicFile(QString::fromStdString(possibleDicPaths[i]));
        QFile affFile(QString::fromStdString(possibleAffPaths[i]));
        if (dicFile.exists() && affFile.exists()) {
            hunspell = new Hunspell(possibleAffPaths[i].c_str(), possibleDicPaths[i].c_str());
            break;
        }
    }
#endif
    
    // Загружаем словарь для подсказок
    std::set<std::string> dict = LoadRussianDictionaryFromHunspell();
    
    if (dict.empty()) {
        // Fallback: показываем ошибку и предлагаем установить пакет
        statusLabel->setText("⚠️ Словарь не найден. Установи: sudo pacman -S hunspell-ru");
        dictSizeLabel->setText("0");
        QMessageBox::warning(this, "Словарь не найден",
            "Русский словарь не найден!\n\n"
            "Пожалуйста установи пакет:\n"
            "sudo pacman -S hunspell-ru");
        return;
    }
    
    checker.LoadDictionary(dict);
    dictSizeLabel->setText(QString::number(checker.GetDictionarySize()));
    
#ifdef HUNSPELL_AVAILABLE
    if (hunspell) {
        statusLabel->setText(QString("✓ Загружено %1 русских слов (Hunspell с склонениями)")
                            .arg(checker.GetDictionarySize()));
    } else {
        statusLabel->setText(QString("✓ Загружено %1 русских слов из Hunspell")
                            .arg(checker.GetDictionarySize()));
    }
#else
    statusLabel->setText(QString("✓ Загружено %1 русских слов из Hunspell")
                        .arg(checker.GetDictionarySize()));
#endif
}

bool SpellCheckerWidget::checkWordWithHunspell(const std::string& word) const {
#ifdef HUNSPELL_AVAILABLE
    if (hunspell) {
        return hunspell->spell(word) != 0;
    }
#endif
    return checker.IsCorrect(word);
}

void SpellCheckerWidget::onLoadDictionary() {
    loadDefaultDictionary();
}

void SpellCheckerWidget::onLoadFile() {
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

void SpellCheckerWidget::onCheckWord() {
    std::string word = wordInput->text().toStdString();
    if (word.empty()) {
        statusLabel->setText("Ошибка: введите слово");
        return;
    }

    bool isCorrect = checkWordWithHunspell(word);
    
    QString output;
    if (isCorrect) {
        output = QString("✓ Слово '%1' написано правильно").arg(QString::fromStdString(word));
    } else {
        int maxDist = maxDistanceSpinBox->value();
        auto result = checker.CheckWord(word, maxDist);
        output = QString("⨯ Слово '%1' написано неправильно\n\nПодсказки:\n").arg(QString::fromStdString(word));
        for (size_t i = 0; i < result.suggestions.size(); ++i) {
            output += QString(" %1. %2\n").arg(i+1).arg(QString::fromStdString(result.suggestions[i]));
        }
    }

    outputText->setText(output);
    statusLabel->setText(isCorrect ? "✓ Слово правильное" : "⨯ Слово неправильное");
}

void SpellCheckerWidget::onCheckText() {
    QString qtext = textInput->toPlainText();
    if (qtext.isEmpty()) {
        statusLabel->setText("Ошибка: введите текст");
        return;
    }

    std::string text = qtext.toStdString();
    auto result = checker.CheckText(text);

    QTextCharFormat normalFormat;
    QTextCharFormat errorFormat;
    errorFormat.setUnderlineStyle(QTextCharFormat::SingleUnderline);
    errorFormat.setUnderlineColor(Qt::red);
    errorFormat.setForeground(Qt::red);

    QTextCursor cursor(textInput->document());
    cursor.select(QTextCursor::Document);
    cursor.setCharFormat(normalFormat);
    cursor.clearSelection();

    int wordIndex = 0;
    QString currentWord;
    int wordStartPos = 0;
    int errorCount = 0;
    int totalWords = 0;
    
    for (int i = 0; i < qtext.length(); ++i) {
        QChar c = qtext[i];
        
        if (c.isLetter() || c == '\'' || c == '-') {
            if (currentWord.isEmpty()) {
                wordStartPos = i;
            }
            currentWord += c;
        } else {
            if (!currentWord.isEmpty()) {
                std::string word = currentWord.toStdString();
                bool isCorrect = checkWordWithHunspell(word);
                totalWords++;
                
                if (!isCorrect) {
                    errorCount++;
                    cursor.setPosition(wordStartPos);
                    cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor, currentWord.length());
                    cursor.setCharFormat(errorFormat);
                    cursor.clearSelection();
                }
                wordIndex++;
                currentWord.clear();
            }
        }
    }
    
    if (!currentWord.isEmpty()) {
        std::string word = currentWord.toStdString();
        bool isCorrect = checkWordWithHunspell(word);
        totalWords++;
        
        if (!isCorrect) {
            errorCount++;
            cursor.setPosition(wordStartPos);
            cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor, currentWord.length());
            cursor.setCharFormat(errorFormat);
            cursor.clearSelection();
        }
    }

    cursor.movePosition(QTextCursor::Start);
    textInput->setTextCursor(cursor);

    double errorPercentage = (totalWords > 0) ? (100.0 * errorCount / totalWords) : 0.0;
    
    QString output = QString(
        "Всего слов: %1\n"
        "Ошибок: %2\n"
        "Процент ошибок: %3%\n\n"
    ).arg(totalWords).arg(errorCount).arg(errorPercentage, 0, 'f', 1);

    if (errorCount > 0) {
        output += "Неправильные слова (проверено через Hunspell):\n";
        // Собираем неправильные слова заново
        QRegularExpression re("[^\\p{L}'\\-]+");
        QStringList words = qtext.split(re, Qt::SkipEmptyParts);
        for (const QString& word : words) {
            if (!checkWordWithHunspell(word.toStdString())) {
                output += QString(" - %1\n").arg(word);
            }
        }
    } else {
        output += "✓ Все слова написаны правильно!";
    }

    outputText->setText(output);
    statusLabel->setText(QString("✓ Проверено %1 слов, найдено %2 ошибок").arg(totalWords).arg(errorCount));
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