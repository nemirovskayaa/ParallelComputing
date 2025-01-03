# Tread Pool Implementation
Here is an implementation of ThreadPool that leverages parallelisation and synchronisation mechanisms in C++, including condition_variable, shared_mutex with shared_lock and unique_lock. The ThreadPool operates with 6 threads and has a single queue with a capacity of 20 Tasks. Tasks are either pushed to the end of the queue or discarded if queue is full. Threads pick up Tasks for execution as soon as they become available. Each Task takes 5-10 s to execute.

ConqurrencyQueue was implemented to store Tasks.

A mechanism for checking results on status of the Task is provided too.

Additionally, there is functionality to manage work of ThreadPool using commands
- a - add tasks to the queue;
- p - pause ThreadPool;
- r - resume ThreadPool;
- b - break ThreadPool, waiting for finishing execution of tasks;
- m - momentary break ThreadPool, that stops all executions.

Console output and input are handled by separate thread.

Finally, before exiting, the program prints some statistics, including total amount of ignored tasks, minimal and maximum time while queue was full.
