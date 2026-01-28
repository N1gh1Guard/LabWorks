#pragma once

#include <QWidget>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPoint>
#include <QRect>
#include <QColor>
#include <vector>
#include <memory>
#include "../core/Map.hpp"
#include "../core/TerrainType.hpp"
#include "../algorithms/Pathfinder.hpp"

class QPainter;

class MapWidget : public QWidget {
    Q_OBJECT

public:
    explicit MapWidget(QWidget* parent = nullptr);
    ~MapWidget() = default;

    void setMap(Map* map);
    void clearMap();
    void setStartFlag(int row, int col);
    void setEndFlag(int row, int col);
    void clearFlags();
    void setPaths(const std::vector<std::vector<int>>& paths);
    void setCurrentPath(int pathIndex);
    void clearPaths();
    std::pair<int, int> getCellAtPosition(const QPoint& pos) const;

signals:
    void cellClicked(int row, int col);

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;

private:
    QColor getTerrainColor(TerrainType terrain) const;
    void updateCellSize();
    void drawCellTexture(QPainter& painter, const QRect& rect, TerrainType terrain, const QColor& baseColor);
    void drawCellSpeed(QPainter& painter, const QRect& rect, TerrainType terrain);
    void drawFlag(QPainter& painter, int centerX, int centerY, int cellWidth, int cellHeight, bool isStart);

    Map* map_;
    int cellWidth_;
    int cellHeight_;
    
    int startRow_;
    int startCol_;
    int endRow_;
    int endCol_;
    bool hasStartFlag_;
    bool hasEndFlag_;
    
    std::vector<std::vector<int>> paths_;
    int currentPathIndex_;
    bool hasPaths_;
};
