#include "MetroPlannerWindow.h"
#include "MetroMapView.h"
#include "RouteCalculator.h"
#include "MetroData.h"
#include "Visualization.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>
#include <QPainter>
#include <QPixmap>
#include <algorithm> /* Needed for std::find */
#include <cmath>
#include <climits>

using namespace std;

/* Implementation of MetroPlannerWindow members */
MetroPlannerWindow::MetroPlannerWindow(QWidget *parent) : QMainWindow(parent)
{
    setWindowTitle("Metro Route Optimizer");
    setMinimumSize(1200, 800);

    auto *centralWidget = new QWidget;
    setCentralWidget(centralWidget);

    auto *mainLayout = new QHBoxLayout(centralWidget);

    /* Left panel for controls */
    auto *controlsPanel = new QWidget;
    auto *controlsLayout = new QVBoxLayout(controlsPanel);
    controlsPanel->setMaximumWidth(400);

    /* Station selection group */
    auto *stationGroup = new QGroupBox("Plan Your Journey");
    auto *stationLayout = new QVBoxLayout(stationGroup);

    fromStation = new QComboBox;
    toStation = new QComboBox;
    initializeStations();
    populateStationCombos();

    auto *fromLayout = new QHBoxLayout;
    fromLayout->addWidget(new QLabel("From:"));
    fromLayout->addWidget(fromStation);

    auto *toLayout = new QHBoxLayout;
    toLayout->addWidget(new QLabel("To:"));
    toLayout->addWidget(toStation);

    auto *swapBtn = new QPushButton;
    /* Create a custom icon with two separate arrows side by side with the color #3B9CFF */
    QIcon swapIcon;
    QPixmap pixmap(32, 24);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.setPen(QPen(QColor(0x3B9CFF), 2));

    /* Draw up arrow on the left side */
    painter.drawLine(8, 18, 8, 4); /* Vertical line for up arrow */
    painter.drawLine(3, 9, 8, 4);  /* Left side of up arrow head */
    painter.drawLine(13, 9, 8, 4); /* Right side of up arrow head */

    /* Draw down arrow on the right side */
    painter.drawLine(24, 4, 24, 18);  /* Vertical line for down arrow */
    painter.drawLine(19, 13, 24, 18); /* Left side of down arrow head */
    painter.drawLine(29, 13, 24, 18); /* Right side of down arrow head */

    painter.end();
    swapIcon.addPixmap(pixmap);
    swapBtn->setIcon(swapIcon);
    swapBtn->setIconSize(QSize(32, 24));
    swapBtn->setToolTip("Swap stations");
    connect(swapBtn, &QPushButton::clicked, this, &MetroPlannerWindow::swapStations);

    holidayCheck = new QCheckBox("Holiday/Sunday");
    metroCardCheck = new QCheckBox("Metro Card (10% discount)");

    stationLayout->addLayout(fromLayout);
    stationLayout->addWidget(swapBtn);
    stationLayout->addLayout(toLayout);
    stationLayout->addWidget(holidayCheck);
    stationLayout->addWidget(metroCardCheck);

    /* Route details without background styling */
    routeDetails = new QTextEdit;
    routeDetails->setReadOnly(true);
    routeDetails->setMinimumHeight(250);
    routeDetails->setStyleSheet("QTextEdit { border: 1px solid #ddd; border-radius: 4px; }");

    /* Find route button */
    findRouteBtn = new QPushButton("Find Route");
    findRouteBtn->setStyleSheet("background-color: #3b82f6; color: white; padding: 10px;");
    connect(findRouteBtn, &QPushButton::clicked, this, &MetroPlannerWindow::findRoute);

    controlsLayout->addWidget(stationGroup);
    controlsLayout->addWidget(routeDetails);
    controlsLayout->addWidget(findRouteBtn);
    controlsLayout->addStretch();

    /* Map view */
    mapView = new MetroMapView;

    mainLayout->addWidget(controlsPanel);
    mainLayout->addWidget(mapView);

    /* Initialize the map */
    initializeGraph();
    drawMetroMap();
}

