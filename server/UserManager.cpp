#include "UserManager.h"

std::shared_ptr<AIchatbot> UserManager::get_or_create_bot(const std::string& user_id)
{
	std::lock_guard<std::mutex>lock(map_mutex);

	if (users.find(user_id) == users.end())users[user_id] = std::make_unique<AIchatbot>();
	return users[user_id];

}

void UserManager::remove_bot(const std::string& user_id)
{
	std::lock_guard<std::mutex> lock(map_mutex);
	users.erase(user_id);

}
