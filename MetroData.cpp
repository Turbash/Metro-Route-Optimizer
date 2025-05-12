#include "MetroData.h"
#include <vector>
#include <algorithm>

using namespace std;

// Helper function to split lines by '/'
vector<string> splitLines(const string &lines)
{
    vector<string> result;
    size_t start = 0, end = 0;

    while ((end = lines.find('/', start)) != string::npos)
    {
        result.push_back(lines.substr(start, end - start));
        start = end + 1;
    }

    /* Include the last segment */
    result.push_back(lines.substr(start));
    return result;
}

void initializeMetroNetwork(vector<Station> &stations, vector<vector<Edge>> &graph)
{
    /* Define Delhi Metro stations with default visualization coordinates */
    stations = {
        /* Blue Line (Major stations) */
        {0, "Dwarka Sec-21", "Blue", 0.0, 0.0},
        {1, "Janakpuri West", "Blue/Magenta", 0.0, 0.0},
        {2, "Rajouri Garden", "Blue/Pink", 0.0, 0.0},
        {3, "Rajiv Chowk", "Blue/Yellow", 0.0, 0.0},
        {4, "Mandi House", "Blue/Violet", 0.0, 0.0},
        {5, "Yamuna Bank", "Blue", 0.0, 0.0},
        {6, "Mayur Vihar Phase-1", "Blue/Pink", 0.0, 0.0},
        {7, "Noida City Centre", "Blue", 0.0, 0.0},
        {8, "Vaishali", "Blue", 0.0, 0.0},

        /* Yellow Line (Major stations) */
        {9, "Samaypur Badli", "Yellow", 0.0, 0.0},
        {10, "Azadpur", "Yellow/Pink", 0.0, 0.0},
        {11, "Kashmere Gate", "Yellow/Red/Violet", 0.0, 0.0},
        {12, "Chandni Chowk", "Yellow", 0.0, 0.0},
        {13, "Rajiv Chowk", "Yellow/Blue", 0.0, 0.0},
        {14, "Central Secretariat", "Yellow/Violet", 0.0, 0.0},
        {15, "INA", "Yellow/Pink", 0.0, 0.0},
        {16, "AIIMS", "Yellow", 0.0, 0.0},
        {17, "Hauz Khas", "Yellow/Magenta", 0.0, 0.0},
        {18, "HUDA City Centre", "Yellow", 0.0, 0.0},

        /* Red Line (Major stations) */
        {19, "Rithala", "Red", 0.0, 0.0},
        {20, "Netaji Subhash Place", "Red/Pink", 0.0, 0.0},
        {21, "Kashmere Gate", "Red/Yellow/Violet", 0.0, 0.0},
        {22, "Welcome", "Red/Pink", 0.0, 0.0},

        /* Pink Line (Major stations) */
        {23, "Majlis Park", "Pink", 0.0, 0.0},
        {24, "Azadpur", "Pink/Yellow", 0.0, 0.0},
        {25, "Netaji Subhash Place", "Pink/Red", 0.0, 0.0},
        {26, "Rajouri Garden", "Pink/Blue", 0.0, 0.0},
        {27, "INA", "Pink/Yellow", 0.0, 0.0},
        {28, "Mayur Vihar Phase-1", "Pink/Blue", 0.0, 0.0},

        /* Magenta Line (Major stations) */
        {29, "Janakpuri West", "Magenta/Blue", 0.0, 0.0},
        {30, "Terminal 1 IGI Airport", "Magenta", 0.0, 0.0},
        {31, "Hauz Khas", "Magenta/Yellow", 0.0, 0.0},
        {32, "Botanical Garden", "Magenta/Blue", 0.0, 0.0}};

    int n = stations.size();
    graph.resize(n);

    /* Helper function to add bidirectional edge */
    auto addEdge = [&](int from, int to, int time, double dist)
    {
        graph[from].push_back({to, time, dist});
        graph[to].push_back({from, time, dist});
    };

    /* Blue Line connections */
    for (int i = 0; i < 8; ++i)
    {
        addEdge(i, i + 1, 5, 2.5); /* 2.5 KM between stations average */
    }

    /* Yellow Line connections */
    for (int i = 9; i < 18; ++i)
    {
        addEdge(i, i + 1, 5, 2.5);
    }

    /* Red Line connections */
    for (int i = 19; i < 22; ++i)
    {
        addEdge(i, i + 1, 5, 2.5);
    }

    /* Pink Line connections */
    for (int i = 23; i < 28; ++i)
    {
        addEdge(i, i + 1, 5, 2.5);
    }

    /* Magenta Line connections */
    for (int i = 29; i < 32; ++i)
    {
        addEdge(i, i + 1, 5, 2.5);
    }

    /* Interchange connections (reduced transfer time to 2 minutes) */
    /* Blue-Yellow */
    addEdge(3, 13, 2, 0.1); /* Rajiv Chowk */
    /* Blue-Pink */
    addEdge(1, 29, 2, 0.1); /* Janakpuri West */
    addEdge(2, 26, 2, 0.1); /* Rajouri Garden */
    addEdge(6, 28, 2, 0.1); /* Mayur Vihar Phase-1 */
    /* Yellow-Red */
    addEdge(11, 21, 2, 0.1); /* Kashmere Gate */
    /* Yellow-Pink */
    addEdge(10, 24, 2, 0.1); /* Azadpur */
    /* Yellow-Magenta */
    addEdge(17, 31, 2, 0.1); /* Hauz Khas */
    /* Red-Pink */
    addEdge(20, 25, 2, 0.1); /* Netaji Subhash Place */
    /* Magenta-Blue */
    addEdge(29, 1, 2, 0.1); /* Janakpuri West */
    addEdge(32, 7, 2, 0.1); /* Botanical Garden-Noida City Centre */
    /* Pink-Yellow at INA */
    addEdge(15, 27, 2, 0.1); /* INA */
}
