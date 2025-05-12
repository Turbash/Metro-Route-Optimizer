#ifndef METRODATA_H
#define METRODATA_H

#include <string>
#include <vector>

/**
 * @brief Station data structure representing a metro station
 *
 * Contains all relevant information about a metro station including
 * its ID, name, lines it belongs to, and coordinates for visualization.
 */
struct Station
{
    int id;           /**< Unique identifier for the station */
    std::string name; /**< Name of the station */
    std::string line; /**< Metro line(s) passing through this station (slash-separated) */
    double x;         /**< X-coordinate for visualization purposes */
    double y;         /**< Y-coordinate for visualization purposes */
};

/**
 * @brief Edge data structure representing a connection between stations
 *
 * Contains information about connections between metro stations including
 * travel time and physical distance.
 */
struct Edge
{
    int destination; /**< ID of the destination station */
    int weight;      /**< Travel time in minutes */
    double distance; /**< Physical distance in kilometers */
};

/**
 * @brief Splits a string of metro lines separated by slashes
 *
 * @param lines A string containing line names separated by '/' character
 * @return Vector of individual line names
 */
std::vector<std::string> splitLines(const std::string &lines);

/**
 * @brief Initializes the complete Delhi Metro network data
 *
 * Creates all stations and connections between them with appropriate
 * travel times and distances. This function serves as the central
 * data source for the entire application.
 *
 * @param stations Output vector to be filled with station information
 * @param graph Output adjacency list to be filled with connections between stations
 */
void initializeMetroNetwork(std::vector<Station> &stations, std::vector<std::vector<Edge>> &graph);

#endif // METRODATA_H
