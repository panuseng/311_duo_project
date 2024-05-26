#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#define MAX_TEXT_LINES 10       // Maximum number of lines to display
#define LINE_HEIGHT 30           // Height of each line of text

int main(int argc, char* argv[])
{
    const int screenWidth = 300;
    const int screenHeight = 300; // Increased for more lines

    InitWindow(screenWidth, screenHeight, "Client");
    SetTargetFPS(60);

    // Input text variables
    char name[64] = { 0 };
    bool namePressed = false;
    Rectangle nameRec = { 20, 20, 260, 30 }; // Positioned at the top

    // Text display
    char displayText[MAX_TEXT_LINES][64] = { 0 };  // Array for multiple lines
    int currentLine = 0;                         // Index of current line

    while (!WindowShouldClose())
    {
        // Text input logic
        if (GuiTextBox(nameRec, name, 64, namePressed)) {
            namePressed = !namePressed;
            // Add name to displayText and move to the next line on Enter key
            if (IsKeyPressed(KEY_ENTER)) {
                strcpy(displayText[currentLine], name);
                currentLine = (currentLine + 1) % MAX_TEXT_LINES;

                // Reset input box
                memset(name, 0, sizeof(name));
                namePressed = false;
            }
        }

        // Drawing
        BeginDrawing();
            ClearBackground(RAYWHITE);
            for (int i = 0; i < MAX_TEXT_LINES; i++) {
                // Draw each line at a fixed vertical position
                DrawText(displayText[i], 20, 50 + i * LINE_HEIGHT, 20, GRAY);
            }
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
