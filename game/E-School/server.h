#pragma once
#include "pch.h"

class LANClient {
public:
    LANClient(asio::io_context& io_context, const std::string& server_ip, int port, const std::string& server_code);
    void connect(); // Connects to the server

private:
    asio::ip::tcp::socket socket_;
    std::string server_code_;
};

void sendDataToServer(int selectedCharacter, int xpos2D, int ypos2D);