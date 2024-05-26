#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

// Color
#define RED "\033[1;31m"
#define BLUE "\033[1;34m"
#define RESET "\033[0m"
#define GREEN "\033[32m"

#define SERVER_PORT 1100
#define BUFFER_SIZE 1024 //ควรลดลงไหม

char* 	mini(char Str[]) {

	int		Diff_Number = 8;//		Number +-8 48 - 57 '0' - '9'
	int		Diff_A = 9;//			A +- 9 65 - 90 'A' - 'Z'
	int		Diff_a = 86;//			a +-86 97 - 122 'a' - 'z'

	if (strlen(Str) % 2 == 1)
		return NULL;
	int	length = strlen(Str) / 2;
	char *mini_Str = (char*)malloc(length + 1);
	if !(mini_Str)
		return NULL;

	int		i = 0;//	Str
	int		j = 0;//	mini_Str

	while (Str[i] && Str[i + 1])
	{
		char	buffer[3] = {Str[i], Str[i+1], '\0'};
		int		num = atoi(buffer);
		if (num >= 40 && num <= 49)
			mini_Str[j++] = num + Diff_Number;
		else if (num >= 11 && num <= 36)// a z
			mini_Str[j++] = num + Diff_a;
		else if (num >= 74 && num <= 99)// A Z
			mini_Str[j++] = num - Diff_A;
		else {
			free(mini_Str);
			return NULL;
		}
		i += 2;
	}

	return mini_Str;
}


void *socket_menagement(void *arg) {

	int			server_socket;
	int			client_socket;
	int			*new_sock;

	struct		sockaddr_in;
	struct		server_addr;
	struct		client_addr;

	pthread_t	thread_id;

	socklen_t addr_size = sizeof(struct sockaddr_in);

	server_socket = socket(AF_INET, SOCK_STREAM, 0); // ต้องไปดู

	// ประเภท ip ต้องไปอ่าน
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(PORT);

	bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));//น่าสนใจ เป็นตัวผูก server & address
	listen(server_socket, 10); //รอการเชื่อมต่อsocket ที่กำหนด ตามจน.เครื่อง

	while ((client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &addr_size)))//accept การเชื่อม 				จนกว่าจะหยุดเชื่อมก็เชื่อม
	{
		char	input_Client[BUFFER_SIZE];

		new_sock = (int *)malloc(sizeof(int));
		*new_sock = client_socket;

		recv(new_sock, input_Client, BUFFER_SIZE, 0); //Ag 1 รับผ่านตัวหนึ่งเลข Ag 2 ตัวรับ Ag 3 ขนาด Ag 4 ไม่แน่ใจ
		//ได้string มาหละ
		char* string = pthread_create(&thread_id, NULL, mini, input_Client);
		printf(string);
		// pthread_detach(thread_id);
	}

	close(server_socket);
	return NULL;
}

int		main(int argc, *char argv[])
{

	// #define SERVER_PORT 1100
	// #define BUFFER_SIZE 1024
	pthread_t server_tid;
	pthread_create(&server_tid, NULL, socket_menagement, NULL);

	pthread_exit(NULL);
}
