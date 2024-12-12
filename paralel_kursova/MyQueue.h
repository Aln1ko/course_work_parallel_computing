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
		std::unique_lock<std::mutex> lock(mtx);
		q.push(t);
		lock.unlock();
		con_var.notify_one();
	}

	Task front_and_pop() {
		Task t = q.front();
		q.pop();
		return t;
	}

	bool empty() {
		return q.empty();
	}
	std::mutex& get_mut() {
		return mtx;
	}
	int size() {
		return q.size();
	}

};