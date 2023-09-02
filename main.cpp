#include "Data.h"
#include "Graph.h"

int main()
{
	bool ret = false;

	// Step 1: ��ȡ����
	string filename = "c101.txt";
	//string filename = "C1_6_1.txt";
	Data data;
	ret = data.ReadData(filename);
	if (!ret) {
		db_print(DB_ERROR, "��ȡ����ʧ�ܣ�\n");
		return 0;
	}
	db_print(DB_NORMAL, "vehicleNum: %d, capicity: %d, custNum: %d\n", data.vehicleNum, data.capicity, data.customers.size());

	// Step 2: ��������ͼ
	Graph graph;

	// ���ӽڵ�
	for (int i = 0; i < data.customers.size(); ++i)
	{
		graph.addNode(data.customers[i].custNo, data.customers[i].posX, data.customers[i].posY, data.customers[i].demand,
			data.customers[i].readyTime, data.customers[i].dueTime, data.customers[i].servTime);
	}
	db_print(DB_NORMAL, "nodelen: %d, %d, %f, %f\n", graph.getNodes().size(), graph.getNode(0)->index, graph.getNode(0)->corX, graph.getNode(0)->corY);

	// ���ӱ�
	for (int i = 0; i < data.customers.size(); ++i)
	{
		for (int j = 0; j < data.customers.size(); ++j)
		{
			if (i == j || (i == 0 && j == data.customers.size() - 1) || (j == 0 && i == data.customers.size() - 1))
				continue;
			graph.addEdge(i, j, data.disMat[i][j], data.disMat[i][j]);
		}
	}
	db_print(DB_NORMAL, "edgelen: %d, size: %d, %d, %d, %f\n", graph.getAllEdges().size(), graph.getEdges(0).size(), graph.getEdge(0, 1)->from, graph.getEdge(0, 1)->to, graph.getEdge(0, 1)->length);
	db_print(DB_NORMAL, "the number of successors for 0: %d\n", graph.getSuccessors(0).size());
	db_print(DB_NORMAL, "the number of previous for 0: %d\n", graph.getPrevious(0).size());


}