#include "mainwindow.hpp"
#include "../core/Exceptions.hpp"
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QGroupBox>
#include <QFormLayout>
#include <QApplication>
#include <chrono>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
      mapWidget_(nullptr),
      currentPathIndex_(-1),
      startRow_(-1),
      startCol_(-1),
      endRow_(-1),
      endCol_(-1),
      hasStartFlag_(false),
      hasEndFlag_(false),
      placingStartFlag_(true) {
    setWindowTitle("Pathfinding Map Application - Поиск кратчайших путей");
    setGeometry(100, 100, 1200, 800);
    setupUI();
    createMenuBar();
    updatePathNavigationButtons();
}

void MainWindow::setupUI() {
    QWidget* centralWidget = new QWidget(this);
    QHBoxLayout* mainLayout = new QHBoxLayout(centralWidget);

    QWidget* controlPanel = new QWidget();
    QVBoxLayout* controlLayout = new QVBoxLayout(controlPanel);
    controlPanel->setMaximumWidth(300);
    controlPanel->setMinimumWidth(250);

    QGroupBox* generationGroup = new QGroupBox("Генерация карт");
    QVBoxLayout* generationLayout = new QVBoxLayout();

    QFormLayout* sizeLayout = new QFormLayout();
    rowsSpinBox_ = new QSpinBox();
    rowsSpinBox_->setRange(5, 100);
    rowsSpinBox_->setValue(20);
    colsSpinBox_ = new QSpinBox();
    colsSpinBox_->setRange(5, 100);
    colsSpinBox_->setValue(20);
    seedSpinBox_ = new QSpinBox();
    seedSpinBox_->setRange(0, 999999);
    seedSpinBox_->setValue(0);
    seedSpinBox_->setSpecialValueText("Случайный");

    sizeLayout->addRow("Строки:", rowsSpinBox_);
    sizeLayout->addRow("Столбцы:", colsSpinBox_);
    sizeLayout->addRow("Seed:", seedSpinBox_);

    generateRandomButton_ = new QPushButton("Случайная карта");
    generateProceduralButton_ = new QPushButton("Процедурная карта");

    generationLayout->addLayout(sizeLayout);
    generationLayout->addWidget(generateRandomButton_);
    generationLayout->addWidget(generateProceduralButton_);
    generationGroup->setLayout(generationLayout);

    QGroupBox* fileGroup = new QGroupBox("Файлы");
    QVBoxLayout* fileLayout = new QVBoxLayout();
    loadButton_ = new QPushButton("Загрузить карту");
    saveButton_ = new QPushButton("Сохранить карту");
    fileLayout->addWidget(loadButton_);
    fileLayout->addWidget(saveButton_);
    fileGroup->setLayout(fileLayout);

    QGroupBox* pathfindingGroup = new QGroupBox("Поиск пути");
    QVBoxLayout* pathfindingLayout = new QVBoxLayout();
    findPathButton_ = new QPushButton("Найти путь");
    findPathButton_->setEnabled(false);
    clearFlagsButton_ = new QPushButton("Очистить флаги");
    clearPathsButton_ = new QPushButton("Очистить пути");
    pathfindingLayout->addWidget(findPathButton_);
    pathfindingLayout->addWidget(clearFlagsButton_);
    pathfindingLayout->addWidget(clearPathsButton_);
    pathfindingGroup->setLayout(pathfindingLayout);

    QGroupBox* navigationGroup = new QGroupBox("Навигация по путям");
    QHBoxLayout* navigationLayout = new QHBoxLayout();
    previousPathButton_ = new QPushButton("◀ Предыдущий");
    nextPathButton_ = new QPushButton("Следующий ▶");
    previousPathButton_->setEnabled(false);
    nextPathButton_->setEnabled(false);
    navigationLayout->addWidget(previousPathButton_);
    navigationLayout->addWidget(nextPathButton_);
    navigationGroup->setLayout(navigationLayout);

    pathInfoLabel_ = new QLabel("Пути не найдены");
    pathInfoLabel_->setWordWrap(true);
    timeLabel_ = new QLabel("Время: -");
    timeLabel_->setWordWrap(true);

    statusLabel_ = new QLabel("Готово");
    statusLabel_->setWordWrap(true);

    controlLayout->addWidget(generationGroup);
    controlLayout->addWidget(fileGroup);
    controlLayout->addWidget(pathfindingGroup);
    controlLayout->addWidget(navigationGroup);
    controlLayout->addWidget(pathInfoLabel_);
    controlLayout->addWidget(timeLabel_);
    controlLayout->addWidget(statusLabel_);
    controlLayout->addStretch();

    mapWidget_ = new MapWidget();
    mapWidget_->setMinimumSize(600, 500);

    connect(mapWidget_, &MapWidget::cellClicked, this, &MainWindow::onCellClicked);
    connect(generateRandomButton_, &QPushButton::clicked, this, &MainWindow::onGenerateRandomMap);
    connect(generateProceduralButton_, &QPushButton::clicked, this, &MainWindow::onGenerateProceduralMap);
    connect(loadButton_, &QPushButton::clicked, this, &MainWindow::onLoadMap);
    connect(saveButton_, &QPushButton::clicked, this, &MainWindow::onSaveMap);
    connect(findPathButton_, &QPushButton::clicked, this, &MainWindow::onFindPath);
    connect(nextPathButton_, &QPushButton::clicked, this, &MainWindow::onNextPath);
    connect(previousPathButton_, &QPushButton::clicked, this, &MainWindow::onPreviousPath);
    connect(clearFlagsButton_, &QPushButton::clicked, this, &MainWindow::onClearFlags);
    connect(clearPathsButton_, &QPushButton::clicked, this, &MainWindow::onClearPaths);

    mainLayout->addWidget(controlPanel);
    mainLayout->addWidget(mapWidget_, 1);

    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);
}

