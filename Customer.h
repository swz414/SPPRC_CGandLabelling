#pragma once

class Customer {
public:
    int custNo;
    float_t posX;
    float_t posY;
    int demand;
    float_t readyTime;
    float_t dueTime;
    float_t servTime;

    Customer(int custNo, float_t posX, float_t posY, int demand, float_t readyTime, float_t dueTime, float_t servTime) :
        custNo(custNo),
        posX(posX),
        posY(posY),
        demand(demand),
        readyTime(readyTime),
        dueTime(dueTime),
        servTime(servTime) {};
};
