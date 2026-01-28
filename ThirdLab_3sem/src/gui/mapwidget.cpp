#include "mapwidget.hpp"
#include "../core/TerrainType.hpp"
#include <QPainter>
#include <QMouseEvent>
#include <QResizeEvent>
#include <QLinearGradient>
#include <QRadialGradient>
#include <QPainterPath>
#include <QPixmap>
#include <QImage>
#include <QFont>
#include <algorithm>
#include <cmath>
#include <limits>

MapWidget::MapWidget(QWidget* parent)
    : QWidget(parent),
      map_(nullptr),
      cellWidth_(20),
      cellHeight_(20),
      startRow_(-1),
      startCol_(-1),
      endRow_(-1),
      endCol_(-1),
      hasStartFlag_(false),
      hasEndFlag_(false),
      currentPathIndex_(-1),
      hasPaths_(false) {
    setMinimumSize(400, 300);
    setMouseTracking(false);
}

void MapWidget::setMap(Map* map) {
    map_ = map;
    updateCellSize();
    update();
}

void MapWidget::clearMap() {
    map_ = nullptr;
    clearFlags();
    clearPaths();
    update();
}

void MapWidget::setStartFlag(int row, int col) {
    if (map_ && map_->isValid(row, col)) {
        startRow_ = row;
        startCol_ = col;
        hasStartFlag_ = true;
        update();
    }
}

void MapWidget::setEndFlag(int row, int col) {
    if (map_ && map_->isValid(row, col)) {
        endRow_ = row;
        endCol_ = col;
        hasEndFlag_ = true;
        update();
    }
}

void MapWidget::clearFlags() {
    hasStartFlag_ = false;
    hasEndFlag_ = false;
    startRow_ = -1;
    startCol_ = -1;
    endRow_ = -1;
    endCol_ = -1;
    update();
}

void MapWidget::setPaths(const std::vector<std::vector<int>>& paths) {
    paths_ = paths;
    hasPaths_ = !paths.empty();
    if (hasPaths_ && currentPathIndex_ < 0) {
        currentPathIndex_ = 0;
    }
    update();
}

void MapWidget::setCurrentPath(int pathIndex) {
    if (pathIndex >= 0 && pathIndex < static_cast<int>(paths_.size())) {
        currentPathIndex_ = pathIndex;
        update();
    }
}

void MapWidget::clearPaths() {
    paths_.clear();
    hasPaths_ = false;
    currentPathIndex_ = -1;
    update();
}

std::pair<int, int> MapWidget::getCellAtPosition(const QPoint& pos) const {
    if (!map_) {
        return std::make_pair(-1, -1);
    }

    int col = pos.x() / cellWidth_;
    int row = pos.y() / cellHeight_;

    if (map_->isValid(row, col)) {
        return std::make_pair(row, col);
    }

    return std::make_pair(-1, -1);
}

