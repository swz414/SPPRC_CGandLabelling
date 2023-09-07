#include "ColumnGeneration.h"
#include <gurobi_c++.h>

void ColumnGeneration::CGandLabelling(Graph& graph)
{
    int vehicleNum = graph.getVehicleNum(); // 车辆数
    int capacity = graph.getCapacity(); // 容量
    int nodesNum = graph.getNodesNum(); // 节点数

    bool ret = graph.genInitSol(1, 0, nodesNum - 1);    // 获取初始路径
    int initRoutesNum = graph.getRoutesNum();

    try {
        // Create an environment
        GRBEnv env = GRBEnv(true);
        env.set(GRB_IntParam_OutputFlag, 0);
        env.start();
        // Create an empty model
        GRBModel model = GRBModel(env);
        
        // Create variables
        GRBVar *x = model.addVars(initRoutesNum, GRB_CONTINUOUS);

        // Set objective: min sum(c * x)
        GRBLinExpr objExpr = 0;
        for (int i = 0; i < initRoutesNum; ++i) {
            objExpr += graph.getRoute(i)->totalDis * *(x + i);
        }
        model.setObjective(objExpr, GRB_MINIMIZE);

        // Add constraint: sum(a * x) == 1
        for (int i = 1; i < nodesNum - 1; ++i)
        {
            GRBLinExpr lhsExpr = 0;
            for (int j = 0; j < initRoutesNum; ++j)
            {
                for (int& node : graph.getRoute(j)->path)
                {
                    if (node == i)
                    {
                        lhsExpr += *(x + j);
                    } 
                }
            }
            model.addConstr(lhsExpr == 1, "cons_" + to_string(i));
        }

        model.update();
        model.optimize();
        model.write("E:/桌面/test.lp");
        
        for (int i = 0; i < initRoutesNum; ++i)
            cout << (x+i)->get(GRB_StringAttr_VarName) << " "
                << (x + i)->get(GRB_DoubleAttr_X) << endl;
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
