#pragma once
#include <thread>
#include <mutex>
#include <vector>
#include <queue>
#include <future>
#include <condition_variable>
#include <functional>

class ThreadPool
{
private:
	bool finish;
	std::mutex queue_mutex;
	std::condition_variable cv;
	std::vector<std::thread> workers;
	std::queue<std::function<void()>>task_queue;
public:
	ThreadPool(size_t threads);
	template<class F>
	auto enqueue(F&& f) -> std::future<typename std::result_of<F()>::type>
	{
		using return_type = typename std::result_of<F()>::type;

		auto task = std::make_shared<std::packaged_task<return_type()>>
			(
				std::forward<F>(f)
			);

		std::future<return_type> res = task->get_future();

		{
			std::unique_lock<std::mutex> lock(queue_mutex);
			if (finish) throw std::runtime_error("enqueue on stopped ThreadPool");
			task_queue.emplace([task]() { (*task)(); });
		}
		this->cv.notify_one();
		return res;
	}
	~ThreadPool();
};

