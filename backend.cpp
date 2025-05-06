#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <unordered_map>
#include <string>
#include <algorithm> // for reverse()


using namespace std;

struct Edge {
    int destination;
    int weight; // time in minutes
};

// Delhi Metro station data structure
struct Station {
    int id;
    string name;
    string line;
};

// Helper function to split lines by '/'
vector<string> splitLines(const string& lines) {
    vector<string> result;
    size_t start = 0, end = 0;
    while ((end = lines.find('/', start)) != string::npos) {
        result.push_back(lines.substr(start, end - start));
        start = end + 1;
    }
    result.push_back(lines.substr(start));
    return result;
}

// Visualisation function
void visualiseMetroNetwork(const vector<Station>& stations, const vector<vector<Edge>>& graph) {
    cout << "\nMetro Network Visualization (Line Sequences):" << endl;
    unordered_map<string, vector<int>> line_to_stations;
    unordered_map<string, bool> valid_lines = {
        {"Blue", true}, {"Yellow", true}, {"Red", true}, {"Pink", true}
    };
    // Only collect stations for the 4 lines
    for (int i = 0; i < stations.size(); ++i) {
        vector<string> lines = splitLines(stations[i].line);
        for (const string& line : lines) {
            if (valid_lines.count(line))
                line_to_stations[line].push_back(i);
        }
    }
    // For each line, build a sequence of stations as per their connections
    for (const auto& entry : line_to_stations) {
        const string& line = entry.first;
        const vector<int>& all_stations = entry.second;
        // Build adjacency for this line
        unordered_map<int, vector<int>> adj;
        unordered_map<int, int> degree;
        for (int idx : all_stations) {
            for (const Edge& e : graph[idx]) {
                // Only consider connections to stations on the same line
                vector<string> dest_lines = splitLines(stations[e.destination].line);
                if (find(dest_lines.begin(), dest_lines.end(), line) != dest_lines.end()) {
                    adj[idx].push_back(e.destination);
                    degree[idx]++;
                }
            }
        }
        // Find start station (degree 1, or lowest index if circular)
        int start_station = -1;
        for (int idx : all_stations) {
            if (adj[idx].size() == 1) {
                start_station = idx;
                break;
            }
        }
        if (start_station == -1 && !all_stations.empty()) {
            start_station = all_stations[0]; // fallback
        }
        // Traverse the line
        vector<int> sequence;
        unordered_map<int, bool> visited;
        int prev = -1, curr = start_station;
        while (curr != -1) {
            sequence.push_back(curr);
            visited[curr] = true;
            int next = -1;
            for (int neighbor : adj[curr]) {
                if (!visited[neighbor]) {
                    next = neighbor;
                    break;
                }
            }
            prev = curr;
            curr = next;
        }
        // Output the line sequence
        cout << "\n[" << line << " Line]: ";
        for (size_t i = 0; i < sequence.size(); ++i) {
            cout << stations[sequence[i]].name << " [" << sequence[i] << "]";
            if (i < sequence.size() - 1) cout << " -> ";
        }
        cout << endl;
    }
    // List all interchange stations (only if they are junctions between Blue, Yellow, Red, Pink)
    vector<int> interchange_stations;
    for (int i = 0; i < stations.size(); ++i) {
        vector<string> lines = splitLines(stations[i].line);
        int valid_count = 0;
        for (const string& l : lines) {
            if (valid_lines.count(l)) ++valid_count;
        }
        if (valid_count > 1)
            interchange_stations.push_back(i);
    }
    cout << "\nInterchange Stations (can switch lines here):" << endl;
    for (int idx : interchange_stations) {
        cout << "  " << stations[idx].name << " [" << idx << "] (" << stations[idx].line << ")" << endl;
    }
}

