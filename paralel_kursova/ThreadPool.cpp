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
		//std::cout << queue.size() << std::endl;
		std::unique_lock<std::mutex> lock(m);
		while ((queue.empty() || paused) && !finished && !working_to_the_end) {
			queue.con_var.wait(lock);
		}
		if (finished || (queue.empty() && working_to_the_end)) return;
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

void ThreadPool::working_to_the_end_finish() {
	std::lock_guard<std::mutex> lock(m_status);
	working_to_the_end = true;
	queue.con_var.notify_all();
	
}

int ThreadPool::get_size_q() { 
	std::mutex& m = queue.get_mut(); 
	std::unique_lock<std::mutex> lock(m);
	int res = queue.size();
	lock.unlock();
	return res;
}

ThreadPool::~ThreadPool()
{
	finish();
	for (int i = 0; i < num_consumers; i++) {
		if (consumer_threads[i].joinable()) 
		{
			consumer_threads[i].join();
		}
	}
}
