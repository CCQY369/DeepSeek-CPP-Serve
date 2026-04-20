#define CPPHTTPLIB_OPENSSL_SUPPORT //启用SSL支持，如果要访问HTTPS网站
#include "httplib.h"
#include "AIserver.h"

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "crypt32.lib")

// 2. OpenSSL 核心库
// 注意：如果你的 VS 是 Debug 模式，就用 MDd；如果是 Release 模式，就用 MD
#pragma comment(lib, "C:\\OpenSSL-Win64\\lib\\VC\\x64\\MDd\\libssl.lib")
#pragma comment(lib, "C:\\OpenSSL-Win64\\lib\\VC\\x64\\MDd\\libcrypto.lib")

AIserver::AIserver(std::shared_ptr<ThreadPool> pool, std::shared_ptr<UserManager> user)
{
	std::cout << "AI Server running: http://localhost:8080" << std::endl;
	this->svr = std::make_unique<httplib::Server>();
	this->threads = pool;
	this->user_manager = user;
	this->svr->Post("/chat", [&](const httplib::Request& req, httplib::Response& res) {
		try {
			json request = json::parse(req.body);
			if (!request.contains("user_id") || !request.contains("message"))
			{
				res.status = 400;
				res.set_content("missing field", "text/plain");
				return;
			}

			std::string user_id = request["user_id"];
			std::string user_input = request["message"];
			auto bot = user_manager->get_or_create_bot(user_id);

			auto future_res = threads->enqueue([bot, user_input]() {
				bot->user_messages_set(user_input);
				json body = bot->build_body();

#ifdef CPPHTTPLIB_OPENSSL_SUPPORT
				std::cout << "CHECK: Macro is defined!" << std::endl;
#else
				std::cout << "CHECK: Macro is MISSING!" << std::endl;
#endif
				httplib::Client cli(bot->get_wb());

				cli.enable_server_certificate_verification(false);

				cli.set_connection_timeout(5, 0);
				cli.set_read_timeout(10, 0);

				auto response = cli.Post(
					"/chat/completions",
					bot->get_headers(),
					body.dump(),
					"application/json"
				);


				if (response) {
					std::cout << "HTTP Status: " << response->status << std::endl;
				}
				else {
					auto err = response.error(); // 关键！
					std::cout << "Connect Error: " << (int)err << std::endl;
					// 1 代表连接失败, 5 代表 SSL 握手失败, 11 代表证书错误
				}

				std::string text;
				if (response && response->status == 200)
				{
					text = bot->get_ai_text(json::parse(response->body));
					bot->assistant_messages_set(text);
				}
				else if (response && response->status == 400) {
					std::cout << "Error Detail: " << response->body << std::endl;
				}
				else
				{
					text = u8"网络连接失败或 API 调用错误";
				}

				return text;
				});

			std::string ai_response = future_res.get();

			json response;
			response["status"] = "success";
			response["reply"] = ai_response;

			res.set_content(response.dump(), "application/json");

		}
		catch (const std::exception& e)
		{
			res.status = 500;
			res.set_content(std::string("error: ") + e.what(), "text/plain");
		}
		});


}

void AIserver::start(int port)
{
	svr->listen("0.0.0.0", port);
}
