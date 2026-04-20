\# DeepSeek-CPP-Serve 🤖



基于 C++11 编写的高性能 AI 聊天后端服务器，集成了 DeepSeek 大模型。



\## 🌟 项目亮点

\- \*\*多线程架构\*\*：基于自定义线程池（ThreadPool），支持多并发请求处理。

\- \*\*安全传输\*\*：集成 OpenSSL，支持 HTTPS 加密通信。

\- \*\*上下文管理\*\*：通过 UserManager 维护不同用户的对话记忆。

\- \*\*配置分离\*\*：API Key 与代码解耦，支持通过 `config.json` 灵活配置。



\## 🛠️ 环境依赖

\- \*\*IDE\*\*: Visual Studio 2022+

\- \*\*库\*\*: \[cpp-httplib](https://github.com/yhirose/cpp-httplib), \[nlohmann/json](https://github.com/nlohmann/json)

\- \*\*依赖项\*\*: OpenSSL 3.x (Win64)



🛠️ 如何开始 (How to Run)

1、克隆本项目到本地。



2、在项目根目录找到 config.json.example。



3、复制并重命名为 config.json。



4、将你的 DeepSeek API Key 填入 config.json 中。



5、使用 Visual Studio 编译并运行。

