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
    client_data.clear();
    try {
        std::cout << "Waiting to receive data..." << std::endl;
        asio::streambuf buf;
        asio::error_code ec;

        // Read until "end" string
        asio::read_until(socket_, buf, "end", ec);
        if (!ec) {
            std::cout << "Data received." << std::endl;
            std::istream is(&buf);
            std::string data((std::istreambuf_iterator<char>(is)), std::istreambuf_iterator<char>());
            data = data.substr(0, data.find("end")); // Remove the "end" string
            std::cout << "Raw received data: " << data << std::endl;

            // Parse the received data
            std::lock_guard<std::mutex> lock(data_mutex);
            std::istringstream ss(data);
            std::string line;
            while (std::getline(ss, line)) {
                std::istringstream lineStream(line);
                std::string id, character, xpos2D, ypos2D, xpos3D, ypos3D, zpos3D, room;
                if (std::getline(lineStream, id, ',') &&
                    std::getline(lineStream, character, ',') &&
                    std::getline(lineStream, xpos2D, ',') &&
                    std::getline(lineStream, ypos2D, ',') &&
                    std::getline(lineStream, xpos3D, ',') &&
                    std::getline(lineStream, ypos3D, ',') &&
                    std::getline(lineStream, zpos3D, ',') &&
                    std::getline(lineStream, room, ',')) {
                    if(id != client->id) 
                    {
                    ClientData client;
                    client.id = id;
                    client.character = std::stoi(character);
                    client.xpos2D = std::stoi(xpos2D);
                    client.ypos2D = std::stoi(ypos2D);
                    client.xpos3D = std::stof(xpos3D);
                    client.ypos3D = std::stof(ypos3D);
                    client.zpos3D = std::stof(zpos3D);
                    client.room = std::stoi(room);
                    client_data.push_back(client);
                    }
                }
            }
        } else {
            std::cerr << "Receive error: " << ec.message() << std::endl;
        }
    } catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    std::cout << "Function end." << std::endl;
}

void sendDataToServer(int selectedCharacter, int xpos2D, int ypos2D, int room, float xpos3D, float ypos3D, float zpos3D) {
    try {
        // Construct the JSON object
        nlohmann::json jsonMessage;
        jsonMessage["selectedCharacter"] = selectedCharacter;
        jsonMessage["xpos2D"] = xpos2D;
        jsonMessage["ypos2D"] = ypos2D;
        jsonMessage["xpos3D"] = xpos3D;
        jsonMessage["ypos3D"] = ypos3D;
        jsonMessage["zpos3D"] = zpos3D;
        jsonMessage["room"] = room;
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
