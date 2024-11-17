#include "SumTable.h"

using namespace std;

int SumTable::updateTable(int numberToSum) {
    /*se consegue pegar o mutex, soma, se nao, espera*/
    while(true){
        cout << "updating table dando erradooeer" << endl;
        if(this->mutex_lock.try_lock()){ 
            cout << endl << "  ...updating table..." << endl << endl;
            this->totalSum = this->totalSum + numberToSum;
            this->numReq = this->numReq + 1; 
            this->mutex_lock.unlock();
            return true;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));  
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
        std::this_thread::sleep_for(std::chrono::milliseconds(10));  
    }
}
