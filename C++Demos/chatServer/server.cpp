#include <stdio.h>
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")

#define CLIENT_NUM   10
SOCKET clients[10];

void funcThread(int i);

/* 1 ȷ��Э��汾
 * 2 ����socket
 * 3 ��ȡ������Э���
 * 4 ��
 * 5 ����
 * 6 ��������
 * 7 ͨ��
 * 8 �Ͽ�
 * 9 ����Э��汾��Ϣ
 */
int main() {
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (HIBYTE(wsaData.wVersion) != 2 || LOBYTE(wsaData.wVersion) != 2) {
		printf("Э��汾��ƥ��: %d \n", GetLastError());
		getchar();
		return -1;
	}

	SOCKET server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (SOCKET_ERROR == server) {
		printf("����socketʧ��: %d \n", GetLastError());
		WSACleanup();
		getchar();
		return -1;
	}

	SOCKADDR_IN addr = { 0 };
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(9565);

	int res = bind(server, (sockaddr*)&addr, sizeof(addr));
	if (SOCKET_ERROR == res) {
		printf("��socketʧ��: %d \n", GetLastError());
		closesocket(server);
		WSACleanup();
		getchar();
		return -1;
	}

	res = listen(server, 10);
	if (SOCKET_ERROR == res) {
		printf("socket����ʧ��: %d \n", GetLastError());
		closesocket(server);
		WSACleanup();
		getchar();
		return -1;
	}

	SOCKADDR_IN cAddr = { 0 };
	int len = sizeof(cAddr);
	//SOCKET client = accept(server, (sockaddr*)&cAddr, &len);
	for (int i = 0; i < CLIENT_NUM; ++i) {
		clients[i] = accept(server, (sockaddr*)&cAddr, &len);
		if (SOCKET_ERROR == clients[i]) {
			printf("���ܿͻ���ʧ��: %d \n", GetLastError());
			closesocket(server);
			WSACleanup();
			getchar();
			return -1;
		}
		printf("�ͻ���%d���ӳɹ�: %s \n", i+1, inet_ntoa(cAddr.sin_addr));

		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)funcThread, (void*)i, NULL, NULL);
	}

	

	closesocket(server);
	WSACleanup();
	getchar();
	return 0;
}


void funcThread(int i) {
	char buff[1024];
	while (1) {
		int res = recv(clients[i], buff, 1023, NULL);
		if (res > 0) {
			buff[res] = 0; // \0
			printf("�ͻ���%d��Ϣ >>> %s \n", i+1, buff);
		}
	}
};