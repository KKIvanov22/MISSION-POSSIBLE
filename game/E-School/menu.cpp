#include "menu.h"
#include "map.h"
#include "rules.h"
#include "characterSelect.h"
#include "gradeBook.h"

asio::io_context io_context;
LANClient* client = nullptr;
std::thread io_thread;

void menu() {
	// Set mouse cursor
	SetMouseCursor(MOUSE_CURSOR_DEFAULT);

	// Define screen dimensions
	const int screenWidth = 1920;
	const int screenHeight = 975;

	// Initialize variables
	int character = 2;
	Texture2D studentFront = LoadTexture("Textures/studentFrontMenu.png");
	Texture2D studentGirl = LoadTexture("Textures/girlSprite.png");
	const Rectangle girlPos = { 80,160,60,80 };

	// Define button rectangles
	const Rectangle startButton = { (screenWidth / 2) - 400, (screenHeight / 2) + 50, 200, 50 };
	const Rectangle rulesButton = { (screenWidth / 2) - 170, (screenHeight / 2) + 50, 200, 50 };
	const Rectangle exitButton = { (screenWidth / 2) + 60, (screenHeight / 2) + 50, 200, 50 };
	const Rectangle gradesButton = { (screenWidth / 2) + 290, (screenHeight / 2) + 50, 200, 50 };

	// Define positions and bounds
	const  Vector2 screenPos[2] = {
		{(screenWidth / 2) - 340, (screenHeight / 2) - 30 },
		{(screenWidth / 2) - 680, (screenHeight / 2) - 30 }
	};

	const Rectangle bounds[2] = {
		{(screenWidth / 2) - 720, (screenHeight / 2) - 30,150,325},
		{ (screenWidth / 2) - 250, (screenHeight / 2) - 30,125,325}
	};

	// Initialize animation variables and textures
	bool animationShown = false;
	bool showParticle = false;
	Texture2D tubes = LoadTexture("Textures/tubes.png");
	Texture2D logo = LoadTexture("../images/logoResized.png");
	Texture2D skeleton = LoadTexture("Textures/skeleton.png");
	Texture2D book = LoadTexture("Textures/book.png");
	Texture2D molecual = LoadTexture("Textures/molecual.png");
	Texture2D globus = LoadTexture("Textures/globus.png");
	Texture2D calculator = LoadTexture("Textures/calculator.png");
	Texture2D bus = LoadTexture("Textures/bus.png");

	// Initialize animation positions
	int tubesX = 0;
	int skeletonX = 0;
	int tubesY = 0;
	int molecualX = 0;
	int molecualY = 0;
	int bookX = 0;
	int bookY = 0;
	int globusX = 0;
	int globusY = 0;
	int calculatorX = 0;
	int calculatorY = 0;
	int busX = 0;
	int doneAnimationCount = 0;
	int particleShow = 0;
	int i = 0;
	int particleCount = 1;
	std::vector<Particle> particles;

	std::thread io_thread; 
	std::string server_code;
	int port = 12345;

	std::string serverCode;
	bool showServerCodeInput = false;
	const Rectangle serverCodeBox = { (screenWidth / 2) - 80, (screenHeight / 2) + 125, 200, 50 };

	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(GRAY);
		DrawTexture(logo, 500, 250, WHITE);
		if (animationShown == 0)
		{

			DrawTexture(tubes, 10 + tubesX, 100 + tubesY, WHITE);
			DrawTexture(calculator, 800 - calculatorX, 710 - calculatorY, WHITE);
			DrawTexture(globus, 1450 - globusX, 400 - globusY, WHITE);
			DrawTexture(skeleton, 100 + skeletonX, 100, WHITE);
			DrawTexture(book, 100 + bookX, 100 + bookY, WHITE);
			DrawTexture(molecual, 1200 - molecualX, 100 + molecualY, WHITE);
			DrawTexture(bus, 1400 - busX, 550, WHITE);
			skeletonX += 10;
			tubesY += 10;
			tubesX += 10;
			molecualX += 10;
			molecualY += 10;
			bookX += 10;
			bookY += 10;
			globusX += 10;
			globusY += 10;
			calculatorX += 10;
			calculatorY += 10;
			busX += 10;
			if (10 + bookX >= 1400)
			{
				bookX = 1400;
				doneAnimationCount++;
			}
			if (10 + bookY >= 450)
			{
				bookY = 450;
				doneAnimationCount++;
			}
			if (10 + skeletonX >= 1400)
			{
				skeletonX = 1400;
				doneAnimationCount++;

			}
			if (10 + tubesX >= 1000)
			{
				tubesX = 1090;
				doneAnimationCount++;

			}
			if (100 + tubesY >= 700)
			{
				tubesY = 710;
				doneAnimationCount++;

			}
			if (10 + molecualY >= 550)
			{
				molecualY = 550;
				doneAnimationCount++;

			}
			if (10 + molecualX >= 700)
			{
				molecualX = 700;
				doneAnimationCount++;

			}
			if (globusY >= 390)
			{
				globusY = 390;
				doneAnimationCount++;

			}
			if (10 + globusX >= 600)
			{
				globusX = 600;
				doneAnimationCount++;

			}
			if (10 + calculatorX >= 700)
			{
				calculatorX = 700;
				doneAnimationCount++;

			}
			if (calculatorY >= 610)
			{
				calculatorY = 610;
				doneAnimationCount++;

			}
			if (busX >= 1300)
			{
				busX = 1300;
				doneAnimationCount++;

			}
			i++;
			if (i == 160)
			{
				animationShown = 1;
			}

		}
		else
		{
			DrawTexture(tubes, 10 + tubesX, 100 + tubesY, WHITE);
			DrawTexture(calculator, 800 - calculatorX, 710 - calculatorY, WHITE);
			DrawTexture(globus, 1450 - globusX, 400 - globusY, WHITE);
			DrawTexture(skeleton, 100 + skeletonX, 100, WHITE);
			DrawTexture(book, 100 + bookX, 100 + bookY, WHITE);
			DrawTexture(molecual, 1200 - molecualX, 100 + molecualY, WHITE);
			DrawTexture(bus, 1400 - busX, 550, WHITE);
			Vector2 mousePosition = GetMousePosition();
			

			DrawRectangleRounded(startButton, 10, 10, (CheckCollisionPointRec(mousePosition, startButton) ? PINK : LIGHTGRAY));
			DrawText("Start", (screenWidth / 2) - 335, (screenHeight / 2) + 62, 25, WHITE);
			if (CheckCollisionPointRec(mousePosition, startButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
				showParticle = 1;
			}

			DrawRectangleRounded(rulesButton, 10, 10, (CheckCollisionPointRec(mousePosition, rulesButton) ? PINK : LIGHTGRAY));
			DrawText("Connect", screenWidth / 2 - 100, screenHeight / 2 + 62, 25, WHITE);
				

			// Open server code input box when pressing 'Connect'
			if (CheckCollisionPointRec(GetMousePosition(), rulesButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
				showServerCodeInput = true;
			}

			// Server code input
			if (showServerCodeInput) {
				DrawRectangleRounded(serverCodeBox, 10, 10, LIGHTGRAY);
				DrawText(serverCode.c_str(), serverCodeBox.x + 25, serverCodeBox.y + 15, 20, BLACK);
				int key = GetCharPressed();
				while (key > 0) {
					if ((key >= 32) && (key <= 125) && (serverCode.length() < 4)) {
						serverCode += static_cast<char>(key);
					}
					key = GetCharPressed();
				}

				if (IsKeyPressed(KEY_BACKSPACE) && !serverCode.empty()) {
					serverCode.pop_back();
				}

				// Try to connect once the user enters the code and presses Enter
				if (IsKeyPressed(KEY_ENTER) && !client) {
					std::string server_ip = "127.0.0.1"; 
					client = new LANClient(io_context, server_ip, 12345, serverCode);
					client->connect();

					if (!io_thread.joinable()) {
						io_thread = std::thread([&]() { io_context.run(); });
					}
					client->girl = LoadTexture("textures/girlBilboard.png");
					client->boy = LoadTexture("textures/studentFrontMenu.png");
					showServerCodeInput = false;
					serverCode.clear();
				}
			}

			// Check if mouse is hovering the specific button
			bool isMouseOverButtonExit = CheckCollisionPointRec(mousePosition, exitButton);
			// Set new colors for hovering the button and draw text
			DrawRectangleRounded(exitButton, 10, 10, (isMouseOverButtonExit ? PINK : LIGHTGRAY));
			DrawText("Grades", screenWidth / 2 + 120, screenHeight / 2 + 62, 25, WHITE);
			// Handle click with the mouse over button
			if (isMouseOverButtonExit && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
				gradeBook();
			}
			// Check if mouse is hovering the specific button
			bool isMouseOverButtonGrades = CheckCollisionPointRec(mousePosition, gradesButton);
			// Set new colors for hovering the button and draw text
			DrawRectangleRounded(gradesButton, 10, 10, (isMouseOverButtonGrades ? PINK : LIGHTGRAY));
			DrawText("Exit", screenWidth / 2 + 365, screenHeight / 2 + 62, 25, WHITE);
			// Handle click with the mouse over button
			if (isMouseOverButtonGrades && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
				
				break;
			}
		}
		if (showParticle == 1)
		{
			if (particleCount == 2)
			{
				for (int i = 0; i < 150; i++) {
					Particle particle;
					particle.position = { 810, 330 };
					particle.velocity = { static_cast<float>(GetRandomValue(-5, 5)), static_cast<float>(GetRandomValue(-5, 5)) };
					particle.color = PINK;
					particle.radius = GetRandomValue(1, 5);
					particles.push_back(particle);
				}
			}

			if (!particles.empty()) {
				for (auto it = particles.begin(); it != particles.end();) {
					it->position.x += it->velocity.x;
					it->position.y += it->velocity.y;


					DrawCircleV(it->position, it->radius, it->color);

					if (it->position.x < 0 || it->position.x > GetScreenWidth() ||
						it->position.y < 0 || it->position.y > GetScreenHeight()) {
						it = particles.erase(it);
					}
					else {
						++it;
					}
				}
			}
			if (particleCount == 2)
			{
				for (int i = 0; i < 150; i++) {
					Particle particle;
					particle.position = { 810, 330 };
					particle.velocity = { static_cast<float>(GetRandomValue(-5, 5)), static_cast<float>(GetRandomValue(-5, 5)) };
					particle.color = PURPLE;
					particle.radius = GetRandomValue(1, 5);
					particles.push_back(particle);
				}
			}


			if (!particles.empty()) {
				for (auto it = particles.begin(); it != particles.end();) {
					it->position.x += it->velocity.x;
					it->position.y += it->velocity.y;


					DrawCircleV(it->position, it->radius, it->color);

					if (it->position.x < 0 || it->position.x > GetScreenWidth() ||
						it->position.y < 0 || it->position.y > GetScreenHeight()) {
						it = particles.erase(it);
					}
					else {
						++it;
					}
				}
			}
			particleCount++;

			particleShow++;

		}
		if (particleShow == 100)
		{
			select();
		}
		EndDrawing();
		
	}
	SetExitKey(KEY_APOSTROPHE);
	EnableCursor();
}