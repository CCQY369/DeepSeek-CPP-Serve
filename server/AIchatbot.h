#pragma once
#include "httplib.h"
#include "json.hpp"
#include <iostream>
#include <string>
#include <deque>
#include <fstream>
using json = nlohmann::json;

#define MAX_MEMORY 20


class AIchatbot
{
private:
	std::deque<json>memory_deque;
	json system_setting;
	std::string model_wb;
	std::string api_key;
	httplib::Headers headers;
public:
	std::string get_wb()const { return model_wb; };
	std::string get_ai_text(const json& j_res);
	httplib::Headers get_headers()const;
	json build_body();
public:
	AIchatbot(const AIchatbot&) = delete;
	AIchatbot& operator=(const AIchatbot&) = delete;
	AIchatbot();
public:
	void user_messages_set(const std::string& input);
	void assistant_messages_set(const std::string& ai_text);
public:
	void Print_ai_reply(const std::string& ai_text);
};


