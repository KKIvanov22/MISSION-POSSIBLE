#include "whiteboard.h"

namespace whiteboard {

    void DrawTexturedFrontFace(Texture2D texture, Vector3 position, float width, float height, float depth) {
        float x = position.x;
        float y = position.y;
        float z = position.z;

        rlPushMatrix();
        rlTranslatef(x, y, z);

        rlBegin(RL_QUADS);
        rlSetTexture(texture.id);

        rlTexCoord2f(0.0f, 0.0f); rlVertex3f(-width / 2, -height / 2, depth / 2); 
        rlTexCoord2f(1.0f, 0.0f); rlVertex3f(width / 2, -height / 2, depth / 2);  
        rlTexCoord2f(1.0f, 1.0f); rlVertex3f(width / 2, height / 2, depth / 2);   
        rlTexCoord2f(0.0f, 1.0f); rlVertex3f(-width / 2, height / 2, depth / 2);  

        rlEnd();
        rlSetTexture(0);  

        rlBegin(RL_QUADS);
        rlColor4ub(255, 255, 255, 255);  

        rlVertex3f(-width / 2, -height / 2, -depth / 2); 
        rlVertex3f(width / 2, -height / 2, -depth / 2); 
        rlVertex3f(width / 2, height / 2, -depth / 2);   
        rlVertex3f(-width / 2, height / 2, -depth / 2);  

        rlEnd();
        rlColor4ub(255, 255, 255, 255);  

        rlPopMatrix();
    }


    Texture2D whiteboard()
    {
        std::vector<Vector2> points;
        Vector2 mousePosition = { 0.0f, 0.0f };
        EnableCursor();
        EndMode3D();

        RenderTexture2D target = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());

        while (!WindowShouldClose())
        {
            if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
            {
                mousePosition = GetMousePosition();
                points.push_back(mousePosition);
            }

            if (IsKeyPressed(KEY_Z))
            {
                if (!points.empty())
                {
                    points.pop_back();
                }
            }

            BeginTextureMode(target);
            ClearBackground(RAYWHITE);

            for (const Vector2& point : points)
            {
                DrawCircleV(point, 2.0f, BLACK);
            }

            EndTextureMode();

            BeginDrawing();
            ClearBackground(RAYWHITE);

            DrawTextureRec(target.texture, { 0, 0, (float)target.texture.width, (float)-target.texture.height }, { 0, 0 }, WHITE);

            EndDrawing();

            if (IsKeyPressed(KEY_F))
            {
                Image image = LoadImageFromTexture(target.texture);
                ImageFlipVertical(&image);  
                ExportImage(image, "whiteboard.png");
                UnloadImage(image);
            }

            if (IsKeyPressed(KEY_ESCAPE))
            {
                break;
            }
        }

        DisableCursor();

        Image flippedImage = LoadImageFromTexture(target.texture);
        ImageFlipVertical(&flippedImage);
        Texture2D flippedTexture = LoadTextureFromImage(flippedImage);
        UnloadImage(flippedImage);
        UnloadRenderTexture(target);

        return flippedTexture;
    }
}