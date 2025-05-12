#ifndef METROPLANNERWINDOW_H
#define METROPLANNERWINDOW_H

#include <QMainWindow>
#include <QComboBox>
#include <QCheckBox>
#include <QTextEdit>
#include <QPushButton>
#include <vector>
#include <unordered_map>
#include <string>
#include "MetroData.h"

class MetroMapView;

/**
 * @brief Main application window for the Metro Route Planner
 *
 * This class provides the main user interface for the metro route planning
 * application, including station selection, route finding, and visualization.
 */
class MetroPlannerWindow : public QMainWindow
{
    Q_OBJECT
public:
    /**
     * @brief Construct the main application window
     * @param parent Optional parent widget
     */
    MetroPlannerWindow(QWidget *parent = nullptr);

private slots:
    /**
     * @brief Swap the source and destination stations
     */
    void swapStations();

    /**
     * @brief Calculate and display the optimal route between selected stations
     */
    void findRoute();

private:
    /**
     * @brief Initialize the metro station data
     */
    void initializeStations();

    /**
     * @brief Fill the station selection dropdown menus
     */
    void populateStationCombos();

    /**
     * @brief Initialize the metro network graph
     */
    void initializeGraph();

    /**
     * @brief Draw the metro map in the map view
     */
    void drawMetroMap();

    /**
     * @brief Get the display color for a metro line
     * @param line The name of the metro line
     * @return QString containing the color code
     */
    QString getLineColor(const std::string &line);

    QComboBox *fromStation, *toStation; /**< Station selection dropdowns */
    QCheckBox *holidayCheck;            /**< Holiday rate checkbox */
    QCheckBox *metroCardCheck;          /**< Metro card discount checkbox */
    QPushButton *findRouteBtn;          /**< Route finding button */
    QTextEdit *routeDetails;            /**< Text area for displaying route details */
    MetroMapView *mapView;              /**< Visual map of the metro network */

    std::vector<Station> stations;                       /**< Collection of all metro stations */
    std::unordered_map<std::string, Station> stationMap; /**< Map for quick station lookup by name */
    std::vector<std::vector<Edge>> graph;                /**< Network graph representation */
};

#endif // METROPLANNERWINDOW_H
