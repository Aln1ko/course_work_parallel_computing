#pragma once
#include <queue>
#include "Task.h"
#include <mutex>
#include <condition_variable>

class MyQueue {
	std::queue<Task> q;
	std::mutex mtx;
public:
	std::condition_variable con_var;

	void push(Task t) {
		mtx.lock();
		q.push(t);
		con_var.notify_one();
		mtx.unlock();
	}

	Task front_and_pop() {
		Task t = q.front();
		q.pop();
		return t;
	}
	bool empty() {
		return q.empty();
	}
};