#pragma once
#include "MyQueue.h"
#include <vector>
#include <thread>
#include <iostream>


class ThreadPool {
	std::vector<std::thread>consumer_threads;
	int num_consumers;
	MyQueue& queue;
	bool finished = false;
	bool paused = false;
	std::mutex m_status;

	void consume();

public:
	ThreadPool(int consumers, MyQueue& q);
	void inizialize();
	~ThreadPool();
	void finish();
	void pause();
	void resume();

};
