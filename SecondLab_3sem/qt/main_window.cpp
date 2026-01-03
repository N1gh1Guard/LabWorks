#include "main_window.h"
#include <QMessageBox>
#include <QString>
#include <QStringList>
#include <QFile>
#include <QTextStream>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , dbManager_(new DatabaseManager<std::string, std::string>(100))
    , indexManager_(new AlphabetIndexManager())
{
    setWindowTitle("Laba2 - Database & Alphabet Index GUI");
    setMinimumSize(800, 600);

    QTabWidget *tabs = new QTabWidget(this);
    setCentralWidget(tabs);

    // Database/Cache tab
    QWidget *dbTab = new QWidget();
    setupDatabaseTab();
    dbTab->setLayout(new QVBoxLayout());
    dbTab->layout()->addWidget(createDatabaseWidget());
    tabs->addTab(dbTab, "Database & Cache");

    // Alphabet Index tab
    QWidget *alphabetTab = new QWidget();
    setupAlphabetTab();
    alphabetTab->setLayout(new QVBoxLayout());
    alphabetTab->layout()->addWidget(createAlphabetWidget());
    tabs->addTab(alphabetTab, "Alphabet Index");
}

MainWindow::~MainWindow()
{
    delete dbManager_;
    delete indexManager_;
}

void MainWindow::setupDatabaseTab()
{
    // This will be called from constructor
}

void MainWindow::setupAlphabetTab()
{
    // This will be called from constructor
}

QWidget* MainWindow::createDatabaseWidget()
{
    QWidget *widget = new QWidget();
    QVBoxLayout *mainLayout = new QVBoxLayout(widget);

    // Input group
    QGroupBox *inputGroup = new QGroupBox("Input");
    QHBoxLayout *inputLayout = new QHBoxLayout();
    
    keyEdit_ = new QLineEdit();
    keyEdit_->setPlaceholderText("Key");
    valueEdit_ = new QLineEdit();
    valueEdit_->setPlaceholderText("Value");
    
    QPushButton *putBtn = new QPushButton("Put");
    QPushButton *getBtn = new QPushButton("Get");
    QPushButton *eraseBtn = new QPushButton("Erase");
    
    connect(putBtn, &QPushButton::clicked, this, &MainWindow::onPutClicked);
    connect(getBtn, &QPushButton::clicked, this, &MainWindow::onGetClicked);
    connect(eraseBtn, &QPushButton::clicked, this, &MainWindow::onEraseClicked);
    
    inputLayout->addWidget(new QLabel("Key:"));
    inputLayout->addWidget(keyEdit_);
    inputLayout->addWidget(new QLabel("Value:"));
    inputLayout->addWidget(valueEdit_);
    inputLayout->addWidget(putBtn);
    inputLayout->addWidget(getBtn);
    inputLayout->addWidget(eraseBtn);
    inputGroup->setLayout(inputLayout);

    // Stats group
    QGroupBox *statsGroup = new QGroupBox("Statistics");
    QHBoxLayout *statsLayout = new QHBoxLayout();
    dbSizeLabel_ = new QLabel("DB Size: 0");
    cacheSizeLabel_ = new QLabel("Cache Size: 0");
    statsLayout->addWidget(dbSizeLabel_);
    statsLayout->addWidget(cacheSizeLabel_);
    QPushButton *refreshBtn = new QPushButton("Refresh");
    connect(refreshBtn, &QPushButton::clicked, this, &MainWindow::onRefreshKeysClicked);
    statsLayout->addWidget(refreshBtn);
    statsLayout->addStretch();
    statsGroup->setLayout(statsLayout);

    // Keys list
    QGroupBox *keysGroup = new QGroupBox("Keys in Database");
    QVBoxLayout *keysLayout = new QVBoxLayout();
    keysList_ = new QListWidget();
    keysLayout->addWidget(keysList_);
    keysGroup->setLayout(keysLayout);

    // Result text
    QGroupBox *resultGroup = new QGroupBox("Result");
    QVBoxLayout *resultLayout = new QVBoxLayout();
    resultText_ = new QTextEdit();
    resultText_->setReadOnly(true);
    resultLayout->addWidget(resultText_);
    resultGroup->setLayout(resultLayout);

    // Control buttons
    QHBoxLayout *controlLayout = new QHBoxLayout();
    QPushButton *clearDBBtn = new QPushButton("Clear DB");
    QPushButton *clearCacheBtn = new QPushButton("Clear Cache");
    connect(clearDBBtn, &QPushButton::clicked, this, &MainWindow::onClearDBClicked);
    connect(clearCacheBtn, &QPushButton::clicked, this, &MainWindow::onClearCacheClicked);
    controlLayout->addWidget(clearDBBtn);
    controlLayout->addWidget(clearCacheBtn);
    controlLayout->addStretch();

    mainLayout->addWidget(inputGroup);
    mainLayout->addWidget(statsGroup);
    mainLayout->addWidget(keysGroup);
    mainLayout->addWidget(resultGroup);
    mainLayout->addLayout(controlLayout);

    updateStats();
    return widget;
}

