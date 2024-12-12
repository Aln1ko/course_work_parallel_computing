#include "ThreadPool.h"

ThreadPool::ThreadPool(int consumers, MyQueue& q) :
	num_consumers(consumers), queue(q)
{}

void ThreadPool::inizialize() {
	for (int i = 0; i < num_consumers; i++) {
		consumer_threads.emplace_back(&ThreadPool::consume, this);
	}
}


void ThreadPool::consume()
{
	while (true) {
		std::mutex& m = queue.get_mut();
		std::unique_lock<std::mutex> lock(m);
		while ((queue.empty() || paused) && !finished) {
			queue.con_var.wait(lock);
		}
		if (finished) return;
		Task t = queue.front_and_pop();
		lock.unlock();
		t.execute();

	}
}

void ThreadPool::pause() {
	std::lock_guard<std::mutex> lock(m_status);
	paused = true;
	std::cout << "pause started" << std::endl;
}

void ThreadPool::resume() {
	std::lock_guard<std::mutex> lock(m_status);
	paused = false;
	queue.con_var.notify_all();
	std::cout << "pause finished" << std::endl;
}

void ThreadPool::finish() {
	std::lock_guard<std::mutex> lock(m_status);
	finished = true;
	queue.con_var.notify_all();
}

ThreadPool::~ThreadPool()
{
	for (int i = 0; i < num_consumers; i++) {
		consumer_threads[i].join();
	}
}
