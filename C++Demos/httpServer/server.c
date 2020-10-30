#include <stdio.h>
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")  // ���뾲̬��


int merror(int redata, int error, char* showinfo) {
	if (redata == error) {
		perror(showinfo);
		getchar();
		return -1;
	}
	return 0;
}


void sendhtml(SOCKET s, char* filename);


int main() {
	printf("Web����������... \n");
	WSADATA wsdata;
	int isok = WSAStartup(MAKEWORD(2, 2), &wsdata);  // socket�汾
	merror(isok, WSAEINVAL, "����socketʧ�� \n");

	// AF_INET: Э���壬������ַ����
	// SOCK_STREAM: ��������
	// IPPROTO_TCP������Э��
	SOCKET server=socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	merror(server, INVALID_SOCKET, "����socketʧ�� \n");

	struct sockaddr_in seraddr;
	seraddr.sin_family = AF_INET;
	seraddr.sin_port = htons(80);  // �����˴洢��pc��С��
	seraddr.sin_addr.s_addr = INADDR_ANY;  //���������ַ
	
	isok = bind(server, &seraddr, sizeof(seraddr));
	merror(isok, SOCKET_ERROR, "��ʧ�� \n");

	isok = listen(server, 5);
	merror(isok, SOCKET_ERROR, "����ʧ�� \n");

	struct sockaddr_in claddr;
	int cllen = sizeof(claddr);
	while (1) {
		SOCKET client = accept(server, &claddr, &cllen);
		merror(client, INVALID_SOCKET, "����ʧ�� \n");

		char revdata[1024] = "";
		recv(client, revdata, 1024, 0);
		printf("%s �����ܵ�%d�ֽ����� \n", revdata, strlen(revdata));

		//char senddata[1024] = "���յ�һ����Ϣ \n";
		//send(client, senddata, 20, 0);

		char* filename = "index.html";
		sendhtml(client, filename);

		closesocket(client);
	}

	closesocket(server);
	WSACleanup();

	getchar();
	return 0;
}


void sendhtml(SOCKET s, char* filename) {
	FILE* file = fopen(filename, "r");
	if (file == NULL) {
		printf("�ļ���ʧ�� \n");
		return;
	}
	char temp[1024] = "";
	do {
		fgets(temp, 1024, file);
		send(s, temp, strlen(temp), 0);
	} while (!feof(file));
}