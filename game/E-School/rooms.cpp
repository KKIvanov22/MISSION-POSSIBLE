#include "rooms.h"
#include "textbooks.h"
#include "exams.h"
#include "map.h"
#include "whiteboard.h"
#include "server.h"
#include "menu.h"


void drawClients3D(const std::vector<ClientData>& client_data, int room,Camera camera) {
    
    for (const auto& clientC : client_data) {
		if (clientC.room == room) {
            if (clientC.character == 2)
            {
                DrawBillboard(camera, client->girl, { clientC.xpos3D, 1.f, clientC.zpos3D }, 3.f, GRAY);
            }
            else if (clientC.character == 1)
            {
                DrawBillboard(camera, client->boy, { clientC.xpos3D, 1.f, clientC.zpos3D }, 3.f, GRAY);
            }
		}
       
    }
}

void drawCoordinates(Camera& camera)
{
    string positionString = "Camera Position: (" +
        to_string(camera.position.x) + ", " +
        to_string(camera.position.y) + ", " +
        to_string(camera.position.z) + ")";
    DrawText(positionString.c_str(), 10, 10, 20, GOLD);
}

bool checkCollision(const BoundingBox& box1, const BoundingBox& box2) {
    return (box1.min.x <= box2.max.x && box1.max.x >= box2.min.x) &&
        (box1.min.y <= box2.max.y && box1.max.y >= box2.min.y) &&
        (box1.min.z <= box2.max.z && box1.max.z >= box2.min.z);
}

void collisions(Camera& camera, Vector3 previousCameraPosition, BoundingBox cameraBox, BoundingBox wallBox)
{

    if (checkCollision(cameraBox, wallBox)) {

    }
    else {
        camera.position = previousCameraPosition;
    }
}

bool collisionsModelToModel(Vector3& ballPosition, Vector3 previousBallPosition, BoundingBox ballBox, BoundingBox wallBox)
{

    if (checkCollision(ballBox, wallBox)) {
        return 0;
    }
    else {
        return 1;
    }

}

void collisionModels(Camera& camera, Vector3 previousCameraPosition, BoundingBox cameraBox, BoundingBox wallBox)
{

    if (!checkCollision(cameraBox, wallBox)) {

    }
    else {
        camera.position = previousCameraPosition;
    }
}

void kickBall(Camera camera, Vector3& ballPosition, float& potentialKickX) {
    float distanceToBall = distanceCalc(camera.position, ballPosition);
    const float kickThreshold = 3.5f;

    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && distanceToBall <= kickThreshold) {
        potentialKickX += GetFrameTime() * 10.0f;
    }

    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && distanceToBall <= kickThreshold) {
        float kickForce = potentialKickX;

        Vector3 direction = Vector3Subtract(ballPosition, camera.position);
        direction = Vector3Normalize(direction);

        ballPosition.x += direction.x * kickForce;
        ballPosition.z += direction.z * kickForce;

        potentialKickX = 0.0f;
    }
}

void drawFurnitures(Model chair, Model desk, Model deskChair, Model studentDesk, Model board, Model laptop, Model book, Camera& camera, Vector3 previousCameraPosition)
{
    BoundingBox cameraBox;
    BoundingBox deskBox;
    cameraBox.min = camera.position;
    cameraBox.max = camera.position;
    cameraBox.min.x -= 0.1f;
    cameraBox.min.y -= 0.1f;
    cameraBox.min.z -= 0.1f;
    cameraBox.max.x += 0.3f;
    cameraBox.max.y += 0.5f;
    cameraBox.max.z += 0.5f;
    int change = 0;

    for (int i = 0; i <= 16; i += 4)
    {
        for (int j = 0; j <= 8; j += 4) {
            DrawModel(chair, { -7.8f + i,0.1f,7.0f - j }, 0.03, WHITE);
            DrawModel(studentDesk, { -7.8f + i,-0.1f,6.5f - j }, 0.015, BROWN);
            DrawModel(book, { -7.8f + i,1.5f,6.4f - j }, 0.04, WHITE);
        }

    }
    DrawModel(desk, { 0.0f,0.2f,-7.0f }, 0.015, DARKGRAY);
    DrawModel(laptop, { -0.5f,1.64f,-6.5f }, 4.0, GRAY);
    DrawModel(deskChair, { 0.0f,0.2f,-8.0f }, 0.6, BLACK);
    DrawModel(board, { 0.0f, 2.5f, -9.4f }, 1.0, WHITE);

    BoundingBox studentDeskBox[16];
    for (int i = 0; i < 3; i++)
    {
        studentDeskBox[i].min = { 7.25f, 0.2f, -2.0f + change };
        studentDeskBox[i].max = { 8.9f,2.0f,0.0f + change };
        change += 4;
    }
    change = 0;
    for (int i = 3; i < 6; i++)
    {
        studentDeskBox[i].min = { 3.25f , 0.2f, -2.0f + change };
        studentDeskBox[i].max = { 4.8f ,2.0f,0.0f + change };
        change += 4;
    }
    change = 0;
    for (int i = 6; i < 9; i++)
    {
        studentDeskBox[i].min = { -0.4f ,0.2f,-2.0f + change };
        studentDeskBox[i].max = { 1.1f , 2.0f, 0.0f + change };
        change += 4;
    }
    change = 0;
    for (int i = 9; i < 12; i++)
    {
        studentDeskBox[i].min = { -4.5f ,0.2f,-2.0f + change };
        studentDeskBox[i].max = { -3.0f , 2.0f, 0.0f + change };
        change += 4;
    }
    change = 0;
    for (int i = 12; i < 15; i++)
    {
        studentDeskBox[i].min = { -8.5f ,0.2f,-2.0f + change };
        studentDeskBox[i].max = { -7.0f , 2.0f, 0.0f + change };
        change += 4;
    }
    for (int i = 0; i < 15; i++)
    {
        collisionModels(camera, previousCameraPosition, cameraBox, studentDeskBox[i]);
    }
    deskBox.min = { -1.0f ,0.2f,-8.5f };
    deskBox.max = { 1.0f ,5.0f,-11.0f };
    collisionModels(camera, previousCameraPosition, cameraBox, deskBox);

}
void drawTubes(Model tubes)
{
    for (int i = 0; i <= 16; i += 4)
    {
        for (int j = 0; j <= 8; j += 4) {
            DrawModel(tubes, { -8.0f + i,1.47f,6.8f - j }, 0.15, WHITE);
        }

    }
}
void drawFurnituresProgramming(Model desk, Model deskChair, Model chair, Model chairOther, Model workstation, Model computer, Model computerRotated, Camera& camera, Vector3 previousCameraPosition)
{
    BoundingBox cameraBox;
    cameraBox.min = camera.position;
    cameraBox.max = camera.position;
    cameraBox.min.x -= 0.1f;
    cameraBox.min.y -= 0.1f;
    cameraBox.min.z -= 0.1f;
    cameraBox.max.x += 0.3f;
    cameraBox.max.y += 0.5f;
    cameraBox.max.z += 0.5f;

    DrawModel(deskChair, { 20.5f,0.2f,7.0f }, 0.6, BLACK);
    DrawModel(desk, { 19.5f,0.2f,7.0f }, 0.015, GRAY);

    for (int i = 0; i <= 17.5; i += 2.5)
    {
        DrawModel(chair, { 15.0f - i,0.1f,7.0f }, 0.03, WHITE);
        DrawModel(workstation, { 15.0f - i,0.1f,7.0f }, 1.5, GRAY);
        DrawModel(computer, { 15.8f - i,1.2f,6.0f }, 1.5, BLACK);
    }

    for (int i = 0; i <= 17.5; i += 2.5)
    {
        DrawModel(chair, { 15.0f - i,0.1f,-4.5f }, 0.03, WHITE);
        DrawModel(workstation, { 15.0f - i,0.1f,-4.5f }, 1.5, GRAY);
        DrawModel(computer, { 16.0f - i,1.2f,-5.2f }, 1.5, BLACK);
    }
    for (int i = 0; i <= 17.5; i += 2.5)
    {
        DrawModel(chairOther, { 15.0f - i,0.1f,4.0f }, 0.03, WHITE);
        DrawModel(computerRotated, { 15.3f - i,1.2f,4.0f }, 1.5, BLACK);
    }

    for (int i = 0; i <= 17.5; i += 2.5)
    {
        DrawModel(chairOther, { 15.0f - i,0.1f,-7.5f }, 0.03, WHITE);
        DrawModel(computerRotated, { 15.5f - i,1.2f,-7.8f }, 1.5, BLACK);
    }
    BoundingBox chairBox[2];
    chairBox[0].min = { -1.8f, 0.0f, -8.3f };
    chairBox[0].max = { 16.0f, 2.0f, -3.6f };
    chairBox[1].min = { -3.7f, 0.0f, 3.3f };
    chairBox[1].max = { 16.7f, 2.0f, 7.7f };
    collisionModels(camera, previousCameraPosition, cameraBox, chairBox[0]);
    collisionModels(camera, previousCameraPosition, cameraBox, chairBox[1]);

}