void MetroPlannerWindow::swapStations()
{
    int fromIdx = fromStation->currentIndex();
    int toIdx = toStation->currentIndex();
    fromStation->setCurrentIndex(toIdx);
    toStation->setCurrentIndex(fromIdx);
}

void MetroPlannerWindow::findRoute()
{
    int fromIdx = fromStation->currentIndex();
    int toIdx = toStation->currentIndex();

    if (fromIdx == toIdx)
    {
        QMessageBox::warning(this, "Invalid Selection", "Source and destination stations are the same.");
        return;
    }

    /* Get the actual station IDs */
    int startId = stationMap[fromStation->currentText().toStdString()].id;
    int endId = stationMap[toStation->currentText().toStdString()].id;

    /* Compute the shortest path */
    vector<int> distances, previous;
    dijkstra(startId, graph, distances, previous);

    if (distances[endId] == INT_MAX)
    {
        routeDetails->setText("No route found between these stations.");
        return;
    }

    /* Reconstruct the path */
    vector<int> path = reconstructPath(startId, endId, previous, stations);

    /* Calculate distance and fare */
    double totalDistance = calculatePathDistance(path, graph);
    int baseFare = calculateFare(totalDistance, holidayCheck->isChecked());

    /* Generate HTML route information using the Visualization module */
    QString routeInfoHTML = getRouteHTML(
        path, stations, distances[endId], totalDistance, baseFare,
        holidayCheck->isChecked(), metroCardCheck->isChecked());

    routeDetails->setHtml(routeInfoHTML);

    /* Extract unique path for highlighting (same logic as in getRouteHTML) */
    vector<vector<string>> path_lines;
    vector<string> station_names;
    vector<int> unique_path;

    /* Create a unique path without duplicate station names */
    for (int idx : path)
    {
        if (station_names.empty() || stations[idx].name != station_names.back())
        {
            station_names.push_back(stations[idx].name);
            unique_path.push_back(idx);
            path_lines.push_back(splitLines(stations[idx].line));
        }
    }

    /* Redraw the map with the highlighted path */
    drawMetroMap();
    mapView->highlightPath(unique_path, stations);
}

