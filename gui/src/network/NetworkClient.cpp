/*
** EPITECH PROJECT, 2025
** B-YEP-400-LIL-4-1-zappy-yanis.asselman
** File description:
** Network Client
*/

#include "../../include/NetworkClient.hpp"

namespace Zappy {

NetworkClient::~NetworkClient()
{
    disconnect();
}

bool NetworkClient::connect(const std::string& host, int port)
{
    socketFd = socket(AF_INET, SOCK_STREAM, 0);
    if (socketFd < 0) {
        std::cerr << "Failed to create socket" << std::endl;
        return false;
    }
    
    int flags = fcntl(socketFd, F_GETFL, 0);
    fcntl(socketFd, F_SETFL, flags | O_NONBLOCK);
    
    struct sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    
    if (inet_pton(AF_INET, host.c_str(), &serverAddr.sin_addr) <= 0) {
        if (host == "localhost") {
            inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);
        } else {
            std::cerr << "Invalid address: " << host << std::endl;
            close(socketFd);
            return false;
        }
    }
    
    if (::connect(socketFd, reinterpret_cast<struct sockaddr*>(&serverAddr), 
                 sizeof(serverAddr)) < 0) {
        if (errno != EINPROGRESS) {
            std::cerr << "Failed to connect: " << strerror(errno) << std::endl;
            close(socketFd);
            return false;
        }
    }
    
    connected = true;
    return true;
}

void NetworkClient::disconnect()
{
    if (!connected) return;
    
    if (socketFd >= 0) {
        close(socketFd);
        socketFd = -1;
    }
    
    connected = false;
    readBuffer.clear();
    
    std::queue<std::string> empty;
    messageQueue.swap(empty);
}

bool NetworkClient::send(const std::string& message)
{
    if (!connected) return false;
    
    std::string fullMessage = message + "\n";
    ssize_t sent = ::send(socketFd, fullMessage.c_str(), fullMessage.length(), MSG_NOSIGNAL);
    
    if (sent < 0) {
        if (errno != EAGAIN && errno != EWOULDBLOCK) {
            std::cerr << "Send failed: " << strerror(errno) << std::endl;
            connected = false;
            return false;
        }
    }
    
    return true;
}

bool NetworkClient::receive()
{
    if (!connected) return false;
    
    char tempBuffer[BUFFER_SIZE];
    ssize_t received = recv(socketFd, tempBuffer, sizeof(tempBuffer) - 1, MSG_DONTWAIT);
    
    if (received < 0) {
        if (errno != EAGAIN && errno != EWOULDBLOCK) {
            std::cerr << "Receive failed: " << strerror(errno) << std::endl;
            connected = false;
            return false;
        }
        return true;
    }
    
    if (received == 0) {
        connected = false;
        return false;
    }
    
    tempBuffer[received] = '\0';
    readBuffer += tempBuffer;
    processBuffer();
    
    return true;
}

void NetworkClient::processBuffer()
{
    size_t pos = 0;
    
    while ((pos = readBuffer.find('\n')) != std::string::npos) {
        std::string line = readBuffer.substr(0, pos);
        
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }
        if (!line.empty()) {
            messageQueue.push(line);
        }
        
        readBuffer.erase(0, pos + 1);
    }
}

std::string NetworkClient::getMessage()
{
    if (messageQueue.empty()) {
        return "";
    }
    
    std::string message = messageQueue.front();
    messageQueue.pop();
    return message;
}

}