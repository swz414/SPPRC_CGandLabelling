#pragma once
#include "comm.h"

// 节点结构体
struct Node
{
    int index;
    float_t corX;
    float_t corY;
    int demand;
    float_t readyTime;
    float_t dueTime;
    float_t serviceTime;
} typedef Node;

// 边结构体
struct Edge
{
    int from;
    int to;
    float_t length;
    float_t travelTime;
    float_t cost;
} typedef Edge;

// 路径结构体
struct Route
{
    vector<int> path;
    vector<float_t> arriveTime;
    float_t totalDis;
    int load;
} typedef Route;

class Graph {
private:
    map<int, Node> nodes;
    map<int, map<int, Edge>> edges;
    map<int, Route> Routes;

    map<int, set<int>> previous;
    map<int, set<int>> successors;

    int nodesNum;
    int edgesNum;
    int routesNum;
    int vehicleNum;
    int capacity;

    bool genInitSol_normal(int startIndex, int endIndex);
    bool genInitSol_NearestNeighbor(int startIndex, int endIndex);

    bool getInsertPos(vector<Node>& customers, Route& route, int* insertPos, int* seq);

public:
    Graph(int _vehicleNum, int _capacity) {
        nodesNum = 0; edgesNum = 0; routesNum = 0; vehicleNum = _vehicleNum; capacity = _capacity;
    };

    void addNode(int index, float_t corX, float_t corY, int demand, float_t readyTime, float_t dueTime, float_t serviceTime);
    Node* getNode(int index);
    vector<Node*> getNodes();
    int getNodesNum();

    void addEdge(int from, int to, float_t length, float_t travelTime, float_t cost);
    Edge* getEdge(int from, int to);
    vector<Edge*> getEdges(int from);
    vector<Edge*> getAllEdges();
    int getEdgesNum();

    int addRoute(Route route);
    Route* getRoute(int index);
    bool insertNodeToRoute(Route* proute, const Node* pnode, int pos);
    bool clearRoutes();
    int getRoutesNum();

    vector<int> getSuccessors(int from);
    vector<int> getPrevious(int to);
    int getVehicleNum();
    int getCapacity();

    vector<float_t> calRoutesLength(const vector<vector<int>>& routes);
    bool genInitSol(int type, int startIndex, int endIndex);

    bool updateCost(vector<float_t>& pi);
};