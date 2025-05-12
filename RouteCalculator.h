#ifndef ROUTECALCULATOR_H
#define ROUTECALCULATOR_H

#include "MetroData.h"
#include <vector>

/**
 * @brief Implements Dijkstra's algorithm for finding the shortest path
 * @param start Starting station ID
 * @param graph Adjacency list representation of the metro network
 * @param distances Output vector to store distances from start to all nodes
 * @param previous Output vector to store the previous node in the optimal path
 */
void dijkstra(int start, const std::vector<std::vector<Edge>> &graph,
              std::vector<int> &distances, std::vector<int> &previous);

/**
 * @brief Reconstructs the path from start to end using the previous nodes array
 * @param start Starting station ID
 * @param end Ending station ID
 * @param previous Vector containing previous nodes in optimal path
 * @param stations Vector of all available stations
 * @return Vector of station IDs representing the optimal path
 */
std::vector<int> reconstructPath(int start, int end, const std::vector<int> &previous,
                                 const std::vector<Station> &stations);

/**
 * @brief Calculates the fare based on distance and holiday status
 * @param distance Total distance in kilometers
 * @param isHoliday Boolean indicating if it's a holiday/Sunday (affects fare)
 * @return Integer representing the fare in INR
 */
int calculateFare(double distance, bool isHoliday);

/**
 * @brief Calculates the total distance of a path
 * @param path Vector of station IDs representing the path
 * @param graph Adjacency list representation of the metro network
 * @return Total distance in kilometers
 */
double calculatePathDistance(const std::vector<int> &path, const std::vector<std::vector<Edge>> &graph);

#endif // ROUTECALCULATOR_H
