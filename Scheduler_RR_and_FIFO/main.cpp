#include <iostream>
#include "Process.h"
using namespace std;

int main()
{
	vector<Process> p;
	int n;
	vector <int> burst_time_remaining;

	cout << "Enter the number of processes: ";
	cin >> n;

	//Process::Round_Robin(p, burst_time_remaining, n);
	Process::FIFO(p, n);
	return 0;
	
}