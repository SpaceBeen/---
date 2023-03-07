#include <iostream>
#pragma comment(lib,"ws2_32.lib")
#include <winsock2.h>
#include <WS2tcpip.h>

using namespace std;

SOCKET Connect;
SOCKET* Connections;
SOCKET Listen;

int clientcount = 0;

void SendMessageToClient(int ID)	
{
	char* buffer = new char[1024];
	for (;; Sleep(75))
	{
		memset(buffer, 0, sizeof(buffer));
		if (recv(Connections[ID], buffer, 1024, NULL))
		{
			printf(buffer);
			printf("\n");
			for (int i = 0; i <= clientcount; i++)
			{
				send(Connections[i], buffer, strlen(buffer), NULL);
			}
		}
	}
	delete(buffer);
}

int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "russian");
	//WSAStartup
	WSAData Data;
	WORD Version = MAKEWORD(2, 2);
	if (WSAStartup(Version, &Data) != 0)
	{
		cout << "Error" << endl;
		exit(1);
	}

	struct addrinfo hints;
	struct addrinfo *result;

	Connections = (SOCKET*)calloc(64, sizeof(SOCKET));
	ZeroMemory(&hints, sizeof(hints));
	
	hints.ai_family = AF_INET;
	hints.ai_flags = AI_PASSIVE;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	getaddrinfo(NULL, "7770", &hints, &result);

	Listen = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	bind(Listen, result->ai_addr, result->ai_addrlen);
	listen(Listen, SOMAXCONN);

	freeaddrinfo(result);

	printf("Start Server....");
	char m_connect[] = "Connect...;;;5";
	for (; ; Sleep(75))
	{
		if (Connect == accept(Listen, NULL, NULL)) 
		{
			printf("Client connect...\n");
			Connections[clientcount] = Connect;
			send(Connections[clientcount], m_connect, strlen(m_connect), NULL);
			clientcount++;
			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)SendMessageToClient, (LPVOID)(clientcount - 1), NULL, NULL);
		}

	}
	return 1;
}