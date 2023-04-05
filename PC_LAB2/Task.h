#pragma once

#include <iostream>
#include <string>
#include <chrono>
#include <shared_mutex>
#include <Windows.h>

#define MIN_TASK_TIME 5000
#define MAX_TASK_TIME 10000

using namespace std;

enum class Status
{
	IS_STORING, IGNORED, IS_PROCESSING, EXECUTED
};

class Task
{
private:
	static unsigned int s_ID;
	unsigned int m_taskID;

	Status m_status;
	mutable shared_mutex m_rw_mutex;
	unsigned int m_duration_ms;
	chrono::high_resolution_clock::time_point m_start_time;
	chrono::high_resolution_clock::time_point m_end_time;
	chrono::milliseconds m_result_time;

public:
	Task();
	~Task();
	void do_work();

	void set_status(Status);
	Status get_status();
	unsigned int get_ID();
	string get_string();

	string get_status_str()
	{
		switch (m_status)
		{
		case Status::IS_STORING:
			return "IS STORING";
		case Status::IGNORED:
			return "WAS IGNORED";
		case Status::IS_PROCESSING:
			return "IS PROCESSING...";
		case Status::EXECUTED:
			return "WAS EXECUTED";
		default:
			return "STATUS ERROR";
		}
	}

	Task& operator= (const Task&) { return *this; };
	Task& operator= (const Task&&) { return *this; };
};