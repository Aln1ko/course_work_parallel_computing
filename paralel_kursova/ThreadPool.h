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
	bool working_to_the_end = false;
	void consume();

public:
	ThreadPool(int consumers, MyQueue& q);
	void inizialize();
	~ThreadPool();
	void finish();
	void pause();
	void resume();
	void working_to_the_end_finish();
	int get_size_q();

};
