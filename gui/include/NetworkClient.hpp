#pragma once
#include "Common.hpp"

namespace Zappy {
    class NetworkClient {
    private:
        int socketFd{-1};
        bool connected{false};
        std::string readBuffer;
        std::queue<std::string> messageQueue;

        void processBuffer();
        
    public:
        NetworkClient() = default;
        ~NetworkClient();

        NetworkClient(const NetworkClient&) = delete;
        NetworkClient& operator=(const NetworkClient&) = delete;

        bool connect(const std::string& host, int port);
        void disconnect();
        bool isConnected() const { return connected; }

        bool send(const std::string& message);
        bool receive();

        bool hasMessage() const { return !messageQueue.empty(); }
        std::string getMessage();
    };
}