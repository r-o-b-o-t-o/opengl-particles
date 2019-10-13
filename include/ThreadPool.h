#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <queue>
#include <vector>
#include <thread>
#include <atomic>
#include <functional>
#include <condition_variable>

class ThreadPool {
public:
	ThreadPool(unsigned int size);
	~ThreadPool();

	ThreadPool(const ThreadPool &rhs) = delete;
	ThreadPool &operator=(const ThreadPool &rhs) = delete;
	ThreadPool(ThreadPool &&rhs) = delete;
	ThreadPool &operator=(ThreadPool &&rhs) = delete;

	typedef std::function<void()> Task;
	void execute(const Task &t);
	void shutdown();
	unsigned int getSize() const;
	bool shuttingDown() const;

private:
	unsigned int size;
	std::atomic<bool> work;
	std::vector<std::thread> workers;
	std::condition_variable taskCv;
	std::queue<Task> tasksQueue;
	std::mutex tasksQueueMutex;

	void workerHandler();
};

#endif