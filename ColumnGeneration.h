#pragma once
#include "comm.h"
#include "Graph.h"

class ColumnGeneration {
public:
	vector<vector<int>> genInitSol(int type, const Graph& data, int startIndex, int endIndex);
	void CGandLabelling();
};