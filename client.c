#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

// Color
#define RED_T "\033[1;31m"
#define BLUE_T "\033[1;34m"
#define RESET_T "\033[0m"
#define GREEN_T "\033[32m"

#define SERVER_PORT 1100
#define BUFFER_SIZE 1024



#define MAX_TEXT_LINES 10
#define LINE_HEIGHT 25


void	gui(int	sock) {

	const int		screenWidth = 300;
	const int		screenHeight = 300;

	InitWindow(screenWidth, screenHeight, "Client");
	SetTargetFPS(60);

	char text[64] = { 0 };
	bool textPressed = false;
	Rectangle textRec = { 20, 60, 150, 30 };

	bool sendButtonPressed = false;
	Rectangle sendButtonRec = { 190, 60, 80, 30 };

	char displayText[MAX_TEXT_LINES][64] = { 0 };
	int currentLine = 0;

	while (!WindowShouldClose())
	{
		if (GuiTextBox(textRec, text, 64, textPressed)) textPressed = !textPressed;

		if (GuiButton(sendButtonRec, "SEND"))
		{
			sendButtonPressed = true;

			strcpy(displayText[currentLine], TextFormat("input: %s", text));
			currentLine = (currentLine + 1) % MAX_TEXT_LINES;

			send(sock, text, BUFFER_SIZE, 0);

			memset(text, 0, sizeof(text));
			textPressed = false;
			// return ;
		}

		BeginDrawing();
			ClearBackground(RAYWHITE);
			GuiDrawText("input:", (Rectangle){ 20, 40, 50, 20 }, TEXT_ALIGN_LEFT, BLACK);

			for (int i = 0; i < MAX_TEXT_LINES; i++) {
				DrawText(displayText[i], 20, 100 + i * LINE_HEIGHT, 20, GRAY);
			}

		EndDrawing();
	}

	CloseWindow();
}

int		main(int argc, char* argv[]) {

	struct	sockaddr_in serv_addr;

	int		sock = socket(AF_INET, SOCK_STREAM, 0);

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(SERVER_PORT);

	inet_pton(AF_INET, "127.000.0.1", &serv_addr.sin_addr);

	connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
	printf(GREEN_T"Connect to server."RESET_T);

	gui(sock);
}
