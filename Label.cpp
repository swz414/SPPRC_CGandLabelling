#include "Label.h"
#include<stdio.h>
#include<windows.h>

float_t calCost(Graph& graph, vector<int>& path)
{
    float_t totalCost = 0;
    for (int i = 1; i < path.size(); ++i)
    {
        totalCost += graph.getEdge(path[i - 1], path[i])->cost;
    }
    return totalCost;
}

void dominate(vector<Label>& Queue, map<int, Label>& Paths, int des)
{
    vector<Label> QueueCopy(Queue.begin(), Queue.end());
    map<int, Label> PathsCopy(Paths);

    vector<int> delIndexs;
    delIndexs.clear();
    for (Label& label : QueueCopy)
    {
        for (int i = 0; i < Queue.size(); ++i)
        {
            if (label.path.back() == Queue[i].path.back()
                && label.path.back() != des
                //&& label.time < Queue[i].time 
                //&& label.dis < Queue[i].dis
                && label.cost < Queue[i].cost
                && count(delIndexs.begin(), delIndexs.end(), i) == 0)
            {
                delIndexs.push_back(i);
            }
        }
    }
    sort(delIndexs.rbegin(), delIndexs.rend());

    for (int& index : delIndexs)
    {
        if (index < Queue.size())
            Queue.erase(Queue.begin() + index);
        else
            db_print(DB_ERROR, "ÏÂ±ê´íÎó index: %d, Size: %d\n", index, Queue.size());
    }

    for (auto& ul1 : PathsCopy)
    {
        int key1 = ul1.first;
        for (auto& ul2 : PathsCopy)
        {
            int key2 = ul2.first;

            if (PathsCopy[key1].path.back() == PathsCopy[key2].path.back()
                && PathsCopy[key1].path.back() != des
                //&& PathsCopy[key1].time < PathsCopy[key2].time
                //&& PathsCopy[key1].dis < PathsCopy[key2].dis
                && PathsCopy[key1].cost < PathsCopy[key2].cost
                && Paths.count(key2) > 0)
            {
                Paths.erase(key2);
            }
        }
    }
}

Label LabellingAlg::labelling_SPPRC(Graph graph, int org, int des, int maxCapacity)
{
    vector<Label> Queue;

    Label label;
    label.path.push_back(org);
    label.arriveTime.push_back(graph.getNode(org)->readyTime + graph.getNode(org)->serviceTime);
    label.load += graph.getNode(org)->demand;
    Queue.push_back(label);

    map<int, Label> Paths;
    int cnt = 0;
    while (Queue.size() > 0)
    {
        //db_print(DB_NORMAL, "Queue: %d\n", Queue.size());
        cnt += 1;

        Label curLabel = Queue[0];

        Queue.erase(Queue.begin());

        int lastNode = curLabel.path.back();

        for (int& child : graph.getSuccessors(lastNode))
        {
            Label extendedPath = curLabel;
            int arcFrom = lastNode;
            int arcTo = child;

            Edge* curEdge = graph.getEdge(arcFrom, arcTo);
            Node* curNode = graph.getNode(child);

            if (0 == count(extendedPath.path.begin(), extendedPath.path.end(), child)
                && curNode->dueTime >= extendedPath.arriveTime.back()
                && extendedPath.load + curNode->demand <= maxCapacity)
            {
                extendedPath.path.push_back(child);
                extendedPath.arriveTime.push_back(max(curNode->readyTime, extendedPath.arriveTime.back() + curEdge->travelTime) + curNode->serviceTime);
                extendedPath.dis += curEdge->length;
                extendedPath.time += curEdge->travelTime;
                extendedPath.load += curNode->demand;
                extendedPath.cost = calCost(graph, extendedPath.path);

                Queue.push_back(extendedPath);
            }
        }

        Paths[cnt] = curLabel;

        dominate(Queue, Paths, des);
    }

    map<int, Label> PathsCopy(Paths);
    for (auto& ul : PathsCopy)
    {
        int key = ul.first;

        if (ul.second.path.back() != des || ul.second.path.size() <= 3)
        {
            Paths.erase(key);
            continue;
        }

        for (int i = 0; i < graph.getRoutesNum(); ++i)
        {
            if (graph.getRoute(i)->path.size() == ul.second.path.size())
            {
                int j = 0;
                for (j = 0; j < ul.second.path.size(); ++j)
                {
                    if (graph.getRoute(i)->path[j] != ul.second.path[j])
                        break;
                }
                if (j == ul.second.path.size())
                {
                    Paths.erase(key);
                    break;
                }
            }
        }
    }

    int minKey = -1;
    float_t minCost = 10000000;
    for (auto& ul : Paths)
    {
        int key = ul.first;
        if (ul.second.cost < minCost)
        {
            minCost = ul.second.cost;
            minKey = key;
        }
    }

    Label optPath;
    optPath.cost = 1e6;
    if (minKey >= 0)
    {
        optPath = Paths[minKey];
    }

    return optPath;
}