void shootBasketball(Camera camera, Vector3& ballPosition, float& potentialKickX, float& potentialKickY, int& footballPoints) {
    Vector3 playerPosition = camera.position; // Assuming the player's position is the camera position

    // Calculate distance between player and basketball hoop
    Vector3 basketballHoopPosition = { -12.0f, 4.0f, 0.0f }; // Assuming basketball hoop position
    float distanceToHoop = distanceCalc(playerPosition, basketballHoopPosition);

    // Check if player is within a certain distance of the ball and within bounds
    if (distanceToHoop <= 3.5f && playerPosition.x <= -12.0f && playerPosition.y >= 4.0f && IsMouseButtonDown(MOUSE_RIGHT_BUTTON)) {
        // Increase potential kick power while right mouse button is held down
        potentialKickX += 0.1f;
        potentialKickY += 0.1f;
    }
    else if (IsMouseButtonReleased(MOUSE_RIGHT_BUTTON)) {
        // Shoot the basketball when right mouse button is released
        if (distanceToHoop <= 3.5f && playerPosition.x <= -12.0f && playerPosition.y >= 4.0f) { // Check if player is within shooting range and bounds
            // Calculate shooting direction based on camera orientation
            Vector3 direction = Vector3Normalize(Vector3Subtract(basketballHoopPosition, playerPosition));
            direction.x *= potentialKickX;
            direction.y *= potentialKickY;

            // Update ball position
            ballPosition = Vector3Add(playerPosition, direction);

            // Reset potential kick power
            potentialKickX = 0.0f;
            potentialKickY = 0.0f;

            // Increment points
            footballPoints++;
        }
        else {
            // Reset potential kick power if player is out of range or out of bounds
            potentialKickX = 0.0f;
            potentialKickY = 0.0f;
        }
    }
}

