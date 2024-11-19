#include "whiteboard.h"
#include <vector>

namespace whiteboard {

    void whiteboard()
    {
        // Store all points where the mouse has been pressed
        std::vector<Vector2> points;

        EndMode3D();
        while (!WindowShouldClose())
        {
            BeginDrawing();
            ClearBackground(RAYWHITE);

            // If the left mouse button is pressed, add the current mouse position to the points
            if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
            {
                Vector2 mousePosition = GetMousePosition();
                points.push_back(mousePosition);
            }

            // Draw all points from the vector
            for (const Vector2& point : points)
            {
                DrawCircleV(point, 2.0f, BLACK);
            }

            EndDrawing();
        }
    }
}
