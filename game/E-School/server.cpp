#include "server.h"
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
    asio::write(socket_, asio::buffer(server_code_), ec);
    if (!ec) {
        std::cout << "Sent server code: " << server_code_ << std::endl;
    }
    else {
        std::cerr << "Send error: " << ec.message() << std::endl;
    }
}

void sendDataToServer(int selectedCharacter, int xpos2D, int ypos2D) {
    try {
        // Define the server IP and port
        const std::string server_ip = "192.168.1.6"; // Replace with your server's IP
        const int server_port = 12345;

        // Create an asio context and socket
        asio::io_context io_context;
        tcp::socket socket(io_context);

        // Connect to the server
        tcp::endpoint endpoint(asio::ip::make_address(server_ip), server_port);
        socket.connect(endpoint);

        // Construct the JSON object
        std::string message = std::to_string(selectedCharacter) + "," +
            std::to_string(xpos2D) + "," +
            std::to_string(ypos2D);

        // Send the serialized string
        asio::write(socket, asio::buffer(message));
        std::cout << "Data sent to server: " << message << std::endl;

    }
    catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}