void MainWindow::createMenuBar() {
    QMenuBar* menuBar = new QMenuBar(this);
    setMenuBar(menuBar);

    QMenu* fileMenu = menuBar->addMenu("&Файл");
    QAction* loadAction = fileMenu->addAction("&Загрузить карту");
    QAction* saveAction = fileMenu->addAction("&Сохранить карту");
    fileMenu->addSeparator();
    QAction* exitAction = fileMenu->addAction("&Выход");
    
    connect(loadAction, &QAction::triggered, this, &MainWindow::onLoadMap);
    connect(saveAction, &QAction::triggered, this, &MainWindow::onSaveMap);
    connect(exitAction, &QAction::triggered, this, &QWidget::close);

    QMenu* helpMenu = menuBar->addMenu("&Справка");
    QAction* aboutAction = helpMenu->addAction("&О программе");
    connect(aboutAction, &QAction::triggered, [this]() {
        QMessageBox::about(this, "О программе",
            "<h2>Pathfinding Map Application</h2>"
            "<p>Приложение для поиска кратчайших путей на карте с различными типами местности</p>"
            "<p><b>Возможности:</b></p>"
            "<ul>"
            "<li>Генерация случайных и процедурных карт</li>"
            "<li>Сохранение и загрузка карт</li>"
            "<li>Поиск кратчайших путей алгоритмом Dijkstra</li>"
            "<li>Поиск всех кратчайших путей</li>"
            "<li>Визуализация путей на карте</li>"
            "</ul>"
            "<p><b>Язык:</b> C++17</p>"
            "<p><b>Фреймворк:</b> Qt5</p>");
    });
}

void MainWindow::onGenerateRandomMap() {
    int rows = rowsSpinBox_->value();
    int cols = colsSpinBox_->value();
    unsigned int seed = seedSpinBox_->value();

    RandomGenerationParams params(rows, cols);
    params.seed = seed;

    try {
        currentMap_ = MapGenerator::generateRandom(params);
        currentGraph_ = currentMap_->generateGraph(false);
        mapWidget_->setMap(currentMap_.get());
        clearFlags();
        clearPaths();
        statusLabel_->setText(QString("Сгенерирована случайная карта %1x%2").arg(rows).arg(cols));
        findPathButton_->setEnabled(true);
    } catch (const BaseException& e) {
        ExceptionTracker::getInstance().registerException(e);
        QMessageBox::critical(this, "Ошибка", QString("Не удалось сгенерировать карту: %1").arg(e.what()));
        statusLabel_->setText("Ошибка генерации карты");
    } catch (const std::exception& e) {
        BaseException baseEx(e.what());
        ExceptionTracker::getInstance().registerException(baseEx);
        QMessageBox::critical(this, "Ошибка", QString("Не удалось сгенерировать карту: %1").arg(e.what()));
        statusLabel_->setText("Ошибка генерации карты");
    }
}