void MapWidget::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    if (!map_) {
        painter.fillRect(rect(), QColor(240, 240, 240));
        painter.setPen(QPen(QColor(100, 100, 100), 1));
        painter.drawText(rect(), Qt::AlignCenter, "Карта не загружена");
        return;
    }

    for (int row = 0; row < map_->getRows(); ++row) {
        for (int col = 0; col < map_->getCols(); ++col) {
            const Cell& cell = map_->getCell(row, col);
            QColor base = getTerrainColor(cell.getTerrainType());

            int x = col * cellWidth_;
            int y = row * cellHeight_;
            QRect cellRect(x, y, cellWidth_, cellHeight_);

            // Базовый градиент
            QLinearGradient grad(x, y, x + cellWidth_, y + cellHeight_);
            grad.setColorAt(0, base.lighter(115));
            grad.setColorAt(0.5, base);
            grad.setColorAt(1, base.darker(115));
            painter.fillRect(cellRect, grad);

            // Добавляем текстуру
            drawCellTexture(painter, cellRect, cell.getTerrainType(), base);

            // Отображаем скорость (стоимость прохождения)
            drawCellSpeed(painter, cellRect, cell.getTerrainType());

            // Рамка
            painter.setPen(QPen(base.lighter(130), 1));
            painter.drawLine(x, y, x + cellWidth_, y);
            painter.drawLine(x, y, x, y + cellHeight_);
            painter.setPen(QPen(base.darker(130), 1));
            painter.drawLine(x + cellWidth_, y, x + cellWidth_, y + cellHeight_);
            painter.drawLine(x, y + cellHeight_, x + cellWidth_, y + cellHeight_);
        }
    }

    if (hasPaths_ && currentPathIndex_ >= 0 && 
        currentPathIndex_ < static_cast<int>(paths_.size())) {
        const auto& path = paths_[currentPathIndex_];
        
        if (!path.empty()) {
            QPen pathPen(QColor(255, 0, 0), 3);
            painter.setPen(pathPen);
            
            for (size_t i = 0; i < path.size() - 1; ++i) {
                int vertex1 = path[i];
                int vertex2 = path[i + 1];
                
                std::pair<int, int> cell1 = map_->vertexIndexToCell(vertex1);
                std::pair<int, int> cell2 = map_->vertexIndexToCell(vertex2);
                
                int x1 = cell1.second * cellWidth_ + cellWidth_ / 2;
                int y1 = cell1.first * cellHeight_ + cellHeight_ / 2;
                int x2 = cell2.second * cellWidth_ + cellWidth_ / 2;
                int y2 = cell2.first * cellHeight_ + cellHeight_ / 2;
                
                painter.drawLine(x1, y1, x2, y2);
            }
            
            painter.setBrush(QBrush(QColor(255, 0, 0)));
            painter.setPen(QPen(QColor(255, 255, 255), 2));
            for (int vertex : path) {
                std::pair<int, int> cell = map_->vertexIndexToCell(vertex);
                int x = cell.second * cellWidth_ + cellWidth_ / 2;
                int y = cell.first * cellHeight_ + cellHeight_ / 2;
                painter.drawEllipse(x - 4, y - 4, 8, 8);
            }
        }
    }

    if (hasStartFlag_ && map_->isValid(startRow_, startCol_)) {
        int x = startCol_ * cellWidth_ + cellWidth_ / 2;
        int y = startRow_ * cellHeight_ + cellHeight_ / 2;
        drawFlag(painter, x, y, cellWidth_, cellHeight_, true);
    }

    if (hasEndFlag_ && map_->isValid(endRow_, endCol_)) {
        int x = endCol_ * cellWidth_ + cellWidth_ / 2;
        int y = endRow_ * cellHeight_ + cellHeight_ / 2;
        drawFlag(painter, x, y, cellWidth_, cellHeight_, false);
    }
}

void MapWidget::mousePressEvent(QMouseEvent* event) {
    if (!map_ || event->button() != Qt::LeftButton) {
        return;
    }

    std::pair<int, int> cell = getCellAtPosition(event->pos());
    if (cell.first >= 0 && cell.second >= 0) {
        emit cellClicked(cell.first, cell.second);
    }
}

void MapWidget::resizeEvent(QResizeEvent* event) {
    QWidget::resizeEvent(event);
    updateCellSize();
    update();
}

QColor MapWidget::getTerrainColor(TerrainType terrain) const {
    switch (terrain) {
        case TerrainType::Grass:
            return QColor(72, 160, 72);
        case TerrainType::Sand:
            return QColor(218, 185, 146);
        case TerrainType::Water:
            return QColor(99, 179, 219);
        case TerrainType::DeepWater:
            return QColor(30, 60, 120);
        case TerrainType::Forest:
            return QColor(45, 95, 45);
        default:
            return QColor(180, 180, 180);
    }
}

