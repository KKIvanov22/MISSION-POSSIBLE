#include "server.h"
#include "menu.h"
using asio::ip::tcp;
using json = nlohmann::json;

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
    asio::write(socket_, asio::buffer(server_code_ + '\n'), ec);
    if (!ec) {
        std::cout << "Sent server code: " << server_code_ << std::endl;

        // Read the server's response
        asio::streambuf buf;
        asio::read_until(socket_, buf, '\n', ec);
        if (!ec) {
            std::istream is(&buf);
            std::string response;
            std::getline(is, response);
            std::cout << "Server response: " << response << std::endl;

            // Read the client ID from the server
            asio::read_until(socket_, buf, '\n', ec);
            if (!ec) {
                std::getline(is, id);
                std::cout << "Received client ID: " << id << std::endl;
            }
            else {
                std::cerr << "Receive error: " << ec.message() << std::endl;
            }
        }
        else {
            std::cerr << "Receive error: " << ec.message() << std::endl;
        }
    }
    else {
        std::cerr << "Send error: " << ec.message() << std::endl;
    }
}

void LANClient::receiveData() {
    try {
        asio::streambuf buf;
        asio::error_code ec;
        asio::read_until(socket_, buf, '\n', ec);
        if (!ec) {
            std::istream is(&buf);
            std::string data;
            std::getline(is, data);
            std::cout << "Received data: " << data << std::endl;

            // Parse the received JSON data
            json receivedData = json::parse(data);
            std::lock_guard<std::mutex> lock(data_mutex);
            client_data = receivedData;
        }
        else {
            std::cerr << "Receive error: " << ec.message() << std::endl;
        }
    }
    catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void sendDataToServer(int selectedCharacter, int xpos2D, int ypos2D) {
    try {
        // Construct the JSON object
        nlohmann::json jsonMessage;
        jsonMessage["selectedCharacter"] = selectedCharacter;
        jsonMessage["xpos2D"] = xpos2D;
        jsonMessage["ypos2D"] = ypos2D;
        jsonMessage["id"] = client->id; 
        std::string message = jsonMessage.dump();
     
        // Send the serialized string
        asio::write(client->socket_, asio::buffer(message + '\n'));
        std::cout << "Data sent to server: " << message << std::endl;

    }
    catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}