void dijkstra(int start, const vector<vector<Edge>>& graph, vector<int>& distances, vector<int>& previous) {
    int n = graph.size();
    distances.assign(n, INT_MAX);
    previous.assign(n, -1);
    distances[start] = 0;

    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;
    pq.push({0, start});

    while (!pq.empty()) {
        int currentDistance = pq.top().first;
        int currentNode = pq.top().second;
        pq.pop();

        if (currentDistance > distances[currentNode]) continue;

        for (const Edge& edge : graph[currentNode]) {
            int nextNode = edge.destination;
            int newDistance = currentDistance + edge.weight;

            if (newDistance < distances[nextNode]) {
                distances[nextNode] = newDistance;
                previous[nextNode] = currentNode;
                pq.push({newDistance, nextNode});
            }
        }
    }
}

vector<int> reconstructPath(int start, int end, const vector<int>& previous) {
    vector<int> path;
    for (int at = end; at != -1; at = previous[at]) {
        path.push_back(at);
    }
    reverse(path.begin(), path.end());
    if (path[0] == start) return path;
    return {}; // Return empty if no path exists
}

int main() {
    // Define Delhi Metro stations
    vector<Station> stations = {
        // Blue Line
        {0, "Dwarka Sec-21", "Blue"},
        {1, "Dwarka Sec-8", "Blue"},
        {2, "Dwarka Sec-10", "Blue"},
        {3, "Dwarka Sec-12", "Blue"},
        {4, "Dwarka Sec-14", "Blue"},
        {5, "Janakpuri West", "Blue/Pink"},
        {6, "Uttam Nagar West", "Blue"},
        {7, "Tilak Nagar", "Blue"},
        {8, "Rajouri Garden", "Blue/Pink"},
        {9, "Karol Bagh", "Blue"},
        {10, "Rajiv Chowk", "Blue/Yellow"},
        {11, "Barakhamba Road", "Blue"},
        {12, "Mandi House", "Blue/Violet"},
        {13, "Yamuna Bank", "Blue"},
        {14, "Mayur Vihar Phase-1", "Blue/Pink"},
        {15, "Akshardham", "Blue"},
        {16, "Noida Sector 18", "Blue"},
        {17, "Noida City Centre", "Blue"},
        {18, "Vaishali", "Blue"},
        {19, "Anand Vihar", "Blue/Pink"},

        // Yellow Line
        {20, "Samaypur Badli", "Yellow"},
        {21, "Rohini Sector 18,19", "Yellow"},
        {22, "Haiderpur Badli Mor", "Yellow"},
        {23, "Jahangirpuri", "Yellow"},
        {24, "Adarsh Nagar", "Yellow"},
        {25, "Azadpur", "Yellow/Pink"},
        {26, "Model Town", "Yellow"},
        {27, "GTB Nagar", "Yellow"},
        {28, "Vishwavidyalaya", "Yellow"},
        {29, "Vidhan Sabha", "Yellow"},
        {30, "Civil Lines", "Yellow"},
        {31, "Kashmere Gate", "Yellow/Red/Violet"},
        {32, "Chandni Chowk", "Yellow"},
        {33, "Rajiv Chowk", "Yellow/Blue"},
        {34, "Central Secretariat", "Yellow/Violet"},
        {35, "INA", "Yellow/Pink"},
        {36, "AIIMS", "Yellow"},
        {37, "Hauz Khas", "Yellow/Magenta"},
        {38, "Saket", "Yellow"},
        {39, "Qutub Minar", "Yellow"},
        {40, "Chhatarpur", "Yellow"},
        {41, "Sultanpur", "Yellow"},
        {42, "Ghitorni", "Yellow"},
        {43, "Arjan Garh", "Yellow"},
        {44, "Guru Dronacharya", "Yellow"},
        {45, "Sikanderpur", "Yellow"},
        {46, "MG Road", "Yellow"},
        {47, "IFFCO Chowk", "Yellow"},
        {48, "HUDA City Centre", "Yellow"},

        // Red Line
        {49, "Rithala", "Red"},
        {50, "Rohini West", "Red"},
        {51, "Rohini East", "Red"},
        {52, "Pitampura", "Red"},
        {53, "Kohat Enclave", "Red"},
        {54, "Netaji Subhash Place", "Red/Pink"},
        {55, "Keshav Puram", "Red"},
        {56, "Kanhaiya Nagar", "Red"},
        {57, "Inderlok", "Red/Green"},
        {58, "Shastri Nagar", "Red"},
        {59, "Pratap Nagar", "Red"},
        {60, "Pulbangash", "Red"},
        {61, "Tis Hazari", "Red"},
        {62, "Kashmere Gate", "Red/Yellow/Violet"},
        {63, "Shastri Park", "Red"},
        {64, "Seelampur", "Red"},
        {65, "Welcome", "Red/Pink"},
        {66, "Shahdara", "Red"},
        {67, "Mansarovar Park", "Red"},
        {68, "Jhilmil", "Red"},
        {69, "Dilshad Garden", "Red"},
        {70, "Shaheed Nagar", "Red"},
        {71, "Raj Bagh", "Red"},
        {72, "Shaheed Sthal (New Bus Adda)", "Red"},

        // Pink Line
        {73, "Majlis Park", "Pink"},
        {74, "Azadpur", "Pink/Yellow"},
        {75, "Shalimar Bagh", "Pink"},
        {76, "Netaji Subhash Place", "Pink/Red"},
        {77, "Shakurpur", "Pink"},
        {78, "Punjabi Bagh West", "Pink"},
        {79, "Rajouri Garden", "Pink/Blue"},
        {80, "Mayapuri", "Pink"},
        {81, "Naraina Vihar", "Pink"},
        {82, "Delhi Cantt", "Pink"},
        {83, "Durgabai Deshmukh South Campus", "Pink"},
        {84, "Lajpat Nagar", "Pink/Violet"},
        {85, "Mayur Vihar Phase-1", "Pink/Blue"},
        {86, "Anand Vihar", "Pink/Blue"},
        {87, "Sarai Kale Khan", "Pink"},
        {88, "Hazrat Nizamuddin", "Pink"},
        {89, "Vinobapuri", "Pink"},
        {90, "Ashram", "Pink"},
        {91, "Sukhdev Vihar", "Pink"},
        {92, "Okhla Vihar", "Pink"},
        {93, "Jasola Vihar Shaheen Bagh", "Pink"},
        {94, "Kalindi Kunj", "Pink"},
        {95, "Okhla Bird Sanctuary", "Pink"}
    };

    int n = stations.size();
    vector<vector<Edge>> graph(n);

    // --- BLUE LINE CONNECTIONS ---
    for (int i = 0; i < 19; ++i) { // 0-18 are Blue line stations in order
        graph[i].push_back({i+1, 3});
        graph[i+1].push_back({i, 3});
    }
    // Connect Blue/Pink interchanges
    graph[5].push_back({8, 4}); graph[8].push_back({5, 4}); // Janakpuri West <-> Rajouri Garden
    graph[8].push_back({14, 5}); graph[14].push_back({8, 5}); // Rajouri Garden <-> Mayur Vihar Phase-1
    graph[14].push_back({19, 6}); graph[19].push_back({14, 6}); // Mayur Vihar Phase-1 <-> Anand Vihar

    // --- YELLOW LINE CONNECTIONS ---
    for (int i = 20; i < 48; ++i) { // 20-48 are Yellow line stations in order
        graph[i].push_back({i+1, 3});
        graph[i+1].push_back({i, 3});
    }
    // Interchanges
    graph[25].push_back({74, 2}); graph[74].push_back({25, 2}); // Azadpur (Yellow/Pink)
    graph[31].push_back({62, 2}); graph[62].push_back({31, 2}); // Kashmere Gate (Yellow/Red)
    graph[33].push_back({10, 2}); graph[10].push_back({33, 2}); // Rajiv Chowk (Yellow/Blue)
    graph[35].push_back({84, 2}); graph[84].push_back({35, 2}); // INA (Yellow/Pink)

    // --- RED LINE CONNECTIONS ---
    for (int i = 49; i < 72; ++i) { // 49-72 are Red line stations in order
        graph[i].push_back({i+1, 3});
        graph[i+1].push_back({i, 3});
    }
    // Interchanges
    graph[54].push_back({76, 2}); graph[76].push_back({54, 2}); // Netaji Subhash Place (Red/Pink)
    graph[57].push_back({31, 2}); graph[31].push_back({57, 2}); // Inderlok (Red/Green, connect to Kashmere Gate for Yellow)
    graph[62].push_back({31, 2}); graph[31].push_back({62, 2}); // Kashmere Gate (Red/Yellow)
    graph[65].push_back({84, 2}); graph[84].push_back({65, 2}); // Welcome (Red/Pink)

    // --- PINK LINE CONNECTIONS ---
    for (int i = 73; i < 95; ++i) { // 73-95 are Pink line stations in order
        graph[i].push_back({i+1, 3});
        graph[i+1].push_back({i, 3});
    }
    // Interchanges
    graph[74].push_back({25, 2}); graph[25].push_back({74, 2}); // Azadpur (Pink/Yellow)
    graph[76].push_back({54, 2}); graph[54].push_back({76, 2}); // Netaji Subhash Place (Pink/Red)
    graph[79].push_back({8, 2}); graph[8].push_back({79, 2});   // Rajouri Garden (Pink/Blue)
    graph[84].push_back({35, 2}); graph[35].push_back({84, 2}); // Lajpat Nagar (Pink/Yellow)
    graph[85].push_back({14, 2}); graph[14].push_back({85, 2}); // Mayur Vihar Phase-1 (Pink/Blue)
    graph[86].push_back({19, 2}); graph[19].push_back({86, 2}); // Anand Vihar (Pink/Blue)
    graph[65].push_back({84, 2}); graph[84].push_back({65, 2}); // Welcome (Pink/Red)

    // Ensure all stations are connected to at least one other station
    // (already handled by above loops and interchanges)

    // Print available stations
    cout << "Delhi Metro Stations:" << endl;
    for (int i = 0; i < stations.size(); i++) {
        cout << i << ": " << stations[i].name << " (" << stations[i].line << ")" << endl;
    }

    // Ask user if they want to visualise the network
    char vis_choice;
    cout << "\nWould you like to visualise the metro network? (y/n): ";
    cin >> vis_choice;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    if (vis_choice == 'y' || vis_choice == 'Y') {
        visualiseMetroNetwork(stations, graph);
    }

    while (true) {
        int start, end;
        cout << "\nEnter start station number (0 to exit): ";
        cin >> start;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a valid station number." << endl;
            continue;
        }
        if (start == 0) {
            cout << "Exiting Metro Route Optimisation.\n";
            break;
        }
        if (start < 0 || start >= stations.size()) {
            cout << "Invalid start station number. Please try again." << endl;
            continue;
        }
        cout << "Enter destination station number: ";
        cin >> end;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a valid station number." << endl;
            continue;
        }
        if (end < 0 || end >= stations.size()) {
            cout << "Invalid destination station number. Please try again." << endl;
            continue;
        }
        if (start == end) {
            cout << "Start and destination stations are the same. Please enter different stations." << endl;
            continue;
        }

        vector<int> distances, previous;
        dijkstra(start, graph, distances, previous);

        if (distances[end] == INT_MAX) {
            cout << "No path found." << endl;
        } else {
            cout << "Shortest time: " << distances[end] << " minutes" << endl;
            vector<int> path = reconstructPath(start, end, previous);

            // Prepare lines info for each station in the path
            vector<vector<string>> path_lines;
            for (int idx : path) {
                path_lines.push_back(splitLines(stations[idx].line));
            }

            cout << "Path: ";
            for (int i = 0; i < path.size(); i++) {
                int station = path[i];
                cout << stations[station].name << " [";

                // Show lines for this station
                for (size_t l = 0; l < path_lines[i].size(); ++l) {
                    cout << path_lines[i][l];
                    if (l < path_lines[i].size() - 1) cout << "/";
                }
                cout << "]";

                if (i < path.size() - 1) {
                    cout << " -> ";
                }
            }
            cout << endl;
        }
    }

    return 0;
}