QWidget* MainWindow::createAlphabetWidget()
{
    QWidget *widget = new QWidget();
    QVBoxLayout *mainLayout = new QVBoxLayout(widget);

    // Text input group
    QGroupBox *textGroup = new QGroupBox("Text Input");
    QVBoxLayout *textLayout = new QVBoxLayout();
    textInput_ = new QTextEdit();
    textInput_->setPlaceholderText("Enter text here or load from file...");
    textInput_->setMaximumHeight(150);
    textLayout->addWidget(textInput_);
    
    QPushButton *loadFileBtn = new QPushButton("Load File (.txt)");
    connect(loadFileBtn, &QPushButton::clicked, this, &MainWindow::onLoadFileClicked);
    textLayout->addWidget(loadFileBtn);
    
    textGroup->setLayout(textLayout);

    // Settings group
    QGroupBox *settingsGroup = new QGroupBox("Settings");
    QHBoxLayout *settingsLayout = new QHBoxLayout();
    settingsLayout->addWidget(new QLabel("Page Size:"));
    pageSizeSpin_ = new QSpinBox();
    pageSizeSpin_->setMinimum(1);
    pageSizeSpin_->setMaximum(1000);
    pageSizeSpin_->setValue(10);
    settingsLayout->addWidget(pageSizeSpin_);
    
    settingsLayout->addWidget(new QLabel("Type:"));
    pageTypeCombo_ = new QComboBox();
    pageTypeCombo_->addItem("By Words", static_cast<int>(PageSizeType::BY_WORDS));
    pageTypeCombo_->addItem("By Characters", static_cast<int>(PageSizeType::BY_CHARS));
    settingsLayout->addWidget(pageTypeCombo_);
    
    QPushButton *buildBtn = new QPushButton("Build Index");
    connect(buildBtn, &QPushButton::clicked, this, &MainWindow::onBuildIndexClicked);
    settingsLayout->addWidget(buildBtn);
    settingsLayout->addStretch();
    settingsGroup->setLayout(settingsLayout);

    // Search group
    QGroupBox *searchGroup = new QGroupBox("Search");
    QHBoxLayout *searchLayout = new QHBoxLayout();
    wordSearchEdit_ = new QLineEdit();
    wordSearchEdit_->setPlaceholderText("Enter word to search...");
    QPushButton *lookupBtn = new QPushButton("Lookup");
    connect(lookupBtn, &QPushButton::clicked, this, &MainWindow::onLookupWordClicked);
    searchLayout->addWidget(wordSearchEdit_);
    searchLayout->addWidget(lookupBtn);
    searchGroup->setLayout(searchLayout);

    // Action buttons
    QHBoxLayout *actionLayout = new QHBoxLayout();
    QPushButton *showPagesBtn = new QPushButton("Show Pages");
    QPushButton *showPointerBtn = new QPushButton("Show Pointer");
    connect(showPagesBtn, &QPushButton::clicked, this, &MainWindow::onShowPagesClicked);
    connect(showPointerBtn, &QPushButton::clicked, this, &MainWindow::onShowPointerClicked);
    actionLayout->addWidget(showPagesBtn);
    actionLayout->addWidget(showPointerBtn);
    actionLayout->addStretch();

    // Result text
    QGroupBox *resultGroup = new QGroupBox("Result");
    QVBoxLayout *resultLayout = new QVBoxLayout();
    indexResultText_ = new QTextEdit();
    indexResultText_->setReadOnly(true);
    resultLayout->addWidget(indexResultText_);
    resultGroup->setLayout(resultLayout);

    mainLayout->addWidget(textGroup);
    mainLayout->addWidget(settingsGroup);
    mainLayout->addWidget(searchGroup);
    mainLayout->addLayout(actionLayout);
    mainLayout->addWidget(resultGroup);

    return widget;
}

void MainWindow::onPutClicked()
{
    QString key = keyEdit_->text();
    QString value = valueEdit_->text();
    
    if (key.isEmpty()) {
        QMessageBox::warning(this, "Warning", "Key cannot be empty!");
        return;
    }
    
    dbManager_->put(key.toStdString(), value.toStdString());
    resultText_->append(QString("Put: %1 = %2").arg(key, value));
    updateStats();
    updateKeyList();
    keyEdit_->clear();
    valueEdit_->clear();
}