void MetroPlannerWindow::initializeStations()
{
    /* Use the centralized function from MetroData to initialize stations and graph */
    initializeMetroNetwork(stations, graph);

    /* Set visualization coordinates for each station */
    /* Blue Line (Major stations) */
    stations[0].x = 50;
    stations[0].y = 300; /* Dwarka Sec-21 */
    stations[1].x = 150;
    stations[1].y = 300; /* Janakpuri West */
    stations[2].x = 250;
    stations[2].y = 300; /* Rajouri Garden */
    stations[3].x = 400;
    stations[3].y = 300; /* Rajiv Chowk */
    stations[4].x = 500;
    stations[4].y = 300; /* Mandi House */
    stations[5].x = 600;
    stations[5].y = 300; /* Yamuna Bank */
    stations[6].x = 700;
    stations[6].y = 300; /* Mayur Vihar Phase-1 */
    stations[7].x = 820;
    stations[7].y = 300; /* Noida City Centre */
    stations[8].x = 850;
    stations[8].y = 250; /* Vaishali */

    /* Yellow Line (Major stations) */
    stations[9].x = 400;
    stations[9].y = 50; /* Samaypur Badli */
    stations[10].x = 400;
    stations[10].y = 100; /* Azadpur */
    stations[11].x = 400;
    stations[11].y = 150; /* Kashmere Gate */
    stations[12].x = 400;
    stations[12].y = 200; /* Chandni Chowk */
    stations[13].x = 400;
    stations[13].y = 300; /* Rajiv Chowk */
    stations[14].x = 400;
    stations[14].y = 400; /* Central Secretariat */
    stations[15].x = 400;
    stations[15].y = 440; /* INA */
    stations[16].x = 400;
    stations[16].y = 480; /* AIIMS */
    stations[17].x = 400;
    stations[17].y = 520; /* Hauz Khas */
    stations[18].x = 400;
    stations[18].y = 600; /* HUDA City Centre */

    /* Red Line (Major stations) */
    stations[19].x = 220;
    stations[19].y = 150; /* Rithala */
    stations[20].x = 300;
    stations[20].y = 150; /* Netaji Subhash Place */
    stations[21].x = 400;
    stations[21].y = 150; /* Kashmere Gate */
    stations[22].x = 500;
    stations[22].y = 150; /* Welcome */

    /* Pink Line (Major stations) */
    stations[23].x = 300;
    stations[23].y = 100; /* Majlis Park */
    stations[24].x = 400;
    stations[24].y = 100; /* Azadpur */
    stations[25].x = 300;
    stations[25].y = 150; /* Netaji Subhash Place */
    stations[26].x = 250;
    stations[26].y = 300; /* Rajouri Garden */
    stations[27].x = 400;
    stations[27].y = 440; /* INA */
    stations[28].x = 700;
    stations[28].y = 300; /* Mayur Vihar Phase-1 */

    /* Magenta Line (Major stations) */
    stations[29].x = 150;
    stations[29].y = 300; /* Janakpuri West */
    stations[30].x = 200;
    stations[30].y = 400; /* Terminal 1 IGI Airport */
    stations[31].x = 400;
    stations[31].y = 520; /* Hauz Khas */
    stations[32].x = 750;
    stations[32].y = 350; /* Botanical Garden */

    /* Build station map for quick lookup */
    for (const auto &station : stations)
    {
        stationMap[station.name] = station;
    }
}

void MetroPlannerWindow::populateStationCombos()
{
    /* Sort station names alphabetically */
    vector<string> stationNames;
    for (const auto &pair : stationMap)
    {
        stationNames.push_back(pair.first);
    }
    sort(stationNames.begin(), stationNames.end());

    /* Populate comboboxes */
    for (const auto &name : stationNames)
    {
        fromStation->addItem(QString::fromStdString(name));
        toStation->addItem(QString::fromStdString(name));
    }
}

void MetroPlannerWindow::initializeGraph()
{
    /* This function is empty as we're now using initializeMetroNetwork in initializeStations */
}

void MetroPlannerWindow::drawMetroMap()
{
    mapView->clearRoute();

    /* Draw Blue Line */
    for (int i = 0; i < 8; i++)
    {
        mapView->drawLine(
            stations[i].x, stations[i].y,
            stations[i + 1].x, stations[i + 1].y,
            "Blue");
    }

    /* Draw Yellow Line */
    for (int i = 9; i < 18; i++)
    {
        mapView->drawLine(
            stations[i].x, stations[i].y,
            stations[i + 1].x, stations[i + 1].y,
            "Yellow");
    }

    /* Draw Red Line */
    for (int i = 19; i < 22; i++)
    {
        mapView->drawLine(
            stations[i].x, stations[i].y,
            stations[i + 1].x, stations[i + 1].y,
            "Red");
    }

    /* Draw Pink Line */
    for (int i = 23; i < 28; i++)
    {
        mapView->drawLine(
            stations[i].x, stations[i].y,
            stations[i + 1].x, stations[i + 1].y,
            "Pink");
    }

    /* Draw Magenta Line */
    for (int i = 29; i < 32; i++)
    {
        mapView->drawLine(
            stations[i].x, stations[i].y,
            stations[i + 1].x, stations[i + 1].y,
            "Magenta");
    }

    /* Draw all stations */
    for (const auto &station : stations)
    {
        mapView->drawStation(
            QString::fromStdString(station.name),
            station.x, station.y,
            QString::fromStdString(station.line));
    }
}
