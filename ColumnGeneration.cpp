#include "ColumnGeneration.h"
#include <gurobi_c++.h>

void ColumnGeneration::CGandLabelling(Graph& graph)
{
	//int vehicleNum		= graph.getVehicleNum();	// ������
	//int capacity		= graph.getCapacity();		// ����
	//int nodesNum		= graph.getNodesNum();		// �ڵ���
	////int initRoutesNum	= 
	//vector<vector<int>> initRoutes	= graph.genInitSol(0, 0, nodesNum - 1);	// ��ȡ��ʼ·��
	//int initRoutesNum				= initRoutes.size();					
	//vector<float_t> initRoutesLen	= graph.calRoutesLength(initRoutes);

	//try {
	//	// Create an environment
	//	GRBEnv env = GRBEnv(true);
	//	env.set(GRB_IntParam_OutputFlag, 0);
	//	env.start();
	//	// Create an empty model
	//	GRBModel model = GRBModel(env);
	//	
	//	// Create variables
	//	GRBVar *x = model.addVars(initRoutesNum, GRB_CONTINUOUS);

	//	// Set objective: min sum(c * x)
	//	GRBLinExpr objExpr = 0;
	//	for (int i = 0; i < initRoutesNum; ++i) {
	//		objExpr += initRoutesLen[i] * *(x + i);
	//	}
	//	model.setObjective(objExpr, GRB_MAXIMIZE);

	//	// Add constraint: sum(a * x) == 1
	//	for (int i = 1; i < nodesNum; ++i)
	//	{

	//	}
	//	GRBLinExpr lhsExpr = 0;
	//	model.addConstr(x + 2 * y + 3 * z <= 4, "c0");



	//	// Optimize model
	//	model.optimize();
	//	cout << x.get(GRB_StringAttr_VarName) << " "
	//		<< x.get(GRB_DoubleAttr_X) << endl;
	//	cout << y.get(GRB_StringAttr_VarName) << " "
	//		<< y.get(GRB_DoubleAttr_X) << endl;
	//	cout << z.get(GRB_StringAttr_VarName) << " "
	//		<< z.get(GRB_DoubleAttr_X) << endl;
	//	cout << "Obj: " << model.get(GRB_DoubleAttr_ObjVal) << endl;
	//}
	//catch (GRBException e) {
	//	cout << "Error code = " << e.getErrorCode() << endl;
	//	cout << e.getMessage() << endl;
	//}
	//catch (...) {
	//	cout << "Exception during optimization" << endl;
	//}
}
