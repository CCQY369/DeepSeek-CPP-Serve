//#include "httplib.h"
#include "AIchatbot.h"

std::string AIchatbot::get_ai_text(const json& j_res)
{
	try {
		return j_res.at("choices").at(0).at("message").at("content").get<std::string>();
	}
	catch (const std::exception& e) {
		return u8"解析AI回复失败，请重试";
	}
}

httplib::Headers AIchatbot::get_headers()const
{
	return headers;
}

json AIchatbot::build_body()
{
	json body;

	body["model"] = "deepseek-chat";
	body["stream"] = false;

	json messages = json::array();
	messages.emplace_back(system_setting);
	for (const auto& msg : memory_deque) messages.emplace_back(msg);
	body["messages"] = std::move(messages);
	return body;
}
AIchatbot::AIchatbot()
{
	system_setting = {
	{"role","system"},
	{"content", u8"你是一个温柔、有帮助的AI聊天好友，回答简洁自然"}
	};
	model_wb = "https://api.deepseek.com";
	std::ifstream config_file("D:\\C++\\AIServer\\x64\\Debug\\config.json");

	if (config_file.is_open())
	{
		json j;
		config_file >> j;
		api_key = j.value("api_key", "");
	}
	std::cout << "Key loaded: " << api_key << std::endl;
	headers = {
		{"Authorization","Bearer " + api_key},
		{"Content-Type","application/json"}
	};
}

void AIchatbot::user_messages_set(const std::string& input)
{
	if (input.empty()) return;

	memory_deque.push_back({ {"role","user"},{"content",input} });

	while (memory_deque.size() > MAX_MEMORY) memory_deque.pop_front();

}



void AIchatbot::assistant_messages_set(const std::string& ai_text)
{
	if (ai_text.empty()) return;

	memory_deque.push_back({ {"role","assistant"},{"content",ai_text } });

	while (memory_deque.size() > MAX_MEMORY) memory_deque.pop_front();
}

void AIchatbot::Print_ai_reply(const std::string& ai_text)
{
	std::cout << u8"AI助手：" << ai_text << std::endl;
}
