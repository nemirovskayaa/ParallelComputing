// PC_LAB2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "Task.h"
#include "ThreadPool.h"
#include <unordered_map>

using namespace std;

shared_mutex task_hs_mutex;
void add_to_pool(ThreadPool&, unordered_map<unsigned int, Task*>&);
void print_info(unordered_map<unsigned int, Task*>&, bool&, unsigned long long&, unsigned long long&);
bool key_process(KEY_EVENT_RECORD, ThreadPool&, unordered_map<unsigned int, Task*>&);
void error_exit(const string );

HANDLE h_std_inp;
DWORD  initial_mode;

int main()
{
    const unsigned int inp_buff_size = 64;
    DWORD rec_num_read;
    INPUT_RECORD inp_buffer[inp_buff_size];

    h_std_inp = GetStdHandle(STD_INPUT_HANDLE);
    if (h_std_inp == INVALID_HANDLE_VALUE)
        error_exit("Invalid handle.");

    if (!GetConsoleMode(h_std_inp, &initial_mode))
        error_exit("Invalid get mode.");

    if (!SetConsoleMode(h_std_inp, ENABLE_INSERT_MODE))
        error_exit("Invalid set mode.");

    bool stop_print_flag = false;
    unsigned long long min_full_q_time, max_full_q_time;
    unordered_map<unsigned int, Task*> tasks_to_print;

    thread print_th = thread(print_info, ref(tasks_to_print), ref(stop_print_flag),
        ref(min_full_q_time), ref(max_full_q_time));

    {
        ThreadPool tp;
        tp.init();

        bool stop_pool = false;
        while (!stop_pool)
        {
            if (!ReadConsoleInput(h_std_inp, inp_buffer, inp_buff_size, &rec_num_read))
                error_exit("Read console input error.");

            for (DWORD i = 0; i < rec_num_read; i++)
            {
                if (inp_buffer[i].EventType == KEY_EVENT)
                    stop_pool = key_process(inp_buffer[i].Event.KeyEvent, tp, tasks_to_print);
            }
        }

        min_full_q_time = tp.min_time;
        max_full_q_time = tp.max_time;
    }

    stop_print_flag = true;
    print_th.join();

    for (auto i : tasks_to_print)
    {
        Task* temp = i.second;
        delete temp;
        temp = nullptr;
    }

    SetConsoleMode(h_std_inp, initial_mode);
}

void add_to_pool(ThreadPool& tp, unordered_map<unsigned int, Task*>& tasks)
{
    Task* new_task;
    for (int i = 0; i < 30; i++)
    {
        new_task = new Task();
        tp.add_task(new_task);

        task_hs_mutex.lock();
        tasks.insert(pair<unsigned int, Task*>(new_task->get_ID(), new_task));
        task_hs_mutex.unlock();
    }
}

void print_info(unordered_map<unsigned int, Task*>& tasks, bool& stop_flag,
    unsigned long long& min_full_q_time, unsigned long long& max_full_q_time)
{
    cout << "Thread Pool Demonstartor" << endl << endl;
    cout << "Instruction:" << endl;
    cout << "\ta - add tasks to Thread Pool Queue" << endl;
    cout << "\tp - pause Thread Pool" << endl;
    cout << "\tr - resume Thread Pool" << endl;
    cout << "\tb - break Thread Pool" << endl;
    cout << "\tm - momentary break Thread Pool" << endl << endl;

    unsigned int ignored_tasks = 0;

    while (!stop_flag)
    {
        vector<unsigned int> tasks_to_del;
        if (tasks.empty())
            continue;

        cout << string(50, '-') << endl;
        for (auto i : tasks)
        {
            cout << (*i.second).get_string() << endl;

            if ((*i.second).get_status() == Status::IGNORED)
                ignored_tasks++;

            if ((*i.second).get_status() == Status::IGNORED || (*i.second).get_status() == Status::EXECUTED)
                tasks_to_del.push_back(i.first);
        }

        for (auto i : tasks_to_del)
        {
            Task* temp = tasks.at(i);
            delete temp;
            temp = nullptr;
            tasks.erase(i);
        }

        cout << endl << endl;
        Sleep(4000);
    }

    if (!tasks.empty())
        cout << string(50, '-') << endl;

    for (auto i : tasks)
        cout << (*i.second).get_string() << endl;

    cout << string(30, '-') << endl;
    cout << "Final statistic: " << endl;
    cout << "Amount of ignored tasks: " << ignored_tasks << endl;
    cout << "Full queue MIN time:     " << min_full_q_time << endl;
    cout << "Full queue MAX time:     " << max_full_q_time << endl;
}

void error_exit(const string message)
{
    cout << message << endl;
    SetConsoleMode(h_std_inp, initial_mode);
    ExitProcess(0);
}

bool key_process(KEY_EVENT_RECORD key_record, ThreadPool& tp, unordered_map<unsigned int, Task*>& tasks_to_print)
{
    if (key_record.bKeyDown)
    {
        switch (key_record.uChar.UnicodeChar)
        {
        case 'a': // add tasks
        {
            cout << "Adding tasks to queue..." << endl;
            thread add_th = thread(add_to_pool, ref(tp), ref(tasks_to_print));
            add_th.join();
            return false;
        }
        case 'p': // pause
            cout << "Thread pool was paused." << endl;
            tp.pause();
            return false;

        case 'r': // resume
            cout << "Thread pool was resumed." << endl;
            tp.resume();
            return false;

        case 'b': // break
            cout << "Thread pool was stopped." << endl;
            tp.break_safe();
            return true;

        case 'm': // break momentary
            cout << "Thread pool was momentary stopped." << endl;
            tp.break_momentary();
            return true;
        }
    }
    return false;
}
