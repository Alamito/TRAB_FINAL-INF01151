#ifndef SUM_TABLE_H
#define SUM_TABLE_H

#include <iostream>
#include <string>
#include <mutex>
#include <chrono>
#include <thread>


class SumTable 
{
    private:
        double numReq;
        double totalSum;
        std::mutex mutex_lock; 
    
    public:
        /*constructor*/
        SumTable(): numReq(0), totalSum(0) {};

        /*getters*/
        int getRequests() const { return numReq; };
        int getSum() const { return totalSum; };

        
        int updateTable(int numberToSum);
        int printTable();
};

#endif