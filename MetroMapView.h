#ifndef METROMAPVIEW_H
#define METROMAPVIEW_H

#include <QGraphicsView>
#include <QResizeEvent>
#include "MetroData.h"

/**
 * @brief Visual representation of the metro network
 *
 * This class provides a graphical view of the metro network with
 * stations and lines, and supports highlighting routes.
 */
class MetroMapView : public QGraphicsView
{
    Q_OBJECT
public:
    /**
     * @brief Construct a new MetroMapView
     * @param parent Optional parent widget
     */
    MetroMapView(QWidget *parent = nullptr);

    /**
     * @brief Draw a station on the map
     *
     * @param name Name of the station
     * @param x X-coordinate on the map
     * @param y Y-coordinate on the map
     * @param line Metro line(s) passing through this station
     */
    void drawStation(const QString &name, double x, double y, const QString &line);

    /**
     * @brief Draw a metro line between two points
     *
     * @param x1 Starting X-coordinate
     * @param y1 Starting Y-coordinate
     * @param x2 Ending X-coordinate
     * @param y2 Ending Y-coordinate
     * @param line Name of the metro line to determine color
     */
    void drawLine(double x1, double y1, double x2, double y2, const QString &line);

    /**
     * @brief Highlight a path on the map
     *
     * Creates a visual highlight effect for stations and connections
     * along the specified path.
     *
     * @param path Vector of station IDs representing the route
     * @param stations Vector of all metro stations
     */
    void highlightPath(const std::vector<int> &path, const std::vector<Station> &stations);

    /**
     * @brief Clear all routes and highlights from the map
     */
    void clearRoute();

protected:
    /**
     * @brief Handle resize events to maintain proper map scaling
     * @param event The resize event
     */
    void resizeEvent(QResizeEvent *event) override;
};

#endif // METROMAPVIEW_H
