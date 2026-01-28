#pragma once

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QSpinBox>
#include <QComboBox>
#include <QFileDialog>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QTime>
#include <memory>
#include "mapwidget.hpp"
#include "../core/Map.hpp"
#include "../io/MapIO.hpp"
#include "../generation/MapGenerator.hpp"
#include "../algorithms/Pathfinder.hpp"
#include "../core/Graph.hpp"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() = default;

private slots:
    void onGenerateRandomMap();
    void onGenerateProceduralMap();
    void onLoadMap();
    void onSaveMap();
    void onFindPath();
    void onNextPath();
    void onPreviousPath();
    void onCellClicked(int row, int col);
    void onClearFlags();
    void onClearPaths();

private:
    void setupUI();
    void createMenuBar();
    void updatePathNavigationButtons();
    void updatePathInfo();
    void setStartFlag(int row, int col);
    void setEndFlag(int row, int col);
    void clearFlags();
    void clearPaths();

    MapWidget* mapWidget_;
    std::unique_ptr<Map> currentMap_;
    std::unique_ptr<Graph> currentGraph_;

    QPushButton* generateRandomButton_;
    QPushButton* generateProceduralButton_;
    QPushButton* loadButton_;
    QPushButton* saveButton_;
    QPushButton* findPathButton_;
    QPushButton* nextPathButton_;
    QPushButton* previousPathButton_;
    QPushButton* clearFlagsButton_;
    QPushButton* clearPathsButton_;

    QSpinBox* rowsSpinBox_;
    QSpinBox* colsSpinBox_;
    QSpinBox* seedSpinBox_;

    QLabel* pathInfoLabel_;
    QLabel* timeLabel_;
    QLabel* statusLabel_;

    std::vector<PathResult> foundPaths_;
    int currentPathIndex_;

    int startRow_;
    int startCol_;
    int endRow_;
    int endCol_;
    bool hasStartFlag_;
    bool hasEndFlag_;
    bool placingStartFlag_;
};
