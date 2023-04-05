#pragma once

#include "ConcurrencyQueue.h"
#include "Task.h"
#include <vector>
#include <thread>
#include <condition_variable>

#define MAX_Q_SIZE 20
#define MAX_THREAD_NUM 6

using namespace std::chrono;

class ThreadPool
{
private:
	ConcurrencyQueue<Task*> m_queue;
	vector<thread> m_threads;

	mutex m_rw_mutex, m_pause_mutex;//, m_clock_mutex;
	condition_variable m_wait_for_task;
	condition_variable m_pause;

	bool m_break_execution;
	bool m_initialised;
	bool m_is_paused;

	high_resolution_clock::time_point m_start_time;
	high_resolution_clock::time_point m_end_time;

public:
	unsigned long long max_time, min_time;

	ThreadPool();
	~ThreadPool();
	void init();
	void add_task(Task*);
	void routine();
	void break_safe();
	void break_momentary();
	void pause();
	void resume();
};