#include <stdio.h>
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")



/* 1 ȷ��Э��汾
 * 2 ����socket
 * 3 ��ȡ������Э���
 * 4 ����
 * 5 ͨ��
 * 6 �Ͽ�
 * 7 ����Э��汾��Ϣ
 */
int main() {
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (HIBYTE(wsaData.wVersion) != 2 || LOBYTE(wsaData.wVersion) != 2) {
		printf("Э��汾��ƥ��: %d \n", GetLastError());
		getchar();
		return -1;
	}

	SOCKET client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (SOCKET_ERROR == client) {
		printf("����socketʧ��: %d \n", GetLastError());
		WSACleanup();
		getchar();
		return -1;
	}

	SOCKADDR_IN addr = { 0 };
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(9565);

	int res = connect(client, (sockaddr*)&addr, sizeof(addr));
	if (SOCKET_ERROR == res) {
		printf("���ӷ�����ʧ��: %d \n", GetLastError());
		closesocket(client);
		WSACleanup();
		getchar();
		return -1;
	}

	char buff[1024];
	while (1) {
		printf("�� >>> ");
		scanf("%s", buff);
		send(client, buff, strlen(buff), NULL);
	}

	closesocket(client);
	WSACleanup();

	getchar();

	return 0;
}