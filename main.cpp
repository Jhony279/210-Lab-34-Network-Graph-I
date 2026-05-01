#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <string>
#include <map>
#include <climits>

using namespace std;

// The total number of possible UWB anchor nodes in our mapping grid (Nodes 0 through 12)
const int SIZE = 13; 

// Structure representing a physical, navigable path between two UWB anchors
struct Path {
    int src, dest, distance;
};

// Typedef to make the code cleaner. Pairs represent (Destination Node, Distance)
typedef pair<int, int> Pair;

class AutonomousNavGraph {
public:
    // Adjacency list representation of the graph. 
    // Each index represents a node, and holds a list of (connected node, distance) pairs.
    vector<vector<Pair>> adjList;
    
    // A dictionary (map) to translate raw node IDs (e.g., 0, 2) into human-readable room names
    map<int, string> locationNames;

    // Constructor: Builds the graph when the object is created
    AutonomousNavGraph(vector<Path> const &paths) {
        // Initialize the adjacency list with the correct number of nodes
        adjList.resize(SIZE);
        
        // Loop through the provided physical paths and build the network
        for (auto &path: paths) {
            int src = path.src;
            int dest = path.dest;
            int distance = path.distance;

            // Since this is an undirected graph (the vehicle can travel both ways),
            // we add an edge from Source -> Destination AND Destination -> Source.
            adjList[src].push_back(make_pair(dest, distance));
            adjList[dest].push_back(make_pair(src, distance));
        }

        // Map the integer vertex IDs to real-world locations in the environment
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
        // Note: Nodes 1 and 3 are intentionally left unmapped (simulating offline nodes)
    }

    void exploreAreaDFS(int startVertex) {
        // Keep track of which locations we've already visited to prevent infinite loops
        vector<bool> visited(SIZE, false);
        stack<int> s;

        // Push the starting location onto the stack
        s.push(startVertex);

        cout << "\n[Exploration Mode] DFS route starting from " << locationNames[startVertex] << ":" << endl;

        // Continue exploring as long as there are nodes in the stack
        while (!s.empty()) {
            // Look at the node on top of the stack and remove it
            int curr = s.top();
            s.pop();

            // If we haven't visited this node, and it's a valid mapped location:
            if (!visited[curr] && locationNames.count(curr)) {
                // Mark it as visited and print the location
                cout << " -> " << locationNames[curr] << " (ID: " << curr << ")" << endl;
                visited[curr] = true;

                // Look at all neighboring anchors connected to our current location
                for (Pair neighbor : adjList[curr]) {
                    // If we haven't visited the neighbor, add it to the stack to explore later
                    if (!visited[neighbor.first]) {
                        s.push(neighbor.first);
                    }
                }
            }
        }
    }

    void shortestHopsBFS(int startVertex) {
        // Keep track of visited locations to prevent infinite loops
        vector<bool> visited(SIZE, false);
        queue<int> q;

        // Mark the start location as visited and add it to the back of the queue
        visited[startVertex] = true;
        q.push(startVertex);

        cout << "\n[Fewest Hops Routing] BFS broadcast starting from " << locationNames[startVertex] << ":" << endl;

        // Continue exploring as long as there are nodes in the queue
        while (!q.empty()) {
            // Get the node at the front of the queue and remove it
            int curr = q.front();
            q.pop();
            
            // Print the location being checked
            if(locationNames.count(curr)) {
                cout << " -> Checked " << locationNames[curr] << " (ID: " << curr << ")" << endl;
            }

            // Look at all neighboring anchors connected to our current location
            for (Pair neighbor : adjList[curr]) {
                // If we haven't visited the neighbor, mark it visited immediately and queue it up
                if (!visited[neighbor.first]) {
                    visited[neighbor.first] = true;
                    q.push(neighbor.first);
                }
            }
        }
    }

    // Utility function to print the layout of the graph
    void printNetwork() {
        cout << "--- UWB Anchor Navigational Network ---" << endl;
        // Loop through every node in the graph
        for (int i = 0; i < adjList.size(); i++) {
            // Skip nodes that have no connections (offline/removed anchors)
            if (adjList[i].empty()) continue; 

            cout << "[" << locationNames[i] << "] connects to:" << endl;
            // Loop through and print all the connected neighbors and their distances
            for (Pair v : adjList[i]) {
                cout << "    - " << locationNames[v.first] << " (Distance: " << v.second << " dm)" << endl;
            }
        }
    }

    void findShortestPathsFromSource(int startNode) {
        // Distance array initialized to infinity
        vector<int> dist(SIZE, INT_MAX);
        // Priority queue to store (distance, node ID)
        priority_queue<Pair, vector<Pair>, greater<Pair>> pq;

        dist[startNode] = 0;
        pq.push(make_pair(0, startNode));

        while (!pq.empty()) {
            int d = pq.top().first;
            int u = pq.top().second;
            pq.pop();

            // If we found a longer path already, skip it
            if (d > dist[u]) continue;

            for (auto &edge : adjList[u]) {
                int v = edge.first;
                int weight = edge.second;

                if (dist[u] + weight < dist[v]) {
                    dist[v] = dist[u] + weight;
                    pq.push(make_pair(dist[v], v));
                }
            }
        }

        // Output formatted to match your reference image
        cout << "\nShortest path from node " << startNode << ":" << endl;
        for (int i = 0; i < SIZE; i++) {
            // Only print nodes that are actually part of our location map
            if (locationNames.count(i)) {
                if (dist[i] == INT_MAX) {
                    cout << startNode << " -> " << i << " : INF" << endl;
                } else {
                    cout << startNode << " -> " << i << " : " << dist[i] << endl;
                }
            }
        }
    }
};

int main() {
    // Define the navigable physical paths between UWB anchors and their physical distances (in decimeters)
    vector<Path> paths = {
        {0, 2, 8}, {2, 6, 2}, {5, 6, 6}, {4, 5, 9}, {2, 4, 4}, {2, 5, 5}, 
        {0, 7, 4}, {7, 8, 6}, {8, 9, 11}, {9, 10, 2}, {10, 11, 8}, {11, 12, 14}, 
        {12, 6, 3}, {8, 4, 1}
    };

    // Initialize the navigation system, passing in our physical layout
    AutonomousNavGraph navSystem(paths);

    // Display the network layout and all connections
    navSystem.printNetwork();
    
    // Run the routing simulations to demonstrate DFS and BFS
    navSystem.exploreAreaDFS(0);
    navSystem.shortestHopsBFS(0);

    // Finally, calculate and display the shortest paths from the charging base (node 0) to all other nodes
    navSystem.findShortestPathsFromSource(0);

    return 0;
}