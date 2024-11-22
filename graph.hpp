
//elichaiza@gmail.com
//id:318691821
#include <iostream>
#include <vector>
#include <stack>
#include <list>
#include <algorithm>

using namespace std;

class Graph {
    int V; // מספר הקודקודים
    list<int> *adj; // רשימת שכנות

    void fillOrder(int v, vector<bool> &visited, stack<int> &Stack);
    void DFSUtil(int v, vector<bool> &visited);

public:
    Graph(int V);
    ~Graph();
    void addEdge(int v, int w);
    void printSCCs();
    Graph getTranspose();
    bool isConnected();
    bool hasEulerianCircuit();
    void findEulerianCircuit();
};
