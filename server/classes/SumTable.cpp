#include "SumTable.h"

using namespace std;

int SumTable::updateTable(int numberToSum) {
    /*se consegue pegar o mutex, soma, se nao, espera*/
    while(true){
        if(this->mutex_lock.try_lock()){ 
            this->totalSum = this->totalSum + numberToSum;
            this->numReq = this->numReq + 1; 
            this->mutex_lock.unlock();
            return this->totalSum;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));  
    }
}

int SumTable::printTable() {
    while(true){
        if(this->mutex_lock.try_lock()){ 
            cout << "  --------------------" << endl;
            cout << " |     Sum Table      |" << endl;
            cout << "  --------------------" << endl;
            cout << " | Number of requests |  " << this->numReq << endl <<" | Total Sum          |  " << this->totalSum << endl; 
            cout << "  --------------------" << endl;
            this->mutex_lock.unlock();
            return true;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));  
    }
}