#pragma once
#include "json.hpp"
#include "AIchatbot.h"
#include <iostream>
#include <map>
#include <memory>
#include <mutex>
using json = nlohmann::json;


class UserManager
{
private:
	std::map<std::string, std::shared_ptr<AIchatbot>>users;
	std::mutex map_mutex;

public:
	std::shared_ptr<AIchatbot> get_or_create_bot(const std::string& user_id);
	void remove_bot(const std::string& user_id);
};

