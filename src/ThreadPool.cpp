#include "ThreadPool.h"

#include <iostream>
ThreadPool::ThreadPool(unsigned int size) :
		size(size),
		work(true) {

	for (unsigned int i = 0; i < size; ++i) {
		this->workers.emplace_back(&ThreadPool::workerHandler, this);
	}
}

ThreadPool::~ThreadPool() {
	this->shutdown();
}

void ThreadPool::execute(const ThreadPool::Task &t) {
	std::unique_lock<std::mutex> lock(this->tasksQueueMutex);
	this->tasksQueue.push(t);
	lock.unlock();
	this->taskCv.notify_one();
}

void ThreadPool::workerHandler() {
	while (this->work.load()) {
		std::unique_lock<std::mutex> lock(this->tasksQueueMutex);
		this->taskCv.wait(lock, [&]() {
			return !this->tasksQueue.empty() || !this->work.load();
		});
		if (this->tasksQueue.empty()) {
			continue;
		}
		Task t = this->tasksQueue.front();
		this->tasksQueue.pop();
		lock.unlock();

		t();
	}
}

void ThreadPool::shutdown() {
	if (!this->work.load()) {
		return;
	}

	this->work.store(false);
	this->taskCv.notify_all();

	for (std::thread &worker : this->workers) {
		worker.join();
	}
}

unsigned int ThreadPool::getSize() const {
	return this->size;
}

bool ThreadPool::shuttingDown() const {
	return !this->work.load();
}
