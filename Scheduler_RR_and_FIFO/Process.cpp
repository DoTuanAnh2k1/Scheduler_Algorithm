#include <iostream>
#include <algorithm>
#include <queue>
#include <cstring>
#include <iomanip>
#include <vector>
#include "Process.h"
using namespace std;

Process::Process(int pid, int arrival_time, int burst_time)
{
	this->pid = pid;
	this->arrival_time = arrival_time;
	this->burst_time = burst_time;
}

Process::Process()
{
	pid = 0;
	arrival_time = 0;
	burst_time = 0;
}

void Process::SetArrival_time(int arrival_time)
{
	this->arrival_time = arrival_time;
}

void Process::SetBurst_time(int burst_time)
{
	this->burst_time = burst_time;
}

void Process::SetPid(int pid)
{
	this->pid = pid;
}

void Process::SetStart_time(int start_time)
{
	this->start_time = start_time;
}

void Process::SetCompletion_time(int completion_time)
{
	this->completion_time = completion_time;
}

void Process::SetTurnaround_time(int turnaround_time)
{
	this->turnaround_time = turnaround_time;
}

void Process::SetWaiting_time(int waiting_time)
{
	this->waiting_time = waiting_time;
}

void Process::SetResponse_time(int response_time)
{
	this->response_time = response_time;
}

int Process::GetPid()
{
	return pid;
}

int Process::GetBurst_time()
{
	return burst_time;
}

int Process::GetArrival_time()
{
	return arrival_time;
}

void Process::Show()
{
	cout << "#P" << pid << endl;
	cout << "Arrivarl time: " << arrival_time << endl;
	cout << "Burst time: " << burst_time << endl;
	cout << "Start time: " << start_time << endl;
	cout << "Waiting time: " << waiting_time << endl;
}

void Input_Round_Robin(vector <Process> &p, vector <int> &burst_time_remaining, int n)
{
	for (int i = 0; i < n; i++)
	{
		Process pk(i + 1, 0, 0);

		int temp;

		cout << "Arrival time of process " << i + 1 << ": ";
		cin >> temp;
		pk.SetArrival_time(temp);

		cout << "Burst time of process " << i + 1 << ": ";
		cin >> temp;
		pk.SetBurst_time(temp);
		
		cout << endl;
		
		burst_time_remaining.push_back( pk.GetBurst_time() );
		p.push_back(pk);
	}
}

void SShow(vector <Process> p)
{
	for (int i = 0; i < p.size(); i++)
	{
		cout << "P" << i + 1 << " " << p[i].GetPid() << " " << p[i].GetArrival_time() << " " << p[i].GetBurst_time() << endl;
	}
}

void Swap_2_Process(Process &p1, Process &p2)
{
	Process temp;
	temp.SetPid(p1.GetPid());
	temp.SetArrival_time(p1.GetArrival_time());
	temp.SetBurst_time(p1.GetBurst_time());

	p1.SetArrival_time(p2.GetArrival_time());
	p1.SetBurst_time(p2.GetBurst_time());
	p1.SetPid(p2.GetPid());

	p2.SetArrival_time(temp.GetArrival_time());
	p2.SetPid(temp.GetPid());
	p2.SetBurst_time(temp.GetBurst_time());
}

void Sort_by_arrival_time(vector <Process> &p) 
{
	int max;
	for (int i = 0; i < p.size() - 1; i++)
	{
		max = i;
		for (int j = i + 1; j < p.size(); j++)
		{
			if (p[j].GetArrival_time() < p[max].GetArrival_time())
				max = j;
		}
		if (max != i)
			Swap_2_Process(p[i], p[max]);
	}
}

void Sort_by_pid(vector <Process> &p)
{
	int max;
	for (int i = 0; i < p.size() - 1; i++)
	{
		max = i;
		for (int j = i + 1; j < p.size(); j++)
		{
			if (p[j].GetPid() < p[max].GetPid())
				max = j;
		}
		if (max != i)
			Swap_2_Process(p[i], p[max]);
	}
}

int max(int a, int b)
{
	if (a > b)
		return a;
	else
		return b;
}


