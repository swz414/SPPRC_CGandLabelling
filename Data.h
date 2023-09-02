#pragma once

#include "comm.h"
#include "Customer.h"

class Data {
private:
	void addCustomer(Customer cust);
	void setVehicleNum(int vehi);
	void setCapicity(int capi);

public:
	int vehicleNum;
	int capicity;
	vector<Customer> customers;
	vector<vector<float_t>> disMat;

	Data() {
		vehicleNum = 0; capicity = 0;
	};

	// ��ȡ����
	bool ReadData(string filename, int maxCustNum = 10000);
};
