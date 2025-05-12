#ifndef VISUALIZATION_H
#define VISUALIZATION_H

#include "MetroData.h"
#include <vector>
#include <string>
#include <QString>

/**
 * @brief Returns HTML color code for a metro line
 * @param line The name of the metro line
 * @return QString containing the HTML color code for the specified line
 */
QString getLineColorHTML(const std::string &line);

/**
 * @brief Generate HTML-formatted route information for display in the Qt interface
 * @param path Vector of station IDs in the route path
 * @param stations Vector of all available metro stations
 * @param travelTime Total travel time in minutes
 * @param totalDistance Total distance in kilometers
 * @param fare Base fare calculated for the journey
 * @param isHoliday Boolean indicating if it's a holiday/Sunday (affects fare)
 * @param hasMetroCard Boolean indicating if the user has a metro card (for discounts)
 * @return QString containing HTML-formatted route information
 */
QString getRouteHTML(const std::vector<int> &path, const std::vector<Station> &stations,
                     int travelTime, double totalDistance, int fare,
                     bool isHoliday = false, bool hasMetroCard = false);

#endif /*VISUALIZATION_H*/ 