void MapWidget::drawCellTexture(QPainter& painter, const QRect& rect, TerrainType terrain, const QColor& baseColor) {
    painter.save();
    
    QColor textureColor = baseColor.darker(120);
    textureColor.setAlpha(80);
    painter.setPen(QPen(textureColor, 1));
    
    int size = std::min(rect.width(), rect.height());
    
    switch (terrain) {
        case TerrainType::Grass: {
            // Точки для травы
            int dotSize = std::max(1, size / 8);
            int spacing = size / 4;
            for (int y = rect.top() + spacing/2; y < rect.bottom(); y += spacing) {
                for (int x = rect.left() + spacing/2; x < rect.right(); x += spacing) {
                    painter.setBrush(QBrush(textureColor));
                    painter.drawEllipse(x - dotSize/2, y - dotSize/2, dotSize, dotSize);
                }
            }
            break;
        }
        case TerrainType::Sand: {
            // Волнистые линии для песка
            int lineSpacing = size / 6;
            for (int y = rect.top(); y < rect.bottom(); y += lineSpacing) {
                QPainterPath path;
                path.moveTo(rect.left(), y);
                for (int x = rect.left(); x < rect.right(); x += lineSpacing/2) {
                    double wave = std::sin((x - rect.left()) * 0.3) * 2;
                    path.lineTo(x, y + wave);
                }
                painter.drawPath(path);
            }
            break;
        }
        case TerrainType::Water:
        case TerrainType::DeepWater: {
            // Горизонтальные линии для воды
            int lineSpacing = size / 5;
            for (int y = rect.top(); y < rect.bottom(); y += lineSpacing) {
                painter.drawLine(rect.left(), y, rect.right(), y);
            }
            // Добавляем небольшие круги для волн
            if (size > 15) {
                int waveSize = std::max(2, size / 10);
                for (int i = 0; i < 3; ++i) {
                    int x = rect.left() + (i + 1) * rect.width() / 4;
                    int y = rect.top() + (i % 2 + 1) * rect.height() / 3;
                    painter.setBrush(Qt::NoBrush);
                    painter.drawEllipse(x - waveSize, y - waveSize, waveSize * 2, waveSize * 2);
                }
            }
            break;
        }
        case TerrainType::Forest: {
            // Штриховка для леса
            int lineSpacing = size / 4;
            for (int i = 0; i < 3; ++i) {
                int offset = i * lineSpacing / 3;
                for (int y = rect.top() + offset; y < rect.bottom(); y += lineSpacing) {
                    painter.drawLine(rect.left(), y, rect.right(), y);
                }
            }
            // Добавляем вертикальные линии
            for (int x = rect.left() + lineSpacing/2; x < rect.right(); x += lineSpacing) {
                painter.drawLine(x, rect.top(), x, rect.bottom());
            }
            break;
        }
        default:
            break;
    }
    
    painter.restore();
}

void MapWidget::drawCellSpeed(QPainter& painter, const QRect& rect, TerrainType terrain) {
    double speed = TerrainTypeHelper::getDefaultTraversalTime(terrain);
    
    // Показываем скорость только если клетка достаточно большая
    int minSize = std::min(rect.width(), rect.height());
    if (minSize < 20) {
        return; // Слишком маленькая клетка
    }
    
    painter.save();
    
    QString speedText;
    if (speed == std::numeric_limits<double>::infinity()) {
        speedText = "∞";
    } else {
        speedText = QString::number(speed, 'f', 1);
    }
    
    // Выбираем цвет текста в зависимости от фона
    QColor textColor = QColor(255, 255, 255);
    if (terrain == TerrainType::Grass || terrain == TerrainType::Sand) {
        textColor = QColor(0, 0, 0);
    }
    
    // Рисуем полупрозрачный фон для текста для лучшей читаемости
    QColor bgColor = QColor(0, 0, 0);
    bgColor.setAlpha(120);
    int padding = 2;
    QRect textBg(rect.center().x() - minSize/4 - padding, 
                 rect.center().y() - minSize/8 - padding, 
                 minSize/2 + padding*2, 
                 minSize/4 + padding*2);
    painter.fillRect(textBg, bgColor);
    
    // Рисуем текст
    painter.setPen(QPen(textColor, 1));
    QFont font = painter.font();
    font.setBold(true);
    int fontSize = std::max(7, minSize / 5);
    font.setPixelSize(fontSize);
    painter.setFont(font);
    painter.drawText(rect, Qt::AlignCenter, speedText);
    
    painter.restore();
}

