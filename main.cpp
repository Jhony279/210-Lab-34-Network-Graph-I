#include <iostream>
#include <vector>
#include <queue>
#include <stack>

using namespace std;

const int SIZE = 7;

struct Edge {
    int src, dest, weight;
};

typedef pair<int, int> Pair;

class Graph {
public:
    vector<vector<Pair>> adjList;

    Graph(vector<Edge> const &edges) {
        adjList.resize(SIZE);
        for (auto &edge: edges) {
            int src = edge.src;
            int dest = edge.dest;
            int weight = edge.weight;

            adjList[src].push_back(make_pair(dest, weight));
            adjList[dest].push_back(make_pair(src, weight));
        }
    }

    // Depth First Search using a Stack
    void DFS(int startVertex) {
        vector<bool> visited(SIZE, false);
        stack<int> s;

        s.push(startVertex);

        cout << "DFS starting from vertex " << startVertex << ":" << endl;

        while (!s.empty()) {
            int curr = s.top();
            s.pop();

            // Only print and process if the node hasn't been visited yet
            if (!visited[curr]) {
                cout << curr << " ";
                visited[curr] = true;

                // Push neighbors to stack in standard forward order. 
                // This puts the last connected node on top of the stack.
                for (Pair neighbor : adjList[curr]) {
                    if (!visited[neighbor.first]) {
                        s.push(neighbor.first);
                    }
                }
            }
        }
        cout << endl;
    }

    // Breadth First Search using a Queue
    void BFS(int startVertex) {
        vector<bool> visited(SIZE, false);
        queue<int> q;

        visited[startVertex] = true;
        q.push(startVertex);

        cout << "BFS starting from vertex " << startVertex << ":" << endl;

        while (!q.empty()) {
            int curr = q.front();
            q.pop();
            cout << curr << " ";

            // Push neighbors to the back of the queue
            for (Pair neighbor : adjList[curr]) {
                if (!visited[neighbor.first]) {
                    visited[neighbor.first] = true;
                    q.push(neighbor.first);
                }
            }
        }
        cout << endl;
    }

    void printGraph() {
        cout << "\nGraph's adjacency list:" << endl;
        for (int i = 0; i < adjList.size(); i++) {
            cout << i << " --> ";
            for (Pair v : adjList[i])
                cout << "(" << v.first << ", " << v.second << ") ";
            cout << endl;
        }
    }
};

int main() {
    vector<Edge> edges = {
        {0,2,8},{2,6,2},{5,6,6},{4,5,9},{2,4,4},{2,5,5}
    };

    Graph graph(edges);

    graph.printGraph();
    
    // Executes the traversal algorithms
    graph.DFS(0);
    graph.BFS(0);

    return 0;
}