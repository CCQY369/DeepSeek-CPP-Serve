#include "ThreadPool.h"

ThreadPool::ThreadPool(size_t threads) :finish(false)
{
	for (size_t i = 0; i < threads; i++) {
		workers.emplace_back([this] {
			while (true)
			{
				std::function<void()> task;
				{
					std::unique_lock<std::mutex>lock(queue_mutex);
					cv.wait(lock, [this] {
						return this->finish || !this->task_queue.empty();
						});
					if (this->finish && this->task_queue.empty()) return;
					task = std::move(this->task_queue.front());
					this->task_queue.pop();
				}
				task();
			}
			});
	}
}

ThreadPool::~ThreadPool()
{
	{
		std::unique_lock<std::mutex>lock(queue_mutex);
		finish = true;
	}

	for (auto& woker : workers)woker.join();
}
