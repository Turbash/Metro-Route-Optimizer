#include "RouteCalculator.h"
#include <climits>
#include <algorithm>
#include <unordered_set>

using namespace std;

void dijkstra(int start, const vector<vector<Edge>> &graph,
              vector<int> &distances, vector<int> &previous)
{
    int n = graph.size();
    distances.assign(n, INT_MAX);
    previous.assign(n, -1);
    distances[start] = 0;

    vector<bool> visited(n, false);

    for (int i = 0; i < n; i++)
    {
        /* Find the unvisited node with minimum distance */
        int minDist = INT_MAX;
        int current = -1;

        for (int j = 0; j < n; j++)
        {
            if (!visited[j] && distances[j] < minDist)
            {
                minDist = distances[j];
                current = j;
            }
        }

        if (current == -1)
            break; /* No reachable unvisited nodes */

        visited[current] = true;

        /* Update distances to neighbors */
        for (const Edge &edge : graph[current])
        {
            int next = edge.destination;
            int newDist = distances[current] + edge.weight;

            if (newDist < distances[next])
            {
                distances[next] = newDist;
                previous[next] = current;
            }
        }
    }
}

vector<int> reconstructPath(int start, int end, const vector<int> &previous,
                            const vector<Station> &stations)
{
    vector<int> path;
    for (int at = end; at != -1; at = previous[at])
    {
        path.push_back(at);
    }
    reverse(path.begin(), path.end());

    if (path[0] == start)
    {
        /* Remove duplicate stations with the same name if needed */
        vector<int> unique_path;
        unordered_set<string> seen_names;

        for (const int &idx : path)
        {
            const string &station_name = stations[idx].name;
            if (seen_names.count(station_name) == 0)
            {
                unique_path.push_back(idx);
                seen_names.insert(station_name);
            }
        }
        return unique_path;
    }

    return {}; /* Return empty if no path exists */
}

int calculateFare(double distance, bool isHoliday)
{
    /* Delhi Metro fare structure based on distance */
    if (distance <= 2)
        return 10;
    if (distance <= 5)
        return isHoliday ? 10 : 20;
    if (distance <= 12)
        return isHoliday ? 20 : 30;
    if (distance <= 21)
        return isHoliday ? 30 : 40;
    if (distance <= 32)
        return isHoliday ? 40 : 50;
    return isHoliday ? 50 : 60;
}

double calculatePathDistance(const vector<int> &path, const vector<vector<Edge>> &graph)
{
    double totalDist = 0;
    for (size_t i = 0; i < path.size() - 1; i++)
    {
        for (const Edge &edge : graph[path[i]])
        {
            if (edge.destination == path[i + 1])
            {
                totalDist += edge.distance;
                break;
            }
        }
    }
    return totalDist;
}
