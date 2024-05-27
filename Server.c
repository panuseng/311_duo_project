#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

// Color
#define RED_T "\033[1;31m"
#define BLUE_T "\033[1;34m"
#define RESET_T "\033[0m"
#define GREEN_T "\033[32m"

#define SERVER_PORT 1100
#define BUFFER_SIZE 1024 //ควรลดลงไหม

char receivedMessage[BUFFER_SIZE] = ""; // Initialize with an empty string

void* mini(void *arg) {
	char *Str = (char *)arg;

	int Diff_Number = 8; // Number +-8 48 - 57 '0' - '9'
	int Diff_A = 9;      // A +- 9 65 - 90 'A' - 'Z'
	int Diff_a = 86;     // a +-86 97 - 122 'a' - 'z'

	if (strlen(Str) % 2 == 1)
		return NULL;
	int length = strlen(Str) / 2;
	char *mini_Str = (char *)malloc(length + 1);
	if (!mini_Str)
		return NULL;

	int i = 0; // Str
	int j = 0; // mini_Str

	while (Str[i] && Str[i + 1]) {
		char buffer[3] = {Str[i], Str[i + 1], '\0'};
		int num = atoi(buffer);
		if (num >= 40 && num <= 49)
			mini_Str[j++] = num + Diff_Number;
		else if (num >= 11 && num <= 36) // a z
			mini_Str[j++] = num + Diff_a;
		else if (num >= 74 && num <= 99) // A Z
			mini_Str[j++] = num - Diff_A;
		else {
			free(mini_Str);
			return NULL;
		}
		i += 2;
	}
	mini_Str[j] = '\0';
	return (void *)mini_Str;
}

void *socket_management(void *arg) {
	int server_socket;
	int client_socket;
	int *new_sock;

	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;

	pthread_t thread_id;

	socklen_t addr_size = sizeof(struct sockaddr_in);

	server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (server_socket < 0) {
		perror("ไม่สามารถสร้าง socket ได้");
		return NULL;
	}

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(SERVER_PORT);

	if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
		perror("ไม่สามารถ bind ได้");
		return NULL;
	}
	listen(server_socket, 1);
		while ((client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &addr_size))) {
		char *result;
		char input_Client[BUFFER_SIZE];

		new_sock = (int *)malloc(sizeof(int));
		*new_sock = client_socket;

		memset(input_Client, 0, BUFFER_SIZE);
		recv(*new_sock, input_Client, BUFFER_SIZE, 0);
		pthread_create(&thread_id, NULL, mini, (void *)input_Client);
		pthread_join(thread_id, (void **)&result);

		strcpy(receivedMessage, (char *)result);

		// แสดงผลในเทอร์มินัล
		printf("ผลลัพธ์: " BLUE_T "%s\n" RESET_T, (char *)result);

		// ส่งผลลัพธ์กลับไปยัง client
		send(*new_sock, result, strlen(result), 0);
		free(result); // ปล่อยหน่วยความจำหลังส่งผลลัพธ์
		free(new_sock); // ปล่อยหน่วยความจำ socket
		close(client_socket);
	}
	close(server_socket);
	return NULL;
}

int main(int argc, char **argv) {
	const int screenWidth = 300;
	const int screenHeight = 300;

	printf(GREEN_T "กำลังเริ่มต้น server...\n" RESET_T); // พิมพ์ข้อความเริ่มต้น
	InitWindow(screenWidth, screenHeight, "Server");

	pthread_t server_tid;
	pthread_create(&server_tid, NULL, socket_management, NULL);

	while (!WindowShouldClose()) {
		BeginDrawing();
		ClearBackground(RAYWHITE);
		DrawText("Server listen...", 10, 10, 20, GRAY);
		DrawText(receivedMessage, 20, 20, 20, DARKGRAY);

		EndDrawing();
	}

	pthread_exit(NULL);
	CloseWindow();
	return 0;
}