void maths()
{
    Model chair = LoadModel("objects/chair.obj");
    Model desk = LoadModel("objects/desk.obj");
    Model deskChair = LoadModel("objects/deskChair.obj");
    Model studentDesk = LoadModel("objects/studentDesk.obj");
    Model board = LoadModel("objects/board.obj");
    Model laptop = LoadModel("objects/laptop.obj");
    Model book = LoadModel("objects/book.obj");

    Texture2D wood = LoadTexture("Textures/wood.png");
    SetMaterialTexture(&studentDesk.materials[0], MATERIAL_MAP_DIFFUSE, wood);

    Texture2D mathBook = LoadTexture("Textures/mathBook.png");
    SetMaterialTexture(&book.materials[0], MATERIAL_MAP_DIFFUSE, mathBook);

    const int screenWidth = 1920;
    const int screenHeight = 975;

    Camera camera = { 0 };
    camera.position = { 8.0f, 2.0f, -7.0f };   // Camera position
    camera.target = { 0.0f, 2.0f, 0.0f };      // Camera looking at point
    camera.up = { 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 60.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;             // Camera projection type

    int cameraMode = CAMERA_FIRST_PERSON;
    Camera staticCamera = { 0 };
    staticCamera.fovy = 60.0f;
    staticCamera.position = { 0.261261,2,-2.52176 };
    staticCamera.projection = 0;
    staticCamera.target = { 0.529181,2.7501,-10.0623 };
    staticCamera.up = { 0,1,0 };

    BoundingBox wallBox;
    wallBox.min = { -9.0f, 0.0f, -9.0f };
    wallBox.max = { 9.3f, 20.0f, 9.0f };

    BoundingBox cameraBox;
    cameraBox.min = camera.position;
    cameraBox.max = camera.position;
    cameraBox.min.x -= 0.1f;
    cameraBox.min.y -= 0.1f;
    cameraBox.min.z -= 0.1f;
    cameraBox.max.x += 0.3f;
    cameraBox.max.y += 0.5f;
    cameraBox.max.z += 0.5f;
    // Generates some random columns
    DisableCursor();                    // Limit cursor to relative movement inside the window

    SetTargetFPS(60);                   // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------
    float frameCounter = 0;
    float frameTime = 0;
    float timeAccumulator = 0.0f;

    Vector3 previousCameraPosition = camera.position;
    // Main game loop
    Texture2D draw = { 0 };
    while (!WindowShouldClose())
    {
        float deltaTime = GetFrameTime();
        timeAccumulator += deltaTime;
		if (timeAccumulator >= 0.2f)
		{
			timeAccumulator = 0.0f;
			frameCounter++;
            if (client != nullptr) client->receiveData();
		}
        previousCameraPosition = camera.position;
        UpdateCamera(&camera, cameraMode);
        BeginDrawing();

        ClearBackground(RAYWHITE);
        if (distanceCalc(camera.position, { 0.0f,0.2f,-7.0f }) <= 5.f && IsKeyPressed(KEY_F))
        {
            draw = whiteboard::whiteboard();
        }
        else if (IsKeyPressed(KEY_F))
        {
            EnableCursor();
            mathsExaminationAlert();
        }
        BeginMode3D(camera);
        DrawBillboard(staticCamera, draw, { 0.0f, 3.5f, -9.0f }, 2.0f, WHITE);
		if (IsKeyPressed(KEY_B))
		{
			cout << camera.fovy << endl << camera.position.x << "," << camera.position.y << "," << camera.position.z << endl << camera.projection << endl << camera.target.x << "," << camera.target.y << "," << camera.target.z << endl << camera.up.x << "," << camera.up.y << "," << camera.up.z << endl;
		}
        cameraBox.min = camera.position;
        cameraBox.max = camera.position;

        DrawPlane({ 0.0f, 0.0f, 0.0f }, { 21.0f, 21.0f }, LIGHTGRAY);
        DrawCube({ -10.0f, 2.5f, 0.0f }, 1.0f, 8.3f, 21.0f, BLUE);
        DrawCube({ 10.0f, 2.5f, 0.0f }, 1.0f, 8.3f, 21.0f, BLUE);
        DrawCube({ 0.0f, 2.5f, 10.0f }, 21.0f, 8.3f, 1.0f, RED);
        DrawCube({ 0.0f, 2.5f, -10.0f }, 21.0f, 8.3f, 1.0f, RED);
        DrawCube({ 0.0f, 6.7f, 0.0f }, 21.0f, 0.2f, 21.0f, LIGHTGRAY);

        drawFurnitures(chair, desk, deskChair, studentDesk, board, laptop, book, camera, previousCameraPosition);
        collisions(camera, previousCameraPosition, cameraBox, wallBox);
        if (client != nullptr) drawClients3D(client->client_data,8,camera);
        if(IsKeyPressed(KEY_A) || IsKeyPressed(KEY_S) || IsKeyPressed(KEY_D) || IsKeyPressed(KEY_W))
        {
            if (client != nullptr) sendDataToServer(2, 435, 700, 8, camera.position.x, camera.position.y, camera.position.z);
        }
        EndMode3D();
        drawCoordinates(camera);
        
        if (IsKeyPressed(KEY_M))
        {
            break;
        }

        EndDrawing();

    }
    SetExitKey(KEY_EIGHT);
    EnableCursor();

}

void history()
{
    Model chair = LoadModel("objects/chair.obj");
    Model desk = LoadModel("objects/desk.obj");
    Model deskChair = LoadModel("objects/deskChair.obj");
    Model studentDesk = LoadModel("objects/studentDesk.obj");
    Model board = LoadModel("objects/board.obj");
    Model laptop = LoadModel("objects/laptop.obj");
    Model musket = LoadModel("objects/musket.obj");
    Model book = LoadModel("objects/book.obj");

    Texture2D wood = LoadTexture("Textures/wood.png");
    SetMaterialTexture(&studentDesk.materials[0], MATERIAL_MAP_DIFFUSE, wood);

    Texture2D textbook = LoadTexture("Textures/historyBook.png");
    SetMaterialTexture(&book.materials[0], MATERIAL_MAP_DIFFUSE, textbook);

    const int screenWidth = 1920;
    const int screenHeight = 975;

    Camera camera = { 0 };
    camera.position = { 8.0f, 2.0f, -7.0f };   // Camera position
    camera.target = { 0.0f, 2.0f, 0.0f };      // Camera looking at point
    camera.up = { 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 60.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;             // Camera projection type

    int cameraMode = CAMERA_FIRST_PERSON;
    Camera staticCamera = { 0 };
    staticCamera.fovy = 5.0f;
    staticCamera.position = { 0.261261,2,-2.52176 };
    staticCamera.projection = 0;
    staticCamera.target = { 0.529181,2.7501,-10.0623 };
    staticCamera.up = { 0,1,0 };

    BoundingBox wallBox;
    wallBox.min = { -9.0f, 0.0f, -9.0f };
    wallBox.max = { 9.3f, 20.0f, 9.0f };

    BoundingBox cameraBox;
    cameraBox.min = camera.position;
    cameraBox.max = camera.position;
    cameraBox.min.x -= 0.1f;
    cameraBox.min.y -= 0.1f;
    cameraBox.min.z -= 0.1f;
    cameraBox.max.x += 0.3f;
    cameraBox.max.y += 0.5f;
    cameraBox.max.z += 0.5f;


    // Generates some random columns
    DisableCursor();                    // Limit cursor to relative movement inside the window
    float frameCounter = 0;
    float frameTime = 0;
    float timeAccumulator = 0.0f;
    SetTargetFPS(60);                   // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------
    Texture2D draw = { 0 };

    // Main game loop
    while (!WindowShouldClose())
    {
        float deltaTime = GetFrameTime();
        timeAccumulator += deltaTime;
        if (timeAccumulator >= 0.2f)
        {
            timeAccumulator = 0.0f;
            frameCounter++;
            if (client != nullptr) client->receiveData();
        }
        if (IsKeyPressed(KEY_A) || IsKeyPressed(KEY_S) || IsKeyPressed(KEY_D) || IsKeyPressed(KEY_W))
        {
            if (client != nullptr) sendDataToServer(2, 435, 700, 5, camera.position.x, camera.position.y, camera.position.z);
        }
        Vector3 previousCameraPosition = camera.position;
        UpdateCamera(&camera, cameraMode);
        BeginDrawing();

        ClearBackground(RAYWHITE);

        BeginMode3D(camera);
        DrawBillboard(staticCamera, draw, { 0.0f, 3.5f, -9.0f }, 2.0f, WHITE);

        if (distanceCalc(camera.position, { 0.0f,0.2f,-7.0f }) <= 5.f && IsKeyPressed(KEY_F))
        {
            draw = whiteboard::whiteboard();
        }
        else if (IsKeyPressed(KEY_F))
        {
            EnableCursor();
            historyExaminationAlert();
        }
        cameraBox.min = camera.position;
        cameraBox.max = camera.position;

        DrawPlane({ 0.0f, 0.0f, 0.0f }, { 21.0f, 21.0f }, LIGHTGRAY);
        DrawCube({ -10.0f, 2.5f, 0.0f }, 1.0f, 8.3f, 21.0f, BROWN);
        DrawCube({ 10.0f, 2.5f, 0.0f }, 1.0f, 8.3f, 21.0f, BROWN);
        DrawCube({ 0.0f, 2.5f, 10.0f }, 21.0f, 8.3f, 1.0f, YELLOW);
        DrawCube({ 0.0f, 2.5f, -10.0f }, 21.0f, 8.3f, 1.0f, YELLOW);
        DrawCube({ 0.0f, 6.7f, 0.0f }, 21.0f, 0.2f, 21.0f, LIGHTGRAY);

        drawFurnitures(chair, desk, deskChair, studentDesk, board, laptop, book, camera, previousCameraPosition);
        collisions(camera, previousCameraPosition, cameraBox, wallBox);
        DrawModel(musket, { 0.0f,3.0f,8.5f }, 0.03f, BROWN);
        if (client != nullptr) drawClients3D(client->client_data, 5,camera);


        EndMode3D();


        

        EndMode3D();

        EndDrawing();
        if (IsKeyPressed(KEY_M))
        {
            break;
        }
    }
    SetExitKey(KEY_EIGHT);
    EnableCursor();
    UnloadTexture(wood);

}
void physics()
{
    Model chair = LoadModel("objects/chair.obj");
    Model desk = LoadModel("objects/desk.obj");
    Model deskChair = LoadModel("objects/deskChair.obj");
    Model studentDesk = LoadModel("objects/studentDesk.obj");
    Model board = LoadModel("objects/board.obj");
    Model laptop = LoadModel("objects/laptop.obj");
    Model book = LoadModel("objects/book.obj");

    Texture2D wood = LoadTexture("Textures/wood.png");
    SetMaterialTexture(&studentDesk.materials[0], MATERIAL_MAP_DIFFUSE, wood);

    Texture2D textbook = LoadTexture("Textures/physics.png");
    SetMaterialTexture(&book.materials[0], MATERIAL_MAP_DIFFUSE, textbook);

    const int screenWidth = 1920;
    const int screenHeight = 975;

    Camera camera = { 0 };
    camera.position = { 8.0f, 2.0f, -7.0f };   // Camera position
    camera.target = { 0.0f, 2.0f, 0.0f };      // Camera looking at point
    camera.up = { 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 60.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;             // Camera projection type

    int cameraMode = CAMERA_FIRST_PERSON;
    Camera staticCamera = { 0 };
    staticCamera.fovy = 5.0f;
    staticCamera.position = { 0.261261,2,-2.52176 };
    staticCamera.projection = 0;
    staticCamera.target = { 0.529181,2.7501,-10.0623 };
    staticCamera.up = { 0,1,0 };
    BoundingBox wallBox;
    wallBox.min = { -9.0f, 0.0f, -9.0f };
    wallBox.max = { 9.3f, 20.0f, 9.0f };

    BoundingBox cameraBox;
    cameraBox.min = camera.position;
    cameraBox.max = camera.position;
    cameraBox.min.x -= 0.1f;
    cameraBox.min.y -= 0.1f;
    cameraBox.min.z -= 0.1f;
    cameraBox.max.x += 0.3f;
    cameraBox.max.y += 0.5f;
    cameraBox.max.z += 0.5f;

    
    // Generates some random columns
    DisableCursor();                    // Limit cursor to relative movement inside the window
    float frameCounter = 0;
    float frameTime = 0;
    float timeAccumulator = 0.0f;
    SetTargetFPS(60);                   // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------
    Texture2D draw = { 0 };

    // Main game loop
    while (!WindowShouldClose())
    {
        float deltaTime = GetFrameTime();
        timeAccumulator += deltaTime;
        if (timeAccumulator >= 0.2f)
        {
            timeAccumulator = 0.0f;
            frameCounter++;
            if (client != nullptr) client->receiveData();
        }
        if (IsKeyPressed(KEY_A) || IsKeyPressed(KEY_S) || IsKeyPressed(KEY_D) || IsKeyPressed(KEY_W))
        {
            if (client != nullptr) sendDataToServer(2, 435, 700, 3, camera.position.x, camera.position.y, camera.position.z);
        }
        Vector3 previousCameraPosition = camera.position;
        UpdateCamera(&camera, cameraMode);
        BeginDrawing();

        ClearBackground(RAYWHITE);

        BeginMode3D(camera);
        DrawBillboard(staticCamera, draw, { 0.0f, 3.5f, -9.0f }, 2.0f, WHITE);

        if (distanceCalc(camera.position, { 0.0f,0.2f,-7.0f }) <= 5.f && IsKeyPressed(KEY_F))
        {
            draw = whiteboard::whiteboard();
        }
        else if (IsKeyPressed(KEY_F))
        {
            EnableCursor();
            physicsExaminationAlert();
        }
        cameraBox.min = camera.position;
        cameraBox.max = camera.position;

        DrawPlane({ 0.0f, 0.0f, 0.0f }, { 19.0f, 19.0f }, LIGHTGRAY);
        DrawCube({ -10.0f, 2.5f, 0.0f }, 1.0f, 8.3f, 21.0f, DARKPURPLE);
        DrawCube({ 10.0f, 2.5f, 0.0f }, 1.0f, 8.3f, 21.0f, DARKPURPLE);
        DrawCube({ 0.0f, 2.5f, 10.0f }, 21.0f, 8.3f, 1.0f, GOLD);
        DrawCube({ 0.0f, 2.5f, -10.0f }, 21.0f, 8.3f, 1.0f, GOLD);
        DrawCube({ 0.0f, 6.7f, 0.0f }, 21.0f, 0.2f, 21.0f, LIGHTGRAY);
        drawFurnitures(chair, desk, deskChair, studentDesk, board, laptop, book, camera, previousCameraPosition);
        collisions(camera, previousCameraPosition, cameraBox, wallBox);
        if (client != nullptr) drawClients3D(client->client_data, 3,camera);

        EndMode3D();
        if (IsKeyPressed(KEY_M))
        {
            break;
        }
        
        EndDrawing();
        
    }


    SetExitKey(KEY_EIGHT);
    EnableCursor();
    UnloadTexture(wood);

}
void literature()
{
    Model chair = LoadModel("objects/chair.obj");
    Model desk = LoadModel("objects/desk.obj");
    Model deskChair = LoadModel("objects/deskChair.obj");
    Model studentDesk = LoadModel("objects/studentDesk.obj");
    Model board = LoadModel("objects/board.obj");
    Model laptop = LoadModel("objects/laptop.obj");
    Model bookshelf = LoadModel("objects/bookshelf.obj");
    Model book = LoadModel("objects/book.obj");

    Texture2D wood = LoadTexture("Textures/wood.png");
    SetMaterialTexture(&studentDesk.materials[0], MATERIAL_MAP_DIFFUSE, wood);
    SetMaterialTexture(&bookshelf.materials[0], MATERIAL_MAP_DIFFUSE, wood);

    Texture2D textbook = LoadTexture("Textures/literatureBook.png");
    SetMaterialTexture(&book.materials[0], MATERIAL_MAP_DIFFUSE, textbook);

    const int screenWidth = 1920;
    const int screenHeight = 975;

    Camera camera = { 0 };
    camera.position = { 8.0f, 2.0f, -7.0f };   // Camera position
    camera.target = { 0.0f, 2.0f, 0.0f };      // Camera looking at point
    camera.up = { 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 60.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;             // Camera projection type

    int cameraMode = CAMERA_FIRST_PERSON;
    Camera staticCamera = { 0 };
    staticCamera.fovy = 5.0f;
    staticCamera.position = { 0.261261,2,-2.52176 };
    staticCamera.projection = 0;
    staticCamera.target = { 0.529181,2.7501,-10.0623 };
    staticCamera.up = { 0,1,0 };
    BoundingBox wallBox;
    wallBox.min = { -9.0f, 0.0f, -9.0f };
    wallBox.max = { 9.3f, 20.0f, 9.0f };

    BoundingBox cameraBox;
    cameraBox.min = camera.position;
    cameraBox.max = camera.position;
    cameraBox.min.x -= 0.1f;
    cameraBox.min.y -= 0.1f;
    cameraBox.min.z -= 0.1f;
    cameraBox.max.x += 0.3f;
    cameraBox.max.y += 0.5f;
    cameraBox.max.z += 0.5f;


    // Generates some random columns
    DisableCursor();                    // Limit cursor to relative movement inside the window
    float frameCounter = 0;
    float frameTime = 0;
    float timeAccumulator = 0.0f;
    SetTargetFPS(60);                   // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------
    Texture2D draw = { 0 };

    // Main game loop
    while (!WindowShouldClose())
    {
        float deltaTime = GetFrameTime();
        timeAccumulator += deltaTime;
        if (timeAccumulator >= 0.2f)
        {
            timeAccumulator = 0.0f;
            frameCounter++;
            if (client != nullptr) client->receiveData();
        }
        if (IsKeyPressed(KEY_A) || IsKeyPressed(KEY_S) || IsKeyPressed(KEY_D) || IsKeyPressed(KEY_W))
        {
            if (client != nullptr) sendDataToServer(2, 435, 700, 7, camera.position.x, camera.position.y, camera.position.z);
        }
        Vector3 previousCameraPosition = camera.position;
        UpdateCamera(&camera, cameraMode);
        BeginDrawing();

        ClearBackground(RAYWHITE);

        BeginMode3D(camera);
        DrawBillboard(staticCamera, draw, { 0.0f, 3.5f, -9.0f }, 2.0f, WHITE);

        if (distanceCalc(camera.position, { 0.0f,0.2f,-7.0f }) <= 5.f && IsKeyPressed(KEY_F))
        {
            draw = whiteboard::whiteboard();
        }
        else if (IsKeyPressed(KEY_F))
        {
            EnableCursor();
            literatureExaminationAlert();
        }
        cameraBox.min = camera.position;
        cameraBox.max = camera.position;

        DrawPlane({ 0.0f, 0.0f, 0.0f }, { 21.0f, 21.0f }, LIGHTGRAY);
        DrawCube({ -10.0f, 2.5f, 0.0f }, 1.0f, 8.3f, 21.0f, BLUE);
        DrawCube({ 10.0f, 2.5f, 0.0f }, 1.0f, 8.3f, 21.0f, BLUE);
        DrawCube({ 0.0f, 2.5f, 10.0f }, 21.0f, 8.3f, 1.0f, GRAY);
        DrawCube({ 0.0f, 2.5f, -10.0f }, 21.0f, 8.3f, 1.0f, GRAY);
        DrawCube({ 0.0f, 6.7f, 0.0f }, 21.0f, 0.2f, 21.0f, LIGHTGRAY);

        drawFurnitures(chair, desk, deskChair, studentDesk, board, laptop, book, camera, previousCameraPosition);
        DrawModel(bookshelf, { 0.0f,0.0f,8.5f }, 2.0f, BROWN);
        collisions(camera, previousCameraPosition, cameraBox, wallBox);


        collisions(camera, previousCameraPosition, cameraBox, wallBox);
        if (client != nullptr) drawClients3D(client->client_data, 7,camera);


        EndMode3D();

        

        EndMode3D();

        if (IsKeyPressed(KEY_M))
        {
            break;
        }


        EndDrawing();

    }
    SetExitKey(KEY_EIGHT);
    EnableCursor();


}
void chemistry()
{

    Model chair = LoadModel("objects/chair.obj");
    Model desk = LoadModel("objects/desk.obj");
    Model deskChair = LoadModel("objects/deskChair.obj");
    Model studentDesk = LoadModel("objects/studentDesk.obj");
    Model board = LoadModel("objects/board.obj");
    Model tubes = LoadModel("objects/tubes.obj");
    Model laptop = LoadModel("objects/laptop.obj");
    Model book = LoadModel("objects/book.obj");

    Texture2D wood = LoadTexture("Textures/wood.png");
    SetMaterialTexture(&studentDesk.materials[0], MATERIAL_MAP_DIFFUSE, wood);

    Texture2D chemistryBook = LoadTexture("Textures/chemistryBook.png");
    SetMaterialTexture(&book.materials[0], MATERIAL_MAP_DIFFUSE, chemistryBook);

    const int screenWidth = 1920;
    const int screenHeight = 975;

    Camera camera = { 0 };
    camera.position = { 8.0f, 2.0f, -7.0f };   // Camera position
    camera.target = { 0.0f, 2.0f, 0.0f };      // Camera looking at point
    camera.up = { 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 60.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;             // Camera projection type

    int cameraMode = CAMERA_FIRST_PERSON;
    Camera staticCamera = { 0 };
    staticCamera.fovy = 5.0f;
    staticCamera.position = { 0.261261,2,-2.52176 };
    staticCamera.projection = 0;
    staticCamera.target = { 0.529181,2.7501,-10.0623 };
    staticCamera.up = { 0,1,0 };
    BoundingBox wallBox;
    wallBox.min = { -9.0f, 0.0f, -9.0f };
    wallBox.max = { 9.3f, 20.0f, 9.0f };

    BoundingBox cameraBox;
    cameraBox.min = camera.position;
    cameraBox.max = camera.position;
    cameraBox.min.x -= 0.1f;
    cameraBox.min.y -= 0.1f;
    cameraBox.min.z -= 0.1f;
    cameraBox.max.x += 0.3f;
    cameraBox.max.y += 0.5f;
    cameraBox.max.z += 0.5f;


    // Generates some random columns
    DisableCursor();                    // Limit cursor to relative movement inside the window
    float frameCounter = 0;
    float frameTime = 0;
    float timeAccumulator = 0.0f;
    SetTargetFPS(60);                   // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------
    Texture2D draw = { 0 };

    // Main game loop
    while (!WindowShouldClose())
    {
        float deltaTime = GetFrameTime();
        timeAccumulator += deltaTime;
        if (timeAccumulator >= 0.2f)
        {
            timeAccumulator = 0.0f;
            frameCounter++;
            if (client != nullptr) client->receiveData();
        }
        if (IsKeyPressed(KEY_A) || IsKeyPressed(KEY_S) || IsKeyPressed(KEY_D) || IsKeyPressed(KEY_W))
        {
            if (client != nullptr) sendDataToServer(2, 435, 700, 4, camera.position.x, camera.position.y, camera.position.z);
        }
        Vector3 previousCameraPosition = camera.position;
        UpdateCamera(&camera, cameraMode);
        BeginDrawing();

        ClearBackground(RAYWHITE);

        BeginMode3D(camera);
        DrawBillboard(staticCamera, draw, { 0.0f, 3.5f, -9.0f }, 2.0f, WHITE);

        if (distanceCalc(camera.position, { 0.0f,0.2f,-7.0f }) <= 5.f && IsKeyPressed(KEY_F))
        {
            draw = whiteboard::whiteboard();
        }
        else if (IsKeyPressed(KEY_F))
        {
            EnableCursor();
            chemistryExaminationAlert();
        }
        cameraBox.min = camera.position;
        cameraBox.max = camera.position;

        DrawPlane({ 0.0f, 0.0f, 0.0f }, { 21.0f, 21.0f }, LIGHTGRAY);
        DrawCube({ -10.0f, 2.5f, 0.0f }, 1.0f, 8.3f, 21.0f, DARKBLUE);
        DrawCube({ 10.0f, 2.5f, 0.0f }, 1.0f, 8.3f, 21.0f, DARKBLUE);
        DrawCube({ 0.0f, 2.5f, 10.0f }, 21.0f, 8.3f, 1.0f, GOLD);
        DrawCube({ 0.0f, 2.5f, -10.0f }, 21.0f, 8.3f, 1.0f, GOLD);
        DrawCube({ 0.0f, 6.7f, 0.0f }, 21.0f, 0.2f, 21.0f, LIGHTGRAY);

        drawFurnitures(chair, desk, deskChair, studentDesk, board, laptop, book, camera, previousCameraPosition);
        drawTubes(tubes);
        collisions(camera, previousCameraPosition, cameraBox, wallBox);
        if (client != nullptr) drawClients3D(client->client_data, 4,camera);

        EndMode3D();
        

        if (IsKeyPressed(KEY_M))
        {
            break;
        }


        EndDrawing();

    }
    SetExitKey(KEY_EIGHT);
    EnableCursor();
    UnloadTexture(wood);

}
void english()
{
    Model chair = LoadModel("objects/chair.obj");
    Model desk = LoadModel("objects/desk.obj");
    Model deskChair = LoadModel("objects/deskChair.obj");
    Model studentDesk = LoadModel("objects/studentDesk.obj");
    Model board = LoadModel("objects/board.obj");
    Model laptop = LoadModel("objects/laptop.obj");
    Model book = LoadModel("objects/book.obj");

    Texture2D wood = LoadTexture("Textures/wood.png");
    SetMaterialTexture(&studentDesk.materials[0], MATERIAL_MAP_DIFFUSE, wood);

    Texture2D textbook = LoadTexture("Textures/englishBook.png");
    SetMaterialTexture(&book.materials[0], MATERIAL_MAP_DIFFUSE, textbook);

    const int screenWidth = 1920;
    const int screenHeight = 975;

    Camera camera = { 0 };
    camera.position = { 8.0f, 2.0f, -7.0f };    // Camera position
    camera.target = { 0.0f, 2.0f, 0.0f };      // Camera looking at point
    camera.up = { 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 60.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;

    int cameraMode = CAMERA_FIRST_PERSON;
    Camera staticCamera = { 0 };
    staticCamera.fovy = 5.0f;
    staticCamera.position = { 0.261261,2,-2.52176 };
    staticCamera.projection = 0;
    staticCamera.target = { 0.529181,2.7501,-10.0623 };
    staticCamera.up = { 0,1,0 };
    BoundingBox wallBox;
    wallBox.min = { -9.0f, 0.0f, -9.0f };
    wallBox.max = { 9.3f, 20.0f, 9.0f };

    BoundingBox cameraBox;
    cameraBox.min = camera.position;
    cameraBox.max = camera.position;
    cameraBox.min.x -= 0.1f;
    cameraBox.min.y -= 0.1f;
    cameraBox.min.z -= 0.1f;
    cameraBox.max.x += 0.3f;
    cameraBox.max.y += 0.5f;
    cameraBox.max.z += 0.5f;

    // Generates some random columns
    DisableCursor();                    // Limit cursor to relative movement inside the window
    float frameCounter = 0;
    float frameTime = 0;
    float timeAccumulator = 0.0f;
    SetTargetFPS(60);                   // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    
    Texture2D draw = { 0 };

    // Main game loop
    while (!WindowShouldClose())
    {
        float deltaTime = GetFrameTime();
        timeAccumulator += deltaTime;
        if (timeAccumulator >= 0.2f)
        {
            timeAccumulator = 0.0f;
            frameCounter++;
            if (client != nullptr) client->receiveData();
        }
        if (IsKeyPressed(KEY_A) || IsKeyPressed(KEY_S) || IsKeyPressed(KEY_D) || IsKeyPressed(KEY_W))
        {
            if (client != nullptr) sendDataToServer(2, 435, 700, 6, camera.position.x, camera.position.y, camera.position.z);
        }
        Vector3 previousCameraPosition = camera.position;
        UpdateCamera(&camera, cameraMode);
        BeginDrawing();

        ClearBackground(RAYWHITE);

        BeginMode3D(camera);
        DrawBillboard(staticCamera, draw, { 0.0f, 3.5f, -9.0f }, 2.0f, WHITE);

        if (distanceCalc(camera.position, { 0.0f,0.2f,-7.0f }) <= 5.f && IsKeyPressed(KEY_F))
        {
            draw = whiteboard::whiteboard();
        }
        else if (IsKeyPressed(KEY_F))
        {
            EnableCursor();
            englishExaminationAlert();
        }
        cameraBox.min = camera.position;
        cameraBox.max = camera.position;

        DrawPlane({ 0.0f, 0.0f, 0.0f }, { 21.0f, 21.0f }, LIGHTGRAY);
        DrawCube({ -10.0f, 2.5f, 0.0f }, 1.0f, 8.3f, 21.0f, LIME);
        DrawCube({ 10.0f, 2.5f, 0.0f }, 1.0f, 8.3f, 21.0f, LIME);
        DrawCube({ 0.0f, 2.5f, 10.0f }, 21.0f, 8.3f, 1.0f, PURPLE);
        DrawCube({ 0.0f, 2.5f, -10.0f }, 21.0f, 8.3f, 1.0f, PURPLE);
        DrawCube({ 0.0f, 6.7f, 0.0f }, 21.0f, 0.2f, 21.0f, LIGHTGRAY);

        drawFurnitures(chair, desk, deskChair, studentDesk, board, laptop, book, camera, previousCameraPosition);
        collisions(camera, previousCameraPosition, cameraBox, wallBox);
        
        if (client != nullptr) drawClients3D(client->client_data, 6,camera);


        EndMode3D();
        drawCoordinates(camera);
        
        if (IsKeyPressed(KEY_M))
        {
            break;
        }


        EndDrawing();

    }
    SetExitKey(KEY_EIGHT);
    EnableCursor();
    UnloadTexture(wood);
}

void biology()
{
    Model chair = LoadModel("objects/chair.obj");
    Model desk = LoadModel("objects/desk.obj");
    Model deskChair = LoadModel("objects/deskChair.obj");
    Model studentDesk = LoadModel("objects/studentDesk.obj");
    Model board = LoadModel("objects/board.obj");
    Model skeleton = LoadModel("objects/skeleton.obj");
    Model laptop = LoadModel("objects/laptop.obj");
    Model book = LoadModel("objects/book.obj");

    Texture2D wood = LoadTexture("Textures/wood.png");
    Texture2D Musculoskeletal = LoadTexture("Textures/Musculoskeletal.png");
    SetMaterialTexture(&book.materials[0], MATERIAL_MAP_DIFFUSE, Musculoskeletal);
    SetMaterialTexture(&studentDesk.materials[0], MATERIAL_MAP_DIFFUSE, wood);

    Camera camera = { 0 };
    camera.position = { 8.0f, 2.0f, -7.0f };    // Camera position
    camera.target = { 0.0f, 2.0f, -5.0f };      // Camera looking at point
    camera.up = { 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 60.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;             // Camera projection type

    int cameraMode = CAMERA_FIRST_PERSON;
    Camera staticCamera = { 0 };
    staticCamera.fovy = 5.0f;
    staticCamera.position = { 0.261261,2,-2.52176 };
    staticCamera.projection = 0;
    staticCamera.target = { 0.529181,2.7501,-10.0623 };
    staticCamera.up = { 0,1,0 };
    BoundingBox wallBox;
    wallBox.min = { -9.0f, 0.0f, -9.0f };
    wallBox.max = { 9.3f, 20.0f, 9.3f };

    BoundingBox cameraBox;
    cameraBox.min = camera.position;
    cameraBox.max = camera.position;
    cameraBox.min.x -= 0.1f;
    cameraBox.min.y -= 0.1f;
    cameraBox.min.z -= 0.1f;
    cameraBox.max.x += 0.3f;
    cameraBox.max.y += 0.5f;
    cameraBox.max.z += 0.5f;


    // Generates some random columns
    DisableCursor();                    // Limit cursor to relative movement inside the window
    float frameCounter = 0;
    float frameTime = 0;
    float timeAccumulator = 0.0f;
    SetTargetFPS(60);                   // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------
    Texture2D draw = { 0 };

    // Main game loop
    while (!WindowShouldClose())
    {
        float deltaTime = GetFrameTime();
        timeAccumulator += deltaTime;
        if (timeAccumulator >= 0.2f)
        {
            timeAccumulator = 0.0f;
            frameCounter++;
            if (client != nullptr) client->receiveData();
        }
        if (IsKeyPressed(KEY_A) || IsKeyPressed(KEY_S) || IsKeyPressed(KEY_D) || IsKeyPressed(KEY_W))
        {
            if (client != nullptr) sendDataToServer(2, 435, 700, 2, camera.position.x, camera.position.y, camera.position.z);
        }
        Vector3 previousCameraPosition = camera.position;

        UpdateCamera(&camera, cameraMode);
        BeginDrawing();
        ClearBackground(RAYWHITE);
        BeginMode3D(camera);
        DrawBillboard(staticCamera, draw, { 0.0f, 3.5f, -9.0f }, 2.0f, WHITE);

        if (distanceCalc(camera.position, { 0.0f,0.2f,-7.0f }) <= 5.f && IsKeyPressed(KEY_F))
        {
            draw = whiteboard::whiteboard();
        }
        else if (IsKeyPressed(KEY_F))
        {
            EnableCursor();
            biologyExaminationAlert();
        }
        cameraBox.min = camera.position;
        cameraBox.max = camera.position;

		DrawBillboard(camera, wood, { 0.0f, 6.0f, 9.0f }, 2.0f, WHITE);
        DrawPlane({ 0.0f, 0.0f, 0.0f }, { 19.0f, 19.0f }, LIGHTGRAY);
        DrawCube({ -10.0f, 2.5f, 0.0f }, 1.0f, 8.3f, 21.0f, PINK);
        DrawCube({ 10.0f, 2.5f, 0.0f }, 1.0f, 8.3f, 21.0f, PINK);
        DrawCube({ 0.0f, 2.5f, 10.0f }, 21.0f, 8.3f, 1.0f, BROWN);
        DrawCube({ 0.0f, 2.5f, -10.0f }, 21.0f, 8.3f, 1.0f, BROWN);
        DrawCube({ 0.0f, 6.7f, 0.0f }, 21.0f, 0.2f, 21.0f, LIGHTGRAY);

        drawFurnitures(chair, desk, deskChair, studentDesk, board, laptop, book, camera, previousCameraPosition);
        DrawModel(skeleton, { 5.0f, 2.2f, -9.0f }, 0.01, WHITE);
        collisions(camera, previousCameraPosition, cameraBox, wallBox);
        
        if (client != nullptr) drawClients3D(client->client_data, 2,camera);


        EndMode3D();
        drawCoordinates(camera);
        
        if (IsKeyPressed(KEY_M))
        {
            break;
        }
        EndDrawing();
    }
    SetExitKey(KEY_EIGHT);
    UnloadTexture(wood);
    EnableCursor();
}


void geography()
{
    Model chair = LoadModel("objects/chair.obj");
    Model desk = LoadModel("objects/desk.obj");
    Model deskChair = LoadModel("objects/deskChair.obj");
    Model studentDesk = LoadModel("objects/studentDesk.obj");
    Model board = LoadModel("objects/board.obj");
    Model globe = LoadModel("objects/globe.obj");
    Model laptop = LoadModel("objects/laptop.obj");
    Model book = LoadModel("objects/book.obj");

    Texture2D wood = LoadTexture("Textures/wood.png");
    SetMaterialTexture(&studentDesk.materials[0], MATERIAL_MAP_DIFFUSE, wood);

    Texture2D geographyBook = LoadTexture("Textures/geographyBook.png");
    SetMaterialTexture(&book.materials[0], MATERIAL_MAP_DIFFUSE, geographyBook);

    const int screenWidth = 1920;
    const int screenHeight = 975;

    Camera camera = { 0 };
    camera.position = { 8.0f, 2.0f, -7.0f };    // Camera position
    camera.target = { 0.0f, 2.0f, 0.0f };      // Camera looking at point
    camera.up = { 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 60.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;             // Camera projection type

    int cameraMode = CAMERA_FIRST_PERSON;
    Camera staticCamera = { 0 };
    staticCamera.fovy = 5.0f;
    staticCamera.position = { 0.261261,2,-2.52176 };
    staticCamera.projection = 0;
    staticCamera.target = { 0.529181,2.7501,-10.0623 };
    staticCamera.up = { 0,1,0 };
    BoundingBox wallBox;
    wallBox.min = { -9.0f, 0.0f, -9.0f };
    wallBox.max = { 9.3f, 20.0f, 9.0f };

    BoundingBox cameraBox;
    cameraBox.min = camera.position;
    cameraBox.max = camera.position;
    cameraBox.min.x -= 0.1f;
    cameraBox.min.y -= 0.1f;
    cameraBox.min.z -= 0.1f;
    cameraBox.max.x += 0.3f;
    cameraBox.max.y += 0.5f;
    cameraBox.max.z += 0.5f;

    

    // Generates some random columns
    DisableCursor();                    // Limit cursor to relative movement inside the window

    SetTargetFPS(60);                   // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------
    Texture2D draw = { 0 };
    float frameCounter = 0;
    float frameTime = 0;
    float timeAccumulator = 0.0f;
    // Main game loop
    while (!WindowShouldClose())
    {
        float deltaTime = GetFrameTime();
        timeAccumulator += deltaTime;
        if (timeAccumulator >= 0.2f)
        {
            timeAccumulator = 0.0f;
            frameCounter++;
            if (client != nullptr) client->receiveData();
        }
        if (IsKeyPressed(KEY_A) || IsKeyPressed(KEY_S) || IsKeyPressed(KEY_D) || IsKeyPressed(KEY_W))
        {
            if (client != nullptr) sendDataToServer(2, 435, 700, 9, camera.position.x, camera.position.y, camera.position.z);
        }
        Vector3 previousCameraPosition = camera.position;
        UpdateCamera(&camera, cameraMode);
        BeginDrawing();

        ClearBackground(RAYWHITE);

        BeginMode3D(camera);
        DrawBillboard(staticCamera, draw, { 0.0f, 3.5f, -9.0f }, 2.0f, WHITE);

        if (distanceCalc(camera.position, { 0.0f,0.2f,-7.0f }) <= 5.f && IsKeyPressed(KEY_F))
        {
            draw = whiteboard::whiteboard();
        }
        else if (IsKeyPressed(KEY_F))
        {
            EnableCursor();
            geographyExaminationAlert();
        }
        cameraBox.min = camera.position;
        cameraBox.max = camera.position;

        DrawPlane({ 0.0f, 0.0f, 0.0f }, { 21.0f, 21.0f }, LIGHTGRAY);
        DrawCube({ -10.0f, 2.5f, 0.0f }, 1.0f, 8.3f, 21.0f, DARKGREEN);
        DrawCube({ 10.0f, 2.5f, 0.0f }, 1.0f, 8.3f, 21.0f, DARKGREEN);
        DrawCube({ 0.0f, 2.5f, 10.0f }, 21.0f, 8.3f, 1.0f, GREEN);
        DrawCube({ 0.0f, 2.5f, -10.0f }, 21.0f, 8.3f, 1.0f, GREEN);
        DrawCube({ 0.0f, 6.7f, 0.0f }, 21.0f, 0.2f, 21.0f, LIGHTGRAY);

        drawFurnitures(chair, desk, deskChair, studentDesk, board, laptop, book, camera, previousCameraPosition);

        DrawModel(globe, { 0.65f,1.65f,-7.0f }, 0.4, GOLD);

        collisions(camera, previousCameraPosition, cameraBox, wallBox);
        if (client != nullptr) drawClients3D(client->client_data, 9,camera);


        EndMode3D();

      
        if (IsKeyPressed(KEY_M))
        {
            break;
        }


        EndDrawing();

    }
    SetExitKey(KEY_EIGHT);
    EnableCursor();
    UnloadTexture(wood);

}

void programming()
{
    Model computer = LoadModel("objects/computer.obj");
    Model computerRotated = LoadModel("objects/computerRotated.obj");
    Model chair = LoadModel("objects/chair.obj");
    Model chairOther = LoadModel("objects/chairRotated.obj");
    Model desk = LoadModel("objects/deskRotated.obj");
    Model deskChair = LoadModel("objects/deskChairRotated.obj");
    Model workstation = LoadModel("objects/workstation.obj");

    const int screenWidth = 1920;
    const int screenHeight = 975;

    Camera camera = { 0 };
    camera.position = { 18.7f, 2.0f, -6.5f };    // Camera position
    camera.target = { 0.0f, 2.0f, 0.0f };      // Camera looking at point
    camera.up = { 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 60.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;             // Camera projection type

    int cameraMode = CAMERA_FIRST_PERSON;
    Camera staticCamera = { 0 };
    staticCamera.fovy = 5.0f;
    staticCamera.position = { 14.6,2,0.3 };
    Vector3 a;
    staticCamera.projection = 0;
    staticCamera.target = { 66,9.9,-1.7 };
    staticCamera.up = { 0,1,0 };
    BoundingBox wallBox;
    wallBox.min = { -7.0f, 0.0f, -9.0f };
    wallBox.max = { 22.0f, 20.0f, 10.5f };

    BoundingBox cameraBox;
    cameraBox.min = camera.position;
    cameraBox.max = camera.position;
    cameraBox.min.x -= 0.1f;
    cameraBox.min.y -= 0.1f;
    cameraBox.min.z -= 0.1f;
    cameraBox.max.x += 0.3f;
    cameraBox.max.y += 0.5f;
    cameraBox.max.z += 0.5f;

    // Generates some random columns
    DisableCursor();                    // Limit cursor to relative movement inside the window
    float frameCounter = 0;
    float frameTime = 0;
    float timeAccumulator = 0.0f;
    SetTargetFPS(60);                   // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------
    Texture2D draw = { 0 };

    // Main game loop
    while (!WindowShouldClose())
    {
        float deltaTime = GetFrameTime();
        timeAccumulator += deltaTime;
        if (timeAccumulator >= 0.2f)
        {
            timeAccumulator = 0.0f;
            frameCounter++;
            if (client != nullptr) client->receiveData();
        }
        if (IsKeyPressed(KEY_A) || IsKeyPressed(KEY_S) || IsKeyPressed(KEY_D) || IsKeyPressed(KEY_W))
        {
            if (client != nullptr) sendDataToServer(2, 435, 700, 1, camera.position.x, camera.position.y, camera.position.z);
        }
        if (IsKeyPressed(KEY_B))
        {
            cout << camera.fovy << endl << camera.position.x << "," << camera.position.y << "," << camera.position.z << endl << camera.projection << endl << camera.target.x << "," << camera.target.y << "," << camera.target.z << endl << camera.up.x << "," << camera.up.y << "," << camera.up.z << endl;
        }
        Vector3 previousCameraPosition = camera.position;
        UpdateCamera(&camera, cameraMode);
        BeginDrawing();

        ClearBackground(RAYWHITE);

        BeginMode3D(camera);
        DrawBillboard(staticCamera, draw, { 21.0,2,0.2 }, 3.0f, WHITE);

        if (distanceCalc(camera.position, { 21.0,2,0.2 }) <= 5.f && IsKeyPressed(KEY_F))
        {
            draw = whiteboard::whiteboard();
        }
        else if (IsKeyPressed(KEY_F))
        {
            EnableCursor();
            programmingExaminationAlert();
        }
        cameraBox.min = camera.position;
        cameraBox.max = camera.position;

        DrawPlane({ 7.5f, 0.0f, 0.0f }, { 28.5f, 20.0f }, LIGHTGRAY);
        DrawCube({ -7.0f, 2.5f, 0.0f }, 1.0f, 8.3f, 21.0f, ORANGE);
        DrawCube({ 22.0f, 2.5f, 0.0f }, 1.0f, 8.3f, 21.0f, ORANGE);
        DrawCube({ 7.5f, 2.5f, 10.0f }, 28.5f, 8.3f, 1.0f, GOLD);
        DrawCube({ 7.5f, 2.5f, -10.0f }, 28.5f, 8.3f, 1.0f, GOLD);
        DrawCube({ 7.5f, 6.7f, 0.0f }, 28.5f, 0.2f, 21.0f, LIGHTGRAY);

        drawFurnituresProgramming(desk, deskChair, chair, chairOther, workstation, computer, computerRotated, camera, previousCameraPosition);
        DrawBoundingBox(wallBox, RED);
        collisions(camera, previousCameraPosition, cameraBox, wallBox);
        if (client != nullptr) drawClients3D(client->client_data, 10,camera);

        EndMode3D();
        prgTextBook(camera, 0);

        if (IsKeyPressed(KEY_M))
        {
            break;
        }


        EndDrawing();

    }
    SetExitKey(KEY_EIGHT);
    EnableCursor();

}

void physyicaEducation()
{
    Model goal = LoadModel("objects/goal.obj");
    Model basketballHoop = LoadModel("objects/basketballHoop.obj");
    const int screenWidth = 1920;
    const int screenHeight = 975;

    Camera camera = { 0 };
    camera.position = { 18.7f, 2.0f, -6.5f };    // Camera position
    camera.target = { 0.0f, 2.0f, 0.0f };      // Camera looking at point
    camera.up = { 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 60.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;             // Camera projection type

    int cameraMode = CAMERA_FIRST_PERSON;

    Vector3 ballPosition = { -4.0f, 0.5f, 0.0f };

    BoundingBox wallBox;
    wallBox.min = { -21.5f, 0.0f, -14.5f };
    wallBox.max = { 37.5f, 22.5f,14.5f };

    BoundingBox ballBox;


    BoundingBox cameraBox;
    cameraBox.min = camera.position;
    cameraBox.max = camera.position;
    cameraBox.min.x -= 0.1f;
    cameraBox.min.y -= 0.1f;
    cameraBox.min.z -= 0.1f;
    cameraBox.max.x += 0.3f;
    cameraBox.max.y += 0.5f;
    cameraBox.max.z += 0.5f;

    BoundingBox goalBox;
    goalBox.min = { 34.0f, 0.0f, -2.5f };
    goalBox.max = { 36.0f, 6.0f,4.0f };

    float diff = 0;
    // Generates some random columns
    DisableCursor();                    // Limit cursor to relative movement inside the window
    int footballPoints = 0;
    SetTargetFPS(60);                   // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------
    float potentialKickX = 0.0f;
    float potentialKickY = 0.0f;
    float frameCounter = 0;
    float frameTime = 0;
    float timeAccumulator = 0.0f;
    // Main game loop
    while (!WindowShouldClose())
    {
        float deltaTime = GetFrameTime();
        timeAccumulator += deltaTime;
        if (timeAccumulator >= 0.2f)
        {
            timeAccumulator = 0.0f;
            frameCounter++;
            if (client != nullptr) client->receiveData();
        }
        if (IsKeyPressed(KEY_A) || IsKeyPressed(KEY_S) || IsKeyPressed(KEY_D) || IsKeyPressed(KEY_W))
        {
           if(client != nullptr) sendDataToServer(2, 435, 700, 10, camera.position.x, camera.position.y, camera.position.z);
        }
        ballBox.min = { ballPosition.x - 0.3f, ballPosition.y ,ballPosition.z - 0.1f };
        ballBox.max = { ballPosition.x + 0.4f , ballPosition.y + 0.7f ,ballPosition.z + 0.4f };
        Vector3 previousCameraPosition = camera.position;
        kickBall(camera, ballPosition, potentialKickX);
        shootBasketball(camera, ballPosition, potentialKickX, potentialKickY, footballPoints);

        UpdateCamera(&camera, cameraMode);
        BeginDrawing();

        ClearBackground(RAYWHITE);

        collisionsModelToModel(ballPosition, { 10.0f, 0.5f, 0.0f }, ballBox, wallBox);

        BeginMode3D(camera);

        DrawSphere(ballPosition, 0.5f, ORANGE);

        cameraBox.min = camera.position;
        cameraBox.max = camera.position;
        DrawPlane({ 7.5f, 0.0f, 0.0f }, { 60.5f, 30.0f }, LIGHTGRAY);
        DrawCube({ 38.0f, 2.5f, 0.0f }, 1.0f, 25.0f, 48.0f, RED);
        DrawCube({ -22.0f, 2.5f,0.0f }, 1.0f, 25.0f, 48.5f, RED);
        DrawCube({ 7.5f, 2.5f, 15.0f }, 60.5f, 25.0f, 1.0f, BLUE);
        DrawCube({ 7.5f, 2.5f, -15.0f }, 60.5f, 25.0f, 1.0f, BLUE);
        DrawCube({ 7.5f, 15.0f, 0.0f }, 62.5f, 0.2f, 30.0f, LIGHTGRAY);

        DrawModel(goal, { 34.0f, 0.2f, 0.0f }, 1.0, WHITE);
        DrawModel(basketballHoop, { -12.0f, -2.0f, 0.0f }, 10.0, WHITE);

        if (IsKeyPressed(KEY_M))
        {
            break;
        }

        collisions(camera, previousCameraPosition, cameraBox, wallBox);
        if (!collisionsModelToModel(ballPosition, { 34.0f, 0.2f, 0.0f }, ballBox, goalBox))
        {
            footballPoints += 1;
            ballPosition = { 10.0f, 0.5f, 0.0f };
        }
        if (client != nullptr) drawClients3D(client->client_data, 10,camera);

        EndMode3D();
        drawCoordinates(camera);
        std::string pointsStr = "Points: " + std::to_string(footballPoints);
        DrawText(pointsStr.c_str(), 10, 100, 25, BLACK);
        EndDrawing();

    }
    EnableCursor();
    CloseWindow();
    SetExitKey(KEY_APOSTROPHE);
}