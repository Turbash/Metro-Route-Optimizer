#include "MetroMapView.h"
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QGraphicsTextItem>
#include <QPen>
#include <QBrush>
#include <QColor>
#include <QResizeEvent>

MetroMapView::MetroMapView(QWidget *parent) : QGraphicsView(parent)
{
    setScene(new QGraphicsScene(this));
    setRenderHint(QPainter::Antialiasing);
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setMinimumSize(800, 600);
}

void MetroMapView::drawStation(const QString &name, double x, double y, const QString &line)
{
    QColor color;
    if (line.contains("Blue"))
        color = QColor(0x4169E1);
    else if (line.contains("Yellow"))
        color = QColor(0xFFDF00);
    else if (line.contains("Red"))
        color = QColor(0xFF4040);
    else if (line.contains("Pink"))
        color = QColor(0xFC8EAC);
    else if (line.contains("Magenta"))
        color = QColor(0xCC338B);
    else if (line.contains("Violet"))
        color = QColor(0x8b5cf6);

    scene()->addEllipse(x - 5, y - 5, 10, 10,
                        QPen(color, 2), QBrush(Qt::white));

    auto *text = scene()->addText(name);
    text->setDefaultTextColor(QColor(0xffffff));
    text->setPos(x - text->boundingRect().width() / 2, y + 10);
}

void MetroMapView::drawLine(double x1, double y1, double x2, double y2, const QString &line)
{
    QColor color;
    if (line == "Blue")
        color = QColor(0x4169E1);
    else if (line == "Yellow")
        color = QColor(0xFFDF00);
    else if (line == "Red")
        color = QColor(0xFF4040);
    else if (line == "Pink")
        color = QColor(0xFC8EAC);
    else if (line == "Magenta")
        color = QColor(0xCC338B);
    else if (line == "Violet")
        color = QColor(0x8b5cf6);

    scene()->addLine(x1, y1, x2, y2, QPen(color, 3));
}

void MetroMapView::highlightPath(const std::vector<int> &path, const std::vector<Station> &stations)
{
    /* Draw glow effect under the path lines first */
    for (size_t i = 0; i < path.size() - 1; i++)
    {
        double x1 = stations[path[i]].x;
        double y1 = stations[path[i]].y;
        double x2 = stations[path[i + 1]].x;
        double y2 = stations[path[i + 1]].y;

        /* Create a glow effect with gradually fading opacity */
        for (int glow = 14; glow > 4; glow -= 2)
        {
            QColor glowColor = QColor(0x00FF66);
            glowColor.setAlpha(50);
            scene()->addLine(x1, y1, x2, y2,
                             QPen(glowColor, glow, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        }

        /* Draw the main path line - brighter and more vibrant */
        scene()->addLine(x1, y1, x2, y2,
                         QPen(QColor(0x00FF99), 6, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    }

    /* Highlight the stations in the path with a nice glow effect */
    for (size_t i = 0; i < path.size(); i++)
    {
        int stationId = path[i];
        double x = stations[stationId].x;
        double y = stations[stationId].y;

        /* Outer glow for stations */
        scene()->addEllipse(x - 12, y - 12, 24, 24,
                            QPen(QColor(0, 255, 102, 70), 2), QBrush(QColor(0, 255, 102, 15)));

        /* Main station highlight */
        scene()->addEllipse(x - 8, y - 8, 16, 16,
                            QPen(QColor(0x00FF66), 4), QBrush(Qt::transparent));

        /* Emphasize start and end stations with an extra highlight */
        if (i == 0 || i == path.size() - 1)
        {
            /* Pulsating outer ring for start/end */
            scene()->addEllipse(x - 16, y - 16, 32, 32,
                                QPen(QColor(0x00FFCC), 2, Qt::DotLine), QBrush(Qt::transparent));

            /* Inner dot to mark it special */
            scene()->addEllipse(x - 3, y - 3, 6, 6,
                                QPen(Qt::transparent), QBrush(QColor(0x00FF66)));
        }
    }
}

void MetroMapView::clearRoute()
{
    scene()->clear();
}

void MetroMapView::resizeEvent(QResizeEvent *event)
{
    QGraphicsView::resizeEvent(event);
    fitInView(scene()->sceneRect(), Qt::KeepAspectRatio);
}
