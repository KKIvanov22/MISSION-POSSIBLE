#pragma once
#include "pch.h"


class LANClient {
public:
    LANClient(asio::io_context& io_context, const std::string& server_ip, int port, const std::string& server_code);
    void connect(); // Connects to the server
    void receiveData(); // Receives data from the server

    asio::ip::tcp::socket socket_;
    std::string server_code_;
    std::string id;
    std::map<int, nlohmann::json> client_data; // Map to store client data
    std::mutex data_mutex; // Mutex to protect access to client data
};

void sendDataToServer(int selectedCharacter, int xpos2D, int ypos2D);