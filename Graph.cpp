#include "Graph.h"

bool Graph::getInsertPos(vector<Node>& candidateNodes, Route& route, int* insertPos, int* seq)
{
	bool ret = false;
	vector<int> ids;

	if (route.path.size() < 2)
	{
		goto END;
	}

	for (Node& node : candidateNodes)
	{
		ids.push_back(node.index);
	}

	// 若线路中只有起点和终点，则添加节点时选择距离起点最远的节点
	if (route.path.size() == 2)
	{
		int startNode = route.path.front();
		int endNode = route.path.back();
		vector<Edge*> tmpEdges = getEdges(startNode);
		float_t maxDis = 0;
		Edge* maxDisEdge = NULL;
		vector<int>::iterator tmpSeq, maxSeq;
		Node* tmpNode = NULL;

		for (Edge* tmpEdge : tmpEdges)
		{
			tmpSeq = find(ids.begin(), ids.end(), tmpEdge->to);
			tmpNode = getNode(tmpEdge->to);

			if (tmpSeq != ids.end()
				&& tmpEdge->length > maxDis 
				&& tmpNode->dueTime >= getNode(startNode)->readyTime + getNode(startNode)->serviceTime
				&& max(tmpNode->readyTime, getNode(startNode)->readyTime + getNode(startNode)->serviceTime) + tmpNode->serviceTime <= getNode(endNode)->dueTime)
			{
				maxDis = tmpEdge->length;
				maxDisEdge = tmpEdge;
				maxSeq = tmpSeq;
			}
		}

		if (maxDisEdge != NULL)
		{
			*insertPos = 1;
			*seq = maxSeq - ids.begin();
			ret = true;
		}
	}
	else 
	{
		int endNode = route.path.back();
		int lastNode = *(route.path.end() - 2);
		vector<Edge*> tmpEdges = getEdges(lastNode);
		float_t minDis = 1000000;
		Edge* minDisEdge = NULL;
		vector<int>::iterator tmpSeq, minSeq;
		Node* tmpNode = NULL;

		for (Edge* tmpEdge : tmpEdges)
		{
			tmpSeq = find(ids.begin(), ids.end(), tmpEdge->to);
			tmpNode = getNode(tmpEdge->to);

			if (tmpSeq != ids.end()
				&& tmpEdge->length < minDis 
				&& tmpNode->dueTime >= *(route.arriveTime.end() - 2) + getNode(lastNode)->serviceTime
				&& max(tmpNode->readyTime, *(route.arriveTime.end() - 2) + getNode(lastNode)->serviceTime) + tmpNode->serviceTime <= getNode(endNode)->dueTime)
			{
				lastNode = tmpEdge->length;
				minDisEdge = tmpEdge;
				minSeq = tmpSeq;
			}
		}

		if (minDisEdge != NULL)
		{
			*insertPos = route.path.size() - 1;
			*seq = minSeq - ids.begin();
			ret = true;
		}
	}

END:
	return ret;
}

void Graph::addNode(int index, float_t corX, float_t corY, int demand, float_t readyTime, float_t dueTime, float_t serviceTime)
{
	nodes.emplace(index, Node{ index, corX, corY, demand, readyTime, dueTime, serviceTime });
	nodesNum += 1;
}

void Graph::addEdge(int from, int to, float_t length, float_t travelTime)
{
	edges[from][to] = Edge{ from, to, length, travelTime };
	edgesNum += 1;

	previous[to].insert(from);
	successors[from].insert(to);
}

Node* Graph::getNode(int index)
{
	return &nodes[index];
}

vector<Node*> Graph::getNodes()
{
	vector<Node*> rstVec;
	transform(
		nodes.begin(),
		nodes.end(),
		std::back_inserter(rstVec), // 需要使用back_inserter，它会自动为vector分配空间
		[](auto& kv) { return &(kv.second); }
	);
	return rstVec;
}

int Graph::getNodesNum()
{
	return nodesNum;
}

Edge* Graph::getEdge(int from, int to)
{
	return &edges[from][to];
}

vector<Edge*> Graph::getEdges(int from)
{
	vector<Edge*> rstVec;
	transform(
		edges[from].begin(),
		edges[from].end(),
		std::back_inserter(rstVec), // 需要使用back_inserter，它会自动为vector分配空间
		[](auto& kv) { return &(kv.second); }
	);

	return rstVec;
}

vector<Edge*> Graph::getAllEdges()
{
	vector<Edge*> retVec, tmpVec;
	for (const auto& kv : edges)
	{
		tmpVec = getEdges(kv.first);
		retVec.assign(tmpVec.begin(), tmpVec.end());
	}
	return retVec;
}

int Graph::getEdgesNum()
{
	return edgesNum;
}

int Graph::addRoute(Route route)
{
	Routes[routesNum] = route;
	routesNum += 1;

	return routesNum - 1;
}

Route* Graph::getRoute(int index)
{
	if(index < routesNum)
		return &Routes[index];
	return NULL;
}

