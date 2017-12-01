/*
 * Generates valid memory requests.
 */

#include <iostream>
#include <math.h>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <fstream>

using namespace std;

int main() {
	srand(time(NULL));
	for (int i = 0; i < 50; i++) {
		cout << "Memory Request 0x" << std::hex << setw(2) << setfill('0') << rand() % 64 + 1 << setfill(' ') <<  endl;
	}
	return 0;
}