void Process::Round_Robin(vector <Process> p, vector <int> burst_time_remaining, int n)
{
	int time_quantum;

	float cpu_utilisation;

	int total_turnaround_time = 0;
	int total_waiting_time = 0;
	int total_response_time = 0;
	int total_idle_time = 0;

	float throughput;

	int idx;

	cout << setprecision(2) << fixed;

	Input_Round_Robin(p, burst_time_remaining, n);

	cout << "Enter time quantum: ";
	cin >> time_quantum;

	Sort_by_arrival_time(p);
	//SShow(p);

	queue<int> q;
	int current_time = 0;
	q.push(0);
	int completed = 0;
	vector<int> mark;
	for (int i = 0; i < p.size() + 1; i++)
	{
		mark.push_back(0);
	}
	mark[0] = 1;

	while(completed != n)
	{
		idx = q.front();
		q.pop();

		if (burst_time_remaining[idx] == p[idx].burst_time)
		{
			p[idx].start_time = max(current_time, p[idx].arrival_time);
			total_idle_time += p[idx].start_time - current_time;
			current_time = p[idx].start_time;
		}

		if (burst_time_remaining[idx] - time_quantum > 0)
		{
			burst_time_remaining[idx] -= time_quantum;
			current_time += time_quantum;
		}
		else
		{
			current_time += burst_time_remaining[idx];
			burst_time_remaining[idx] = 0;
			completed++;

			p[idx].completion_time = current_time;
			p[idx].turnaround_time = p[idx].completion_time - p[idx].arrival_time;
			p[idx].waiting_time = p[idx].turnaround_time - p[idx].burst_time;
			p[idx].response_time = p[idx].start_time - p[idx].arrival_time;

			total_turnaround_time += p[idx].turnaround_time;
			total_waiting_time += p[idx].waiting_time;
			total_response_time += p[idx].response_time;
		}
		for (int i = 1; i < n; i++)
		{
			if (burst_time_remaining[i] > 0 && p[i].arrival_time <= current_time && mark[i] == 0)
			{
				q.push(i);
				mark[i] = 1;
			}
		}

		if (burst_time_remaining[idx] > 0)
		{
			q.push(idx);
		}

		if (q.empty())
		{
			for (int i = 1; i < n; i++)
			{
				if (burst_time_remaining[i] > 0)
				{
					q.push(i);
					mark[i] = 1;
					break;
				}
			}
		}
	}

	cpu_utilisation = ((p[n - 1].completion_time - total_idle_time) / (float)p[n - 1].completion_time) * 100;
	throughput = float(n) / (p[n - 1].completion_time - p[0].arrival_time);

	Sort_by_pid(p);
	//SShow(p);

	cout << endl;
	cout << "#P\t" << "AT\t" << "BT\t" << "ST\t" << "CT\t" << "TAT\t" << "WT\t" << "RT\t" << "\n" << endl;

	for (int i = 0; i < n; i++)
	{
		cout << p[i].pid << "\t" << p[i].arrival_time << "\t" << p[i].burst_time << "\t" << p[i].start_time << "\t" << p[i].completion_time << "\t" << p[i].turnaround_time << "\t" << p[i].waiting_time << "\t" << p[i].response_time << "\t" << "\n" << endl;
	}

	cout << "Average Turnaround Time = " << (float)total_turnaround_time / n << endl;
	cout << "Average Waiting Time = " << (float)total_waiting_time / n << endl;
	cout << "Average Response Time = " << (float)total_response_time / n << endl;
	cout << "CPU Utilization = " << cpu_utilisation << "%" << endl;
	cout << "Throughput = " << throughput << " process/unit time" << endl;
}

void Input_First_In_First_Out(vector <Process>& p, int n)
{
	for (int i = 0; i < n; i++)
	{
		Process pk(i + 1, 0, 0);

		int temp;

		cout << "Arrival time of process " << i + 1 << ": ";
		cin >> temp;
		pk.SetArrival_time(temp);

		cout << "Burst time of process " << i + 1 << ": ";
		cin >> temp;
		pk.SetBurst_time(temp);

		cout << endl;
		p.push_back(pk);
	}
	Process pk;
	pk.SetArrival_time(9999);
	p.push_back(pk);
}

void Process::FIFO(vector <Process> p, int n)
{
	int Total_wait_time = 0;
	int Total_turn_time = 0;
	int count = 0;
	int time = 0;
	int smallest;

	Input_First_In_First_Out(p, n);
	
	while (count != n)
	{
		smallest = p.size() - 1;
		for (int i = 0; i < n; i++)
		{
			if (p[i].GetArrival_time() < p[smallest].GetArrival_time() && p[i].GetBurst_time() > 0)
			{
				smallest = i;
			}
		}
		
		time += p[smallest].GetBurst_time();
		Total_wait_time += time - p[smallest].GetArrival_time() - p[smallest].GetBurst_time();
		Total_turn_time += time - p[smallest].GetArrival_time();

		cout << "P" << smallest + 1 << "\t" << time - p[smallest].GetArrival_time() << "\t" << time - p[smallest].GetArrival_time() - p[smallest].GetBurst_time() << endl;
		
		p[smallest].SetBurst_time(0);
		count++;
	}
	
	cout << endl;
	cout << "Average waiting time = " << (float)Total_wait_time / n << endl;
	cout << "Average turnaround time = " << (float)Total_turn_time / n << endl;
}