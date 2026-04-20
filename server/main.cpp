#define CPPHTTPLIB_OPENSSL_SUPPORT //启用SSL支持，如果要访问HTTPS网站
#include "httplib.h"
#include "json.hpp"
#include "AIserver.h"
//#include <iostream>
//#include <string>
//#include <deque>
//#include <map>
//#include <memory>
//#include <queue>
//#include <thread>
//#include <mutex>
//#include <future>
//#include <functional>
using json = nlohmann::json;

// 1. 系统网络和加密基础（VS自带，直接点名即可）
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "crypt32.lib")

// 2. OpenSSL 核心库
// 注意：如果你的 VS 是 Debug 模式，就用 MDd；如果是 Release 模式，就用 MD
#pragma comment(lib, "C:\\OpenSSL-Win64\\lib\\VC\\x64\\MDd\\libssl.lib")
#pragma comment(lib, "C:\\OpenSSL-Win64\\lib\\VC\\x64\\MDd\\libcrypto.lib")

#pragma execution_character_set("utf-8")



int main()
{
    try {
        // 1. 动力系统：开启 4 个工作线程
        auto pool = std::make_shared<ThreadPool>(4);

        // 2. 记忆系统：管理不同用户的对话
        auto manager = std::make_shared<UserManager>();

        // 3. 指挥系统：启动 Web 服务器
        AIserver server(pool, manager);

        std::cout << "Successfully started AI Backend Server!" << std::endl;

        // 4. 阻塞运行：监听 8080 端口
        server.start(8080);
    }
    catch (const std::exception& e) {
        std::cerr << "Fatal Error: " << e.what() << std::endl;
        return -1;
    }

    return 0;
}