void MainWindow::onGenerateProceduralMap() {
    int rows = rowsSpinBox_->value();
    int cols = colsSpinBox_->value();
    unsigned int seed = seedSpinBox_->value();

    ProceduralGenerationParams params(rows, cols);
    params.seed = seed;

    try {
        currentMap_ = MapGenerator::generateProcedural(params);
        currentGraph_ = currentMap_->generateGraph(false);
        mapWidget_->setMap(currentMap_.get());
        clearFlags();
        clearPaths();
        statusLabel_->setText(QString("Сгенерирована процедурная карта %1x%2").arg(rows).arg(cols));
        findPathButton_->setEnabled(true);
    } catch (const BaseException& e) {
        ExceptionTracker::getInstance().registerException(e);
        QMessageBox::critical(this, "Ошибка", QString("Не удалось сгенерировать карту: %1").arg(e.what()));
        statusLabel_->setText("Ошибка генерации карты");
    } catch (const std::exception& e) {
        BaseException baseEx(e.what());
        ExceptionTracker::getInstance().registerException(baseEx);
        QMessageBox::critical(this, "Ошибка", QString("Не удалось сгенерировать карту: %1").arg(e.what()));
        statusLabel_->setText("Ошибка генерации карты");
    }
}

void MainWindow::onLoadMap() {
    QString filename = QFileDialog::getOpenFileName(
        this,
        "Загрузить карту",
        "",
        "Текстовые файлы (*.txt);;JSON файлы (*.json);;Все файлы (*.*)"
    );

    if (filename.isEmpty()) {
        return;
    }

    std::unique_ptr<Map> loadedMap;
    if (filename.endsWith(".txt", Qt::CaseInsensitive)) {
        loadedMap = MapIO::loadMapText(filename.toStdString());
    } else {
        loadedMap = MapIO::loadMap(filename.toStdString());
    }

    if (loadedMap) {
        currentMap_ = std::move(loadedMap);
        currentGraph_ = currentMap_->generateGraph(false);
        mapWidget_->setMap(currentMap_.get());
        clearFlags();
        clearPaths();
        statusLabel_->setText(QString("Карта загружена: %1").arg(filename));
        findPathButton_->setEnabled(true);
    } else {
        QMessageBox::warning(this, "Ошибка", "Не удалось загрузить карту из файла");
        statusLabel_->setText("Ошибка загрузки карты");
    }
}

void MainWindow::onSaveMap() {
    if (!currentMap_) {
        QMessageBox::warning(this, "Предупреждение", "Нет карты для сохранения");
        return;
    }

    QString filename = QFileDialog::getSaveFileName(
        this,
        "Сохранить карту",
        "",
        "Текстовые файлы (*.txt);;JSON файлы (*.json);;Все файлы (*.*)"
    );

    if (filename.isEmpty()) {
        return;
    }

    bool success = false;
    if (filename.endsWith(".txt", Qt::CaseInsensitive)) {
        success = MapIO::saveMapText(*currentMap_, filename.toStdString());
    } else {
        if (!filename.endsWith(".json", Qt::CaseInsensitive)) {
            filename += ".json";
        }
        success = MapIO::saveMap(*currentMap_, filename.toStdString());
    }

    if (success) {
        statusLabel_->setText(QString("Карта сохранена: %1").arg(filename));
    } else {
        QMessageBox::warning(this, "Ошибка", "Не удалось сохранить карту");
        statusLabel_->setText("Ошибка сохранения карты");
    }
}

void MainWindow::onFindPath() {
    if (!currentMap_ || !currentGraph_ || !hasStartFlag_ || !hasEndFlag_) {
        QMessageBox::warning(this, "Предупреждение", 
            "Установите флаги старта и финиша на карте");
        return;
    }

    int startVertex = currentMap_->cellToVertexIndex(startRow_, startCol_);
    int endVertex = currentMap_->cellToVertexIndex(endRow_, endCol_);

    auto startTime = std::chrono::high_resolution_clock::now();
    foundPaths_ = Pathfinder::findAllShortestPaths(*currentGraph_, startVertex, endVertex);
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);

    if (foundPaths_.empty() || !foundPaths_[0].found) {
        QMessageBox::information(this, "Результат", "Путь не найден");
        statusLabel_->setText("Путь не найден");
        foundPaths_.clear();
        mapWidget_->clearPaths();
        updatePathNavigationButtons();
        return;
    }

    std::vector<std::vector<int>> pathsForWidget;
    for (const auto& pathResult : foundPaths_) {
        pathsForWidget.push_back(pathResult.path);
    }

    mapWidget_->setPaths(pathsForWidget);
    currentPathIndex_ = 0;
    mapWidget_->setCurrentPath(0);

    statusLabel_->setText(QString("Найдено путей: %1, время: %2 мкс")
                         .arg(foundPaths_.size())
                         .arg(duration.count()));
    timeLabel_->setText(QString("Время выполнения: %1 мкс (%2 мс)")
                       .arg(duration.count())
                       .arg(duration.count() / 1000.0));

    updatePathInfo();
    updatePathNavigationButtons();
}

