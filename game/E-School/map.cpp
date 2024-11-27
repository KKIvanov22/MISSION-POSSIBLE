#include "map.h"
#include "rooms.h"
#include "menu.h"
#include "server.h"
void initMap(int character)
{
	EndMode3D();
	const int screenWidth = 1920;
	const int screenHeight = 975;
	float studentX = 200;
	float studentY = 475;
	Rectangle subjects[10] =
	{
		{1620, 500, 300, 220}, //prg
		{310, 0, 300, 270}, //bio
		{670, 720, 300, 270}, //physics
		{1050, 0, 270, 270}, //chemistry
		{1400, 720, 300, 270}, //History
		{310, 720, 300, 270}, //literature
		{670, 0, 300, 270}, //english
		{1400, 0, 270, 270}, //math
		{1050, 720, 250, 270}, //Geography
		{1620, 260, 300, 220}, //pe

	};

	Texture2D map = LoadTexture("textures/map.png");
	Texture2D studentBack = LoadTexture("Textures/studentBack.png");
	Texture2D studentLeft = LoadTexture("Textures/studentLeft.png");
	Texture2D studentRight = LoadTexture("Textures/studentRight.png");
	Texture2D studentFront = LoadTexture("Textures/studentFront.png");
	Texture2D studentGirl = LoadTexture("Textures/girlSprite.png");

	Rectangle student = { studentX, studentY, 16, 50 };
	Rectangle spriteGirl[14] =
	{
		{0,0,200,200},
		{200,0,200,200},
		{400,0,200,200},
		{600,0,200,200},
		{0,200,200,200},
		{200,200,200,200},
		{400,200,200,200},
		{600,200,200,200},
		{0,400,200,200},
		{200,400,200,200},
		{400,400,200,200},
		{600,400,200,200},
		{0,620,200,200},
		{200,620,200,200},
	};


	int currentFrameGirl = 8;
	int boyFrame = 0;
	float frameCounter = 0;
	float frameTime = 0;
	float timeAccumulator = 0.0f;
	Rectangle currentFrameBoy;
	while (!WindowShouldClose())
	{
		float deltaTime = GetFrameTime();
		timeAccumulator += deltaTime;
		if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))
		{
			studentY -= 3.0f;
			student.y = studentY;
			currentFrameGirl = 11 + (int)(GetTime() * 5) % 2; // Animation for walking forward
			if (client != nullptr) sendDataToServer(character, static_cast<int>(studentX), static_cast<int>(studentY),0);

		}

		if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))
		{
			studentY += 3.0f;
			student.y = studentY;
			currentFrameGirl = 8 + (int)(GetTime() * 5) % 2; // Animation for walking back
			if (client != nullptr) sendDataToServer(character, static_cast<int>(studentX), static_cast<int>(studentY), 0);

		}

		if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))
		{
			studentX -= 3.0f;
			student.x = studentX;
			currentFrameGirl = 2 + (int)(GetTime() * 5) % 2; // Animation for walking left
			if (client != nullptr) sendDataToServer(character, static_cast<int>(studentX), static_cast<int>(studentY), 0);

		}

		if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT))
		{
			studentX += 3.0f;
			student.x = studentX;
			currentFrameGirl = 4 + (int)(GetTime() * 5) % 2; // Animation for walking right
			if (client != nullptr) sendDataToServer(character, static_cast<int>(studentX), static_cast<int>(studentY), 0);

		}
		if (timeAccumulator >= 0.5f)
		{
			if (client != nullptr) client->receiveData();
			timeAccumulator = 0.0f;
		}

		BeginDrawing();
		ClearBackground(WHITE);
		DrawTexture(map, 0, 0, WHITE);
		if (client != nullptr) {
			for (size_t i = 0; i < client->client_data.size(); i++)
			{
				if (client->client_data[i].character == 1)
				{
					DrawTextureRec(studentGirl, spriteGirl[currentFrameGirl], { static_cast<float>(client->client_data[i].xpos2D), static_cast<float>(client->client_data[i].ypos2D) }, WHITE);
				}
				else if (client->client_data[i].character == 2)
				{
					DrawTextureRec(studentGirl, spriteGirl[currentFrameGirl], { static_cast<float>(client->client_data[i].xpos2D), static_cast<float>(client->client_data[i].ypos2D) }, WHITE);
				}
			}
		}

		if (IsKeyPressed(KEY_G))
		{
			gradeBook();
		}
		DrawText("Programming", 1620, 580, 30, BLACK);

		DrawText("Biology", 400, 120, 30, BLACK);

		DrawText("Physics", 770, 800, 30, BLACK);

		DrawText("Chemistry", 1090, 140, 30, BLACK);

		DrawText("History", 1500, 840, 30, BLACK);

		DrawText("Literature", 730, 120, 30, BLACK);

		DrawText("English", 330, 720, 30, BLACK);

		DrawText("Math", 1500, 90, 30, BLACK);

		DrawText("Geography", 1090, 840, 30, BLACK);

		DrawText("P.E.", 1620, 300, 30, BLACK);


		if (character == 1)
		{
			if ((IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) && (IsKeyUp(KEY_A) && IsKeyUp(KEY_D)))
			{
				currentFrameBoy = { (float)boyFrame * studentBack.width / 3, 0, (float)studentBack.width / 3, (float)studentBack.height };
				DrawTextureRec(studentBack, currentFrameBoy, { studentX, studentY }, WHITE);
				frameCounter += 0.4f;

				if (frameCounter >= 7)
				{
					frameCounter = 0;
					boyFrame++;
					if (boyFrame >= 3)
					{
						boyFrame = 0;
					}
				}
			}

			if ((IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) && (IsKeyUp(KEY_A) && IsKeyUp(KEY_D)))
			{
				currentFrameBoy = { (float)boyFrame * studentFront.width / 3, 0, (float)studentFront.width / 3, (float)studentFront.height };
				DrawTextureRec(studentFront, currentFrameBoy, { studentX, studentY }, WHITE);
				frameCounter += 0.4f;

				if (frameCounter >= 7)
				{
					frameCounter = 0;
					boyFrame++;
					if (boyFrame >= 3)
					{
						boyFrame = 0;
					}
				}
			}

			if ((IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) || (IsKeyDown(KEY_A) && IsKeyDown(KEY_W)) || (IsKeyDown(KEY_A) && IsKeyDown(KEY_S)))
			{
				currentFrameBoy = { (float)boyFrame * studentLeft.width / 2, 0, (float)studentLeft.width / 2, (float)studentLeft.height };
				DrawTextureRec(studentLeft, currentFrameBoy, { studentX, studentY }, WHITE);
				frameCounter += 0.4f;

				if (frameCounter >= 7)
				{
					frameCounter = 0;
					boyFrame++;
					if (boyFrame >= 2)
					{
						boyFrame = 0;
					}
				}
			}

			if ((IsKeyDown(KEY_D)) || (IsKeyDown(KEY_D) && (IsKeyDown(KEY_W)) || (IsKeyDown(KEY_D)) && IsKeyDown(KEY_S)))
			{
				currentFrameBoy = { (float)boyFrame * studentRight.width / 2, 0, (float)studentRight.width / 2, (float)studentRight.height };
				DrawTextureRec(studentRight, currentFrameBoy, { studentX, studentY }, WHITE);
				frameCounter += 0.4f;

				if (frameCounter >= 7)
				{
					frameCounter = 0;
					boyFrame++;
					if (boyFrame >= 2)
					{
						boyFrame = 0;
					}
				}
			}

			if (IsKeyUp(KEY_W) && IsKeyUp(KEY_S) && IsKeyUp(KEY_A) && IsKeyUp(KEY_D))
			{
				currentFrameBoy = { (float)boyFrame * studentBack.width / 3, 0, (float)studentBack.width / 3, (float)studentBack.height };
				DrawTextureRec(studentBack, currentFrameBoy, { studentX, studentY }, WHITE);
			}

		}
		else if (character == 2)
		{
			DrawTextureRec(studentGirl, spriteGirl[currentFrameGirl], { studentX,studentY }, WHITE);
			frameCounter += 0.5f;

			if (frameCounter >= 5)
			{
				frameCounter = 0;
				currentFrameGirl++;

				if (currentFrameGirl > 13)
				{
					currentFrameGirl = 0;
				}
			}
		}

		if (CheckCollisionRecs(student, subjects[0]))
		{
			DrawText("Press ENTER to enter.", 435, 700, 30, BLACK);
			if (IsKeyDown(KEY_ENTER))
			{
				programming();
				if (client != nullptr) sendDataToServer(character, static_cast<int>(studentX), static_cast<int>(studentY), 1);

			}
		}

		if (CheckCollisionRecs(student, subjects[1]))
		{
			DrawText("Press ENTER to enter.", 435, 700, 30, BLACK);
			if (IsKeyDown(KEY_ENTER))
			{
				biology();
				if (client != nullptr) sendDataToServer(character, static_cast<int>(studentX), static_cast<int>(studentY), 2);

			}
		}

		if (CheckCollisionRecs(student, subjects[2]))
		{
			DrawText("Press ENTER to enter.", 435, 700, 30, BLACK);
			if (IsKeyDown(KEY_ENTER))
			{
				physics();
				if (client != nullptr) sendDataToServer(character, static_cast<int>(studentX), static_cast<int>(studentY), 3);

			}
		}

		if (CheckCollisionRecs(student, subjects[3]))
		{
			DrawText("Press ENTER to enter.", 435, 700, 30, BLACK);
			if (IsKeyDown(KEY_ENTER))
			{
				chemistry();
				if (client != nullptr) sendDataToServer(character, static_cast<int>(studentX), static_cast<int>(studentY), 4);

			}
		}

		if (CheckCollisionRecs(student, subjects[4]))
		{
			DrawText("Press ENTER to enter.", 435, 700, 30, BLACK);
			if (IsKeyDown(KEY_ENTER))
			{
				history();
				if (client != nullptr) sendDataToServer(character, static_cast<int>(studentX), static_cast<int>(studentY), 5);

			}
		}

		if (CheckCollisionRecs(student, subjects[5]))
		{
			DrawText("Press ENTER to enter.", 435, 700, 30, BLACK);
			if (IsKeyDown(KEY_ENTER))
			{
				english();
				if (client != nullptr) sendDataToServer(character, static_cast<int>(studentX), static_cast<int>(studentY), 6);

			}
		}

		if (CheckCollisionRecs(student, subjects[6]))
		{
			DrawText("Press ENTER to enter.", 435, 700, 30, BLACK);
			if (IsKeyDown(KEY_ENTER))
			{
				literature();
				if (client != nullptr) sendDataToServer(character, static_cast<int>(studentX), static_cast<int>(studentY), 7);

			}
		}

		if (CheckCollisionRecs(student, subjects[7]))
		{
			DrawText("Press ENTER to enter.", 435, 700, 30, BLACK);
			if (IsKeyDown(KEY_ENTER))
			{
				if (client != nullptr) sendDataToServer(character, static_cast<int>(studentX), static_cast<int>(studentY), 8);
				maths();
			}
		}

		if (CheckCollisionRecs(student, subjects[8]))
		{
			DrawText("Press ENTER to enter.", 435, 700, 30, BLACK);
			if (IsKeyDown(KEY_ENTER))
			{
				geography();
				if (client != nullptr) sendDataToServer(character, static_cast<int>(studentX), static_cast<int>(studentY), 9);

			}
		}

		if (CheckCollisionRecs(student, subjects[9]))
		{
			DrawText("Press ENTER to enter.", 435, 500, 30, BLACK);
			if (IsKeyDown(KEY_ENTER))
			{
				physyicaEducation();
				if (client != nullptr) sendDataToServer(character, static_cast<int>(studentX), static_cast<int>(studentY), 10);

			}
		}


		if (IsKeyPressed(KEY_ESCAPE))
		{
			EnableCursor();
			menu();
		}
		EndDrawing();
		if (studentX < 20)
		{
			studentX = 20;
		}
		if (studentX > 1880)
		{
			studentX = 1880;
		}
		if (studentY < 20)
		{
			studentY = 20;
		}
		if (studentY > 950)
		{
			studentY = 950;
		}
		currentFrameGirl = 8;
		if (IsKeyPressed(KEY_F))
		{
			cout << "Character: (" << student.x << ", " << student.y << "), width: " << student.width << ", height: " << student.height << endl;

			for (int i = 0; i < 9; ++i) {
				cout << "Subject " << i << ": (" << subjects[i].x << ", " << subjects[i].y << "), width: " << subjects[i].width << ", height: " << subjects[i].height << endl;
			}

			cout << "Collision with subject 0: " << (CheckCollisionRecs(student, subjects[0]) ? "Yes" : "No") << endl;
		}

	}
	SetExitKey(KEY_APOSTROPHE);
	UnloadTexture(map);
	UnloadTexture(studentBack);
	UnloadTexture(studentLeft);
	UnloadTexture(studentRight);
	UnloadTexture(studentFront);
	UnloadTexture(studentGirl);


}