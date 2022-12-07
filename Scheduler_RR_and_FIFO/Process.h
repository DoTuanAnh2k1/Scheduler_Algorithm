#pragma once
#include <iostream>
#include <vector>
using namespace std;

class Process
{
private:
    int pid;
    int arrival_time;
    int burst_time;
    int start_time;
    int completion_time;
    int turnaround_time;
    int waiting_time;
    int response_time;
public:
    Process (int, int, int);
    Process();

    void SetPid(int);
    void SetArrival_time(int);
    void SetBurst_time(int);
    void SetStart_time(int);
    void SetCompletion_time(int);
    void SetTurnaround_time(int);
    void SetWaiting_time(int);
    void SetResponse_time(int);

    int GetPid();
    int GetArrival_time();
    int GetBurst_time();

    void Show();

    static void Round_Robin(vector<Process>, vector <int>, int);
    static void FIFO(vector <Process>, int n);
};