void MainWindow::onGetClicked()
{
    QString key = keyEdit_->text();
    
    if (key.isEmpty()) {
        QMessageBox::warning(this, "Warning", "Key cannot be empty!");
        return;
    }
    
    auto value = dbManager_->get(key.toStdString());
    if (value.has_value()) {
        resultText_->append(QString("Get: %1 = %2").arg(key, QString::fromStdString(*value)));
    } else {
        resultText_->append(QString("Get: %1 - not found").arg(key));
    }
    updateStats();
}

void MainWindow::onEraseClicked()
{
    QString key = keyEdit_->text();
    
    if (key.isEmpty()) {
        QMessageBox::warning(this, "Warning", "Key cannot be empty!");
        return;
    }
    
    bool result = dbManager_->erase(key.toStdString());
    if (result) {
        resultText_->append(QString("Erased: %1").arg(key));
    } else {
        resultText_->append(QString("Erase failed: %1 not found").arg(key));
    }
    updateStats();
    updateKeyList();
    keyEdit_->clear();
}

void MainWindow::onClearDBClicked()
{
    dbManager_->clear_db();
    resultText_->append("Database cleared");
    updateStats();
    updateKeyList();
}

void MainWindow::onClearCacheClicked()
{
    dbManager_->clear_cache();
    resultText_->append("Cache cleared");
    updateStats();
}

void MainWindow::onRefreshKeysClicked()
{
    updateKeyList();
    updateStats();
}

void MainWindow::onBuildIndexClicked()
{
    QString text = textInput_->toPlainText();
    if (text.isEmpty()) {
        QMessageBox::warning(this, "Warning", "Text cannot be empty!");
        return;
    }
    
    int pageSize = pageSizeSpin_->value();
    PageSizeType type = static_cast<PageSizeType>(pageTypeCombo_->currentData().toInt());
    
    indexManager_->build_index(text.toStdString(), pageSize, type);
    indexResultText_->setPlainText("Index built successfully!");
}

void MainWindow::onLookupWordClicked()
{
    if (!indexManager_->is_ready()) {
        QMessageBox::warning(this, "Warning", "Please build index first!");
        return;
    }
    
    QString word = wordSearchEdit_->text();
    if (word.isEmpty()) {
        QMessageBox::warning(this, "Warning", "Word cannot be empty!");
        return;
    }
    
    auto pages = indexManager_->lookup_word(word.toStdString());
    if (pages.empty()) {
        indexResultText_->setPlainText(QString("Word '%1' not found").arg(word));
    } else {
        QString result = QString("Word '%1' found on pages: ").arg(word);
        for (int p : pages) {
            result += QString::number(p) + " ";
        }
        indexResultText_->setPlainText(result);
    }
}

void MainWindow::onShowPagesClicked()
{
    if (!indexManager_->is_ready()) {
        QMessageBox::warning(this, "Warning", "Please build index first!");
        return;
    }
    
    QString pagesText = QString::fromStdString(indexManager_->get_pages_string());
    indexResultText_->setPlainText(pagesText);
}

void MainWindow::onShowPointerClicked()
{
    if (!indexManager_->is_ready()) {
        QMessageBox::warning(this, "Warning", "Please build index first!");
        return;
    }
    
    QString pointerText = QString::fromStdString(indexManager_->get_pointer_string());
    indexResultText_->setPlainText(pointerText);
}

void MainWindow::onLoadFileClicked()
{
    QString fileName = QFileDialog::getOpenFileName(
        this,
        "Open Text File",
        "",
        "Text Files (*.txt);;All Files (*)"
    );
    
    if (fileName.isEmpty()) {
        return; // User cancelled
    }
    
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Error", 
            QString("Cannot open file: %1\n%2").arg(fileName, file.errorString()));
        return;
    }
    
    QTextStream in(&file);
    QString content = in.readAll();
    file.close();
    
    if (content.isEmpty()) {
        QMessageBox::warning(this, "Warning", "File is empty!");
        return;
    }
    
    textInput_->setPlainText(content);
    indexResultText_->setPlainText(QString("File loaded successfully: %1\nCharacters: %2\nWords will be parsed when you click 'Build Index'")
        .arg(fileName).arg(content.length()));
}

void MainWindow::updateKeyList()
{
    keysList_->clear();
    auto keys = dbManager_->get_all_keys();
    for (const auto& key : keys) {
        keysList_->addItem(QString::fromStdString(key));
    }
}

void MainWindow::updateStats()
{
    dbSizeLabel_->setText(QString("DB Size: %1").arg(dbManager_->db_size()));
    cacheSizeLabel_->setText(QString("Cache Size: %1").arg(dbManager_->cache_size()));
}

