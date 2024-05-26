#include "raylib.h"

#define RAYGUI_IMPLEMENTATION     // Define before including raygui.h
#include "raygui.h"

#define MAX_TEXT_LINES 10        // Maximum number of lines to display
#define LINE_HEIGHT 25           // Height of each line of text

int main(int argc, char* argv[])
{
    const int screenWidth = 300;
    const int screenHeight = 300;

    InitWindow(screenWidth, screenHeight, "Client");
    SetTargetFPS(60);

    char text[64] = { 0 };
    bool textPressed = false;
    Rectangle textRec = { 20, 60, 150, 30 };

    bool sendButtonPressed = false;
    Rectangle sendButtonRec = { 190, 60, 80, 30 };

    // Text display
    char displayText[MAX_TEXT_LINES][64] = { 0 };  // Array for multiple lines
    int currentLine = 0;                         // Index of current line

    while (!WindowShouldClose())
    {
        // Text input logic
        if (GuiTextBox(textRec, text, 64, textPressed)) textPressed = !textPressed;

        // Button logic
        if (GuiButton(sendButtonRec, "SEND"))
        {
            sendButtonPressed = true;

            // Add text to displayText
            strcpy(displayText[currentLine], TextFormat("input: %s", text));
            currentLine = (currentLine + 1) % MAX_TEXT_LINES; // Cycle through lines

            // Reset input box
            memset(text, 0, sizeof(text));
            textPressed = false;
        }

        // Drawing
        BeginDrawing();
            ClearBackground(RAYWHITE);
            GuiDrawText("input:", (Rectangle){ 20, 40, 50, 20 }, TEXT_ALIGN_LEFT, BLACK);

            for (int i = 0; i < MAX_TEXT_LINES; i++) {
                // Draw each line at a fixed vertical position
                DrawText(displayText[i], 20, 100 + i * LINE_HEIGHT, 20, GRAY);
            }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}

