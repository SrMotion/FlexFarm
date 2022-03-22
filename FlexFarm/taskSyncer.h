#pragma once
#include <mutex>
#include <thread>
#include <iostream>
#include <vector>
#include <functional>
#include <chrono>
#include <string>
#include <future>


template <typename mutex>
class taskSyncer
{
public:
	taskSyncer() {
	}

	taskSyncer(std::chrono::milliseconds duration) {
		checker = std::async(std::launch::async, &taskSyncer::checkerFunc, this, duration);
	}

	~taskSyncer() {
		running.store(false);
		checker.wait();
	}

	void lock() {
		std::unique_lock<mutex> lock(mtx);
		waiting.store(true);
		c_var.wait(lock, [&]() -> bool { return flag.load(); });
		waiting.store(false);
		flag.store(false);
	}

	void unlock() {
		if (waiting.load()) {
			std::lock_guard<mutex> lock(mtx);
			flag.store(true);
			c_var.notify_one();
		}
	}

	void checkerFunc(std::chrono::milliseconds duration) {
		while (running.load()) {
			if (waiting.load()) {
				std::this_thread::sleep_for(duration);
				if (waiting.load())
					unlock();
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
	}

	mutex mtx;
	std::future<void> checker;
	std::condition_variable c_var;
	std::atomic_bool flag = false, running = true, waiting;
};

