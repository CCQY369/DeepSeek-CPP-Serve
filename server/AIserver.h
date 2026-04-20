#pragma once
#define CPPHTTPLIB_OPENSSL_SUPPORT

#include "httplib.h"
#include "json.hpp"
#include "AIchatbot.h"
#include "ThreadPool.h"
#include "UserManager.h"
#include <memory>
#include <string>

class AIserver
{
private:
	std::unique_ptr<httplib::Server> svr;
	std::shared_ptr<ThreadPool> threads;
	std::shared_ptr<UserManager>user_manager;
public:
	AIserver(std::shared_ptr<ThreadPool> pool, std::shared_ptr<UserManager> user);
	void start(int port);

};

