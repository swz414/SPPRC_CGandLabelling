#include "ColumnGeneration.h"
#include <gurobi_c++.h>
#include "Label.h"

void ColumnGeneration::CGandLabelling(Graph& graph, int startIndex, int endIndex)
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
        vector<GRBVar> x;
        for (int i = 0; i < initRoutesNum; ++i)
        {
            x.push_back(model.addVar(0, 1, 1, GRB_CONTINUOUS));
        }

        // Set objective: min sum(c * x)
        GRBLinExpr objExpr = 0;
        for (int i = 0; i < initRoutesNum; ++i) {
            objExpr += graph.getRoute(i)->totalDis * x[i];
        }
        model.setObjective(objExpr, GRB_MINIMIZE);

        vector<GRBConstr> constrs;

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
                        lhsExpr += x[j];
                    } 
                }
            }
            GRBConstr curCons = model.addConstr(lhsExpr == 1, "cons_" + to_string(i));
            constrs.push_back(curCons);
        }

        model.update();
        model.optimize();
        model.write("E:/桌面/test.lp");

      /*  for (int i = 0; i < initRoutesNum; ++i)
            cout << (x + i)->get(GRB_StringAttr_VarName) << " " << (x + i)->get(GRB_DoubleAttr_X) << endl;*/
        cout << "Obj: " << model.get(GRB_DoubleAttr_ObjVal) << endl;

        vector<float_t> pi;
        static float_t reduceCost = 0;

        int retryCnt = 20;

        while (true)
        {
            if (GRB_OPTIMAL != model.get(GRB_IntAttr_Status))
                break;

            int constrsNum = model.get(GRB_IntAttr_NumConstrs);
            
            pi.clear();
            pi.push_back(0);
            for (int i = 0; i < constrsNum; ++i)
            {
                pi.push_back((float_t)model.getConstr(i).get(GRB_DoubleAttr_Pi));
            }
            pi.push_back(0);

            graph.updateCost(pi);

            LabellingAlg labelAlg;
            Label rst = labelAlg.labelling_SPPRC(graph, startIndex, endIndex, capacity);
            db_print(DB_WARNING, "reduce_cost: %f\n", rst.cost);
             
            if (rst.cost >= 0)
                break;

            // 若reduced cost与上次相同，给与一定的重试，若还是不变，则退出
            if (reduceCost == rst.cost) {
                retryCnt--;
                if (retryCnt < 0)
                    break;
            }
            else {
                retryCnt = 20;
            }

            reduceCost = rst.cost;
            Route curRoute;
            curRoute.path.assign(rst.path.begin(), rst.path.end());
            curRoute.arriveTime.assign(rst.arriveTime.begin(), rst.arriveTime.end());
            curRoute.load = rst.load;
            curRoute.totalDis = rst.dis;
            graph.addRoute(curRoute);

            GRBColumn col;
            for (int i = 1; i < nodesNum - 1; ++i)
            {
                if (count(rst.path.begin(), rst.path.end(), i) > 0)
                {
                    col.addTerm(1, constrs[i-1]);
                }
            }
            x.push_back(model.addVar(0, 1, rst.dis, GRB_CONTINUOUS, col));
            
            model.update();
            model.optimize();
            model.write("E:/桌面/curtest.lp");
            cout << "Obj: " << model.get(GRB_DoubleAttr_ObjVal) << endl;
        }

        int cnt = 1;

        for (int i = 0; i < x.size(); ++i)
        {
            double attr = x[i].get(GRB_DoubleAttr_X);
            //db_print(DB_NORMAL, "%llf\n", attr);
            if (attr > 0.4)
            {
                cout << "route " << cnt << ": ";
                for (auto node : graph.getRoute(i)->path)
                {
                    cout << node << "->";
                }
                cout << endl;
                cnt++;
            }
        }

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
