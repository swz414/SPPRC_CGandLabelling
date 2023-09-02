#include "ColumnGeneration.h"
#include <gurobi_c++.h>

bool genInitSol_normal(vector<vector<int>>& rstVec, Graph graph, int startIndex, int endIndex)
{
	int nodesNum = graph.getNodesNum();

	for (int i = 0; i < nodesNum; ++i)
	{
		if (i == startIndex || i == endIndex)
			continue;

		vector<int> tmpVec;
		tmpVec.push_back(startIndex);
		tmpVec.push_back(i);
		tmpVec.push_back(endIndex);

		rstVec.push_back(tmpVec);
	}

	return true;
}

vector<vector<int>> ColumnGeneration::genInitSol(int type, const Graph& graph, int startIndex, int endIndex)
{
	vector<vector<int>> retVec;
	switch (type)
	{
	case 0:
		genInitSol_normal(retVec, graph, startIndex, endIndex);
		break;
	default:
		break;
	}

	return retVec;
}

void ColumnGeneration::CGandLabelling()
{
	try {
		// Create an environment
		GRBEnv env = GRBEnv(true);
		env.set(GRB_IntParam_OutputFlag, 0);
		env.start();
		// Create an empty model
		GRBModel model = GRBModel(env);
		
		// Create variables
		GRBVar x = model.addVar(0.0, 1.0, 0.0, GRB_BINARY, "x");
		GRBVar y = model.addVar(0.0, 1.0, 0.0, GRB_BINARY, "y");
		GRBVar z = model.addVar(0.0, 1.0, 0.0, GRB_BINARY, "z");
		// Set objective: maximize x + y + 2 z
		model.setObjective(x + y + 2 * z, GRB_MAXIMIZE);
		// Add constraint: x + 2 y + 3 z <= 4
		model.addConstr(x + 2 * y + 3 * z <= 4, "c0");
		// Add constraint: x + y >= 1
		model.addConstr(x + y >= 1, "c1");

		// Optimize model
		model.optimize();
		cout << x.get(GRB_StringAttr_VarName) << " "
			<< x.get(GRB_DoubleAttr_X) << endl;
		cout << y.get(GRB_StringAttr_VarName) << " "
			<< y.get(GRB_DoubleAttr_X) << endl;
		cout << z.get(GRB_StringAttr_VarName) << " "
			<< z.get(GRB_DoubleAttr_X) << endl;
		cout << "Obj: " << model.get(GRB_DoubleAttr_ObjVal) << endl;
	}
	catch (GRBException e) {
		cout << "Error code = " << e.getErrorCode() << endl;
		cout << e.getMessage() << endl;
	}
	catch (...) {
		cout << "Exception during optimization" << endl;
	}
}
