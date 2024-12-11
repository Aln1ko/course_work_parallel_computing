#pragma once
#include "MyQueue.h"
#include <vector>
#include <thread>


class ThreadPool {
	std::vector<std::thread>consumer_threads;
	int num_consumers;
	MyQueue& queue;
	void consume();
public:
	ThreadPool(int consumers, MyQueue& q);
	~ThreadPool();

};
