//  InputGenerator
//  Created by Oscar Tovar

#include <iostream>
#include <math.h>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <fstream>

using namespace std;

int main() {
    srand(time(NULL));
    int tp = 20;//Total Page Frames in Memory
    int sl = 5;//Segment Length in Pages
    int ps = 32;//Page Size in Entries
    int r = 5;//Pages per Process
    int x = 3;//Look ahead value
    int min = 14, max = 18;
    int k = 4;//Number of processes to fork
    int maxRequests[k];
    int currentRequests[k];
    int totalRequests = 0;
    bool finishedProcesses[k];
    
    for (int i = 0; i < k; i++) {
        currentRequests[i] = 0;
        finishedProcesses[i] = false;
        maxRequests[i] = rand() % 5 + 1;
        totalRequests += maxRequests[i];
    }
    
    
    
    for (int i = 0; i < k; i++) {
        cout << 100 + i << " " << 32 << endl;
    }
    for (int i = 0; i < totalRequests + k; i++) {
        int pid = 0;
        do {
           pid = (rand() % 4);
        } while (finishedProcesses[pid] == true);
        if (currentRequests[pid] < maxRequests[pid]) {
            cout << std::dec << pid + 100 << " 0x" << std::hex << setw(2) << setfill('0') << rand() % (sl*ps) + 1 << setfill(' ') <<  endl;
            currentRequests[pid] = currentRequests[pid] + 1;
        }
        else {
            cout << std::dec << pid + 100 << " -1" << endl;
            finishedProcesses[pid] = true;
        }
    }
    return 0;
}
