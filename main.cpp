// Created by: me - idk
#include <iostream>
#include <vector>
#include <stack>
#include <list>
#include <algorithm>
#include <limits>
#include <cstdlib>
#include <ctime>
#include <unistd.h> // for getopt
#include "graph.hpp"

using namespace std;


Graph::Graph(int V) {
    this->V = V;
    adj = new list<int>[V];
}

Graph::~Graph() {
    delete[] adj;
}

void Graph::addEdge(int v, int w) {
    adj[v].push_back(w);
    adj[w].push_back(v); // Since the graph is undirected
}

void Graph::DFSUtil(int v, vector<bool> &visited) {
    visited[v] = true;
    // cout << v + 1 << " "; // Print vertex, add 1 because vertices are represented from 1 to n
    for (int i : adj[v])
        if (!visited[i])
            DFSUtil(i, visited);
}

// void Graph::fillOrder(int v, vector<bool> &visited, stack<int> &Stack) {
//     visited[v] = true;
//     for (int i : adj[v])
//         if (!visited[i])
//             fillOrder(i, visited, Stack);
//     Stack.push(v);
// }

Graph Graph::getTranspose() {
    Graph g(V);
    for (int v = 0; v < V; v++) {
        for (int i : adj[v]) {
            g.adj[i].push_back(v);
        }
    }
    return g;
}

// void Graph::printSCCs() {
//     stack<int> Stack;
//     vector<bool> visited(V, false);

//     for (int i = 0; i < V; i++)
//         if (!visited[i])
//             fillOrder(i, visited, Stack);

//     Graph gr = getTranspose();
//     fill(visited.begin(), visited.end(), false);

//     while (!Stack.empty()) {
//         int v = Stack.top();
//         Stack.pop();

//         if (!visited[v]) {
//             gr.DFSUtil(v, visited);
//             cout << endl;
//         }
//     }
// }

bool Graph::hasEulerianCircuit() {
    if (!isConnected()) {
        return false;
    }
    for (int i = 0; i < V; i++) {
        if (adj[i].size() % 2 != 0) {
            return false;
        }
    }
    return true;
}

void Graph::findEulerianCircuit() {
    if (!hasEulerianCircuit()) {
        cout << "Graph does not have an Eulerian Circuit" << endl;
        return;
    }
    stack<int> curr_path;
    vector<int> circuit;
    curr_path.push(0);
    int curr_v = 0;
    while (!curr_path.empty()) {
        if (adj[curr_v].size()) {
            curr_path.push(curr_v);
            int next_v = adj[curr_v].front();
            adj[curr_v].remove(next_v);
            adj[next_v].remove(curr_v);
            curr_v = next_v;
        } else {
            circuit.push_back(curr_v);
            curr_v = curr_path.top();
            curr_path.pop();
        }
    }
    for (int i = circuit.size() - 1; i >= 0; i--) {
        cout << circuit[i];
        if (i) {
            cout << " -> ";
        }
    }
    cout << endl;
}

bool Graph::isConnected() {
    vector<bool> visited(V, false);
    int i;
    for (i = 0; i < V; i++) {
        if (adj[i].size() != 0) {
            break;
        }
    }
    if (i == V) {
        return true;
    }
    DFSUtil(i, visited);
    for (i = 0; i < V; i++) {
        if (visited[i] == false && adj[i].size() > 0) {
            return false;
        }
    }
    return true;
}

void print_usage() {
    cout << "Usage: ./OS_EX4 -v <number_of_vertices> -e <number_of_edges> -s <random_seed>" << endl;
}

int main(int argc, char *argv[]) {
    int n = 0, m = 0, seed = time(0);
    int opt;

    while ((opt = getopt(argc, argv, "v:e:s:")) != -1) {
        switch (opt) {
            case 'v':
                n = atoi(optarg);
                break;
            case 'e':
                m = atoi(optarg);
                break;
            case 's':
                seed = atoi(optarg);
                break;
            default:
                print_usage();
                return 1;
        }
    }

    if (n <= 0 || m < 0) {
        print_usage();
        return 1;
    }

    srand(seed);

    Graph g(n);

    for (int i = 0; i < m; i++) {
        int u = rand() % n;
        int v = rand() % n;
        g.addEdge(u, v);
    }

    cout << "Eulerian Circuit in the generated graph:\n";
    g.findEulerianCircuit();

    return 0;
}