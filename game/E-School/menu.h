#pragma once

#include "pch.h"
#include "server.h"

struct Particle {
    Vector2 position;
    Vector2 velocity;
    Color color;
    int radius;
};
extern asio::io_context io_context; 
extern LANServer* server;
extern LANClient* client;

void menu();



