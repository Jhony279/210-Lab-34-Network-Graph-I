#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <string>
#include <map>

using namespace std;

const int SIZE = 13; // Number of UWB anchor nodes in the mapping grid

// Structure representing a physical path between two UWB anchors
struct Path {
    int src, dest, distance;
};

typedef pair<int, int> Pair;

class AutonomousNavGraph {
public:
    vector<vector<Pair>> adjList;
    map<int, string> locationNames;

    AutonomousNavGraph(vector<Path> const &paths) {
        adjList.resize(SIZE);
        
        // Add edges (navigable physical routes) to the graph
        for (auto &path: paths) {
            int src = path.src;
            int dest = path.dest;
            int distance = path.distance;

            adjList[src].push_back(make_pair(dest, distance));
            adjList[dest].push_back(make_pair(src, distance));
        }

        // Mapping vertex IDs to real-world locations in the environment
        locationNames[0] = "Charging Base";
        locationNames[2] = "Living Room Center";
        locationNames[4] = "Kitchen Entry";
        locationNames[5] = "Feeding Station";
        locationNames[6] = "Lounge Area";
        locationNames[7] = "Hallway North";
        locationNames[8] = "Bedroom Door";
        locationNames[9] = "Under-Bed Zone";
        locationNames[10] = "Walk-in Closet";
        locationNames[11] = "Hallway South";
        locationNames[12] = "Patio Door";
        // Note: Nodes 1 and 3 are offline or removed from the system.
    }

    // Depth First Search: Useful for deep mapping/exploration of dead-end rooms
    void exploreAreaDFS(int startVertex) {
        vector<bool> visited(SIZE, false);
        stack<int> s;

        s.push(startVertex);

        cout << "\n[Exploration Mode] DFS route starting from " << locationNames[startVertex] << ":" << endl;

        while (!s.empty()) {
            int curr = s.top();
            s.pop();

            if (!visited[curr] && locationNames.count(curr)) {
                cout << " -> " << locationNames[curr] << " (ID: " << curr << ")" << endl;
                visited[curr] = true;

                for (Pair neighbor : adjList[curr]) {
                    if (!visited[neighbor.first]) {
                        s.push(neighbor.first);
                    }
                }
            }
        }
    }

    // Breadth First Search: Useful for finding the path with the fewest anchor hops
    void shortestHopsBFS(int startVertex) {
        vector<bool> visited(SIZE, false);
        queue<int> q;

        visited[startVertex] = true;
        q.push(startVertex);

        cout << "\n[Fewest Hops Routing] BFS broadcast starting from " << locationNames[startVertex] << ":" << endl;

        while (!q.empty()) {
            int curr = q.front();
            q.pop();
            
            if(locationNames.count(curr)) {
                cout << " -> Checked " << locationNames[curr] << " (ID: " << curr << ")" << endl;
            }

            for (Pair neighbor : adjList[curr]) {
                if (!visited[neighbor.first]) {
                    visited[neighbor.first] = true;
                    q.push(neighbor.first);
                }
            }
        }
    }

    void printNetwork() {
        cout << "\n--- UWB Anchor Navigational Network ---" << endl;
        for (int i = 0; i < adjList.size(); i++) {
            if (adjList[i].empty()) continue; // Skip offline nodes

            cout << "[" << locationNames[i] << "] connects to:" << endl;
            for (Pair v : adjList[i]) {
                cout << "    - " << locationNames[v.first] << " (Distance: " << v.second << " dm)" << endl;
            }
        }
    }
};

int main() {
    // Navigable paths between UWB anchors and their physical distances
    vector<Path> paths = {
        {0, 2, 8}, {2, 6, 2}, {5, 6, 6}, {4, 5, 9}, {2, 4, 4}, {2, 5, 5}, 
        {0, 7, 4}, {7, 8, 6}, {8, 9, 11}, {9, 10, 2}, {10, 11, 8}, {11, 12, 14}, 
        {12, 6, 3}, {8, 4, 1}
    };

    // Initialize the navigation system
    AutonomousNavGraph navSystem(paths);

    // Display the network layout
    navSystem.printNetwork();
    
    // Run the routing simulations
    navSystem.exploreAreaDFS(0);
    navSystem.shortestHopsBFS(0);

    return 0;
}