bool Graph::insertNodeToRoute(Route* proute, const Node* pnode, int pos)
{
	bool rst = false;

	if (NULL == proute || NULL == pnode || pos < 0 || pos > proute->path.size())
	{
		goto END;
	}

	// 注意：向path中添加节点必须放在更新load、dis、arrivetime之后，否则会出错！
	if (pos == 0) // 添加节点到线路首部
	{
		// 原路线中有节点
		if (proute->path.size() > 0)
		{
			proute->load += pnode->demand;
			proute->totalDis += getEdge(pnode->index, proute->path.front())->length;
			proute->arriveTime.insert(proute->arriveTime.begin(), pnode->readyTime);
			proute->path.insert(proute->path.begin(), pnode->index);
			// 更新arriveTime
			for (int i = 1; i < proute->arriveTime.size(); ++i) {
				proute->arriveTime[i] = max(proute->arriveTime[i - 1] + getNode(proute->path[i - 1])->serviceTime, getNode(proute->path[i])->readyTime);
			}
		}
		else // 原路线中没有节点
		{
			proute->load = pnode->demand;
			proute->totalDis = 0;
			proute->arriveTime.push_back(pnode->readyTime);
			proute->path.push_back(pnode->index);
		}
	}
	else if (pos == proute->path.size()) //添加节点到线路尾部
	{
		proute->load += pnode->demand;
		proute->totalDis += getEdge(proute->path.back(), pnode->index)->length;
		proute->arriveTime.push_back(max(pnode->readyTime, proute->arriveTime.back()+getNode(proute->path.back())->serviceTime));
		proute->path.push_back(pnode->index);
	}
	else
	{
		proute->load += pnode->demand;
		proute->totalDis += getEdge(proute->path[pos-1], pnode->index)->length + getEdge(pnode->index, proute->path[pos])->length;
		proute->arriveTime.insert(proute->arriveTime.begin() + pos, pnode->readyTime);
		proute->path.insert(proute->path.begin() + pos, pnode->index);
		// 更新arriveTime
		for (int i = pos; i < proute->arriveTime.size(); ++i) {
			proute->arriveTime[i] = max(proute->arriveTime[i - 1] + getNode(proute->path[i - 1])->serviceTime, getNode(proute->path[i])->readyTime);
		}
	}

	rst = true;

END:
	return rst;
}

bool Graph::clearRoutes()
{
	Routes.clear();
	routesNum = 0;

	return true;
}

int Graph::getRoutesNum()
{
	return routesNum;
}

vector<int> Graph::getSuccessors(int from)
{
	return vector<int>(successors[from].begin(), successors[from].end());
}

vector<int> Graph::getPrevious(int to)
{
	return vector<int>(previous[to].begin(), previous[to].end());
}

int Graph::getVehicleNum()
{
	return vehicleNum;
}

int Graph::getCapacity()
{
	return capacity;
}

vector<float_t> Graph::calRoutesLength(const vector<vector<int>>& routes)
{
	vector<float_t> routesLen;
	
	for (int i = 0; i < routes.size(); ++i)
	{
		int prev = -1;
		int succ = -1;
		float_t tmpLen = 0.0;

		for (int j = 0; j < routes[i].size(); ++j)
		{
			if (i == 0) {
				prev = routes[i][j];
				continue;
			}

			succ = routes[i][j];
			tmpLen += edges[prev][succ].length;
			prev = succ;
		}
		
		routesLen.push_back(tmpLen);
		
	}
	return routesLen;
}

bool Graph::genInitSol_normal(int startIndex, int endIndex)
{
	clearRoutes();

	for (int i = 0; i < nodesNum; ++i)
	{
		if (i == startIndex || i == endIndex)
			continue;

		Route tmpVec = {};
		insertNodeToRoute(&tmpVec, getNode(startIndex), tmpVec.path.size());
		insertNodeToRoute(&tmpVec, getNode(i), tmpVec.path.size());
		insertNodeToRoute(&tmpVec, getNode(endIndex), tmpVec.path.size());

		addRoute(tmpVec);
	}

	return true;
}

bool Graph::genInitSol_NearestNeighbor(int startIndex, int endIndex)
{
	// 先清空路线
	clearRoutes();

	// 拷贝一份客户节点
	vector<Node> copyNodes;
	for (int i = 0; i < nodes.size(); ++i)
	{
		Node tmp = nodes[i];
		if (tmp.index == startIndex || tmp.index == endIndex)
			continue;
		copyNodes.push_back(tmp);
	}

	while (copyNodes.size() > 0)
	{
		Route tmpPath = {};
		insertNodeToRoute(&tmpPath, getNode(startIndex), tmpPath.path.size());
		insertNodeToRoute(&tmpPath, getNode(endIndex), tmpPath.path.size());

		while (tmpPath.load < capacity && copyNodes.size() > 0)
		{
			int pos = -1;
			int seq = -1;
			
			// 先获取最佳插入点和客户所在的下标
			if (getInsertPos(copyNodes, tmpPath, &pos, &seq))
			{
				// 若超出容量则退出，否则添加客户到路线中，并从待插入客户表中删除
				if (tmpPath.load + getNode(copyNodes[seq].index)->demand > capacity) {
					break;
				} else {
					insertNodeToRoute(&tmpPath, getNode(copyNodes[seq].index), pos);
					copyNodes.erase(copyNodes.begin() + seq);
				}
			}
			else
			{
				break;
			}
		}

		if (tmpPath.path.size() > 2)
		{
			addRoute(tmpPath);
		}
	}

	return true;
}

bool Graph::genInitSol(int type, int startIndex, int endIndex)
{
	bool ret = false;

	switch (type)
	{
	case 0:
		ret = genInitSol_normal(startIndex, endIndex);
		break;
	case 1:
		ret = genInitSol_NearestNeighbor(startIndex, endIndex);
		break;
	default:
		ret = genInitSol_NearestNeighbor(startIndex, endIndex);
		break;
	}

	return true;
}
