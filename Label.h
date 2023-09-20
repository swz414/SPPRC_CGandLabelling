#pragma once
#include "comm.h"
#include "Graph.h"

class Label {

public:
    vector<int> path;
    vector<float_t> arriveTime;
    float_t time;
    float_t dis;
    float_t cost;
    int load;

    Label() {
        time = 0; dis = 0; cost = 0; load = 0;
    };
    Label(const Label& label) {
        path.assign(label.path.begin(), label.path.end()); arriveTime.assign(label.arriveTime.begin(), label.arriveTime.end());
        time = label.time; dis = label.dis; cost = label.cost; load = label.load;
    };
};

class LabellingAlg {
public:
    Label labelling_SPPRC(Graph graph, int org, int des, int maxCapacity);
};