void MainWindow::onNextPath() {
    if (foundPaths_.empty() || currentPathIndex_ < 0) {
        return;
    }

    if (currentPathIndex_ < static_cast<int>(foundPaths_.size()) - 1) {
        currentPathIndex_++;
        mapWidget_->setCurrentPath(currentPathIndex_);
        updatePathInfo();
        updatePathNavigationButtons();
    }
}

void MainWindow::onPreviousPath() {
    if (foundPaths_.empty() || currentPathIndex_ < 0) {
        return;
    }

    if (currentPathIndex_ > 0) {
        currentPathIndex_--;
        mapWidget_->setCurrentPath(currentPathIndex_);
        updatePathInfo();
        updatePathNavigationButtons();
    }
}

void MainWindow::onCellClicked(int row, int col) {
    if (!currentMap_ || !currentMap_->isValid(row, col)) {
        return;
    }

    if (placingStartFlag_) {
        setStartFlag(row, col);
        placingStartFlag_ = false;
        statusLabel_->setText(QString("Флаг старта установлен на (%1, %2). Кликните для установки флага финиша")
                             .arg(row).arg(col));
    } else {
        setEndFlag(row, col);
        placingStartFlag_ = true;
        statusLabel_->setText(QString("Флаг финиша установлен на (%1, %2). Кликните для установки флага старта")
                             .arg(row).arg(col));
    }
}

void MainWindow::onClearFlags() {
    clearFlags();
}

void MainWindow::onClearPaths() {
    clearPaths();
}

void MainWindow::updatePathNavigationButtons() {
    bool hasPaths = !foundPaths_.empty();
    bool canGoNext = hasPaths && currentPathIndex_ >= 0 && 
                     currentPathIndex_ < static_cast<int>(foundPaths_.size()) - 1;
    bool canGoPrev = hasPaths && currentPathIndex_ > 0;

    nextPathButton_->setEnabled(canGoNext);
    previousPathButton_->setEnabled(canGoPrev);
}

void MainWindow::updatePathInfo() {
    if (foundPaths_.empty() || currentPathIndex_ < 0) {
        pathInfoLabel_->setText("Пути не найдены");
        return;
    }

    const auto& currentPath = foundPaths_[currentPathIndex_];
    pathInfoLabel_->setText(
        QString("Путь %1 из %2\nДлина: %3 клеток\nСтоимость: %4")
        .arg(currentPathIndex_ + 1)
        .arg(foundPaths_.size())
        .arg(currentPath.path.size())
        .arg(currentPath.totalCost, 0, 'f', 2)
    );
}

void MainWindow::setStartFlag(int row, int col) {
    startRow_ = row;
    startCol_ = col;
    hasStartFlag_ = true;
    mapWidget_->setStartFlag(row, col);
}

void MainWindow::setEndFlag(int row, int col) {
    endRow_ = row;
    endCol_ = col;
    hasEndFlag_ = true;
    mapWidget_->setEndFlag(row, col);
}

void MainWindow::clearFlags() {
    hasStartFlag_ = false;
    hasEndFlag_ = false;
    startRow_ = -1;
    startCol_ = -1;
    endRow_ = -1;
    endCol_ = -1;
    placingStartFlag_ = true;
    mapWidget_->clearFlags();
}

void MainWindow::clearPaths() {
    foundPaths_.clear();
    currentPathIndex_ = -1;
    mapWidget_->clearPaths();
    pathInfoLabel_->setText("Пути не найдены");
    timeLabel_->setText("Время: -");
    updatePathNavigationButtons();
}
