#include "server.h"
using asio::ip::tcp;

LANServer::LANServer(asio::io_context& io_context, int port)
    : acceptor_(io_context, tcp::endpoint(tcp::v4(), port)) {
    // Generate a random 4-digit server code using raylib
    int random_number = GetRandomValue(1000, 9999);
    server_code_ = std::to_string(random_number);
}

void LANServer::start() {
    std::cout << "Server started. Code: " << server_code_ << std::endl;

    acceptor_.async_accept([this](asio::error_code ec, tcp::socket socket) {
        handleAccept(std::move(socket), ec);
        });
}

std::string LANServer::getServerCode() const {
    return server_code_;
}

void LANServer::handleAccept(tcp::socket socket, const asio::error_code& ec) {
    if (!ec) {
        std::cout << "Client connected from: " << socket.remote_endpoint() << std::endl;

    }
    else {
        std::cerr << "Accept error: " << ec.message() << std::endl;
    }

    start();
}

LANClient::LANClient(asio::io_context& io_context, const std::string& server_ip, int port, const std::string& server_code)
    : socket_(io_context), server_code_(server_code) {
    tcp::endpoint server_endpoint(asio::ip::make_address(server_ip), port);
    asio::error_code ec;

    socket_.connect(server_endpoint, ec);
    if (!ec) {
        std::cout << "Connected to server!" << std::endl;
    }
    else {
        std::cerr << "Connection error: " << ec.message() << std::endl;
    }
}

void LANClient::connect() {
    asio::error_code ec;
    asio::write(socket_, asio::buffer(server_code_), ec);
    if (!ec) {
        std::cout << "Sent server code: " << server_code_ << std::endl;
    }
    else {
        std::cerr << "Send error: " << ec.message() << std::endl;
    }
}