void MapWidget::drawFlag(QPainter& painter, int centerX, int centerY, int cellWidth, int cellHeight, bool isStart) {
    painter.save();
    
    int flagSize = std::min(cellWidth, cellHeight) / 2;
    int poleHeight = cellHeight / 2;
    int flagWidth = flagSize;
    int flagHeight = flagSize * 2 / 3;
    
    // Рисуем флагшток
    painter.setPen(QPen(QColor(101, 67, 33), 3));
    painter.drawLine(centerX, centerY - poleHeight, centerX, centerY + poleHeight / 2);
    
    // Рисуем флаг
    QRect flagRect(centerX + 2, centerY - poleHeight, flagWidth, flagHeight);
    
    if (isStart) {
        // Зеленый флаг старта
        QLinearGradient flagGrad(flagRect.topLeft(), flagRect.bottomRight());
        flagGrad.setColorAt(0, QColor(50, 200, 50));
        flagGrad.setColorAt(1, QColor(30, 150, 30));
        painter.setBrush(QBrush(flagGrad));
        painter.setPen(QPen(QColor(20, 120, 20), 2));
        painter.drawRect(flagRect);
        
        // Белая галочка
        painter.setPen(QPen(QColor(255, 255, 255), 3));
        painter.setBrush(Qt::NoBrush);
        int checkSize = flagHeight / 3;
        QPointF checkStart(centerX + flagWidth / 4.0, centerY - poleHeight + flagHeight / 2.0);
        painter.drawLine(checkStart, QPointF(checkStart.x() - checkSize/2.0, checkStart.y()));
        painter.drawLine(checkStart, QPointF(checkStart.x(), checkStart.y() + checkSize));
    } else {
        // Красный флаг финиша
        QLinearGradient flagGrad(flagRect.topLeft(), flagRect.bottomRight());
        flagGrad.setColorAt(0, QColor(255, 50, 50));
        flagGrad.setColorAt(1, QColor(200, 20, 20));
        painter.setBrush(QBrush(flagGrad));
        painter.setPen(QPen(QColor(150, 10, 10), 2));
        painter.drawRect(flagRect);
        
        // Белый крестик
        painter.setPen(QPen(QColor(255, 255, 255), 3));
        painter.setBrush(Qt::NoBrush);
        int crossSize = flagHeight / 3;
        QPointF crossCenter(centerX + flagWidth / 2.0, centerY - poleHeight + flagHeight / 2.0);
        painter.drawLine(crossCenter.x() - crossSize/2.0, crossCenter.y(),
                        crossCenter.x() + crossSize/2.0, crossCenter.y());
        painter.drawLine(crossCenter.x(), crossCenter.y() - crossSize/2.0,
                        crossCenter.x(), crossCenter.y() + crossSize/2.0);
    }
    
    painter.restore();
}

void MapWidget::updateCellSize() {
    if (!map_) {
        cellWidth_ = 20;
        cellHeight_ = 20;
        return;
    }

    int availableWidth = width();
    int availableHeight = height();

    if (map_->getCols() > 0 && map_->getRows() > 0) {
        cellWidth_ = std::max(10, availableWidth / map_->getCols());
        cellHeight_ = std::max(10, availableHeight / map_->getRows());
        
        int minSize = std::min(cellWidth_, cellHeight_);
        cellWidth_ = minSize;
        cellHeight_ = minSize;
    } else {
        cellWidth_ = 20;
        cellHeight_ = 20;
    }
}
