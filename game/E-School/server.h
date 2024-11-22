#pragma once
#include "pch.h"

class LANServer {
public:
    LANServer(asio::io_context& io_context, int port);
    void start(); // Starts the server
    std::string getServerCode() const; // Returns the server's 4-digit code

private:
    asio::ip::tcp::acceptor acceptor_;
    std::string server_code_;

    void handleAccept(asio::ip::tcp::socket socket, const asio::error_code& ec);
};

class LANClient {
public:
    LANClient(asio::io_context& io_context, const std::string& server_ip, int port, const std::string& server_code);
    void connect(); // Connects to the server

private:
    asio::ip::tcp::socket socket_;
    std::string server_code_;
};