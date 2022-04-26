#include <windows.networking.sockets.h>
#include <iostream>
#pragma comment(lib, "Ws2_32.lib")

using namespace std;

int main()
{
	//starts Winsock DLLs		
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		return 0;

	//create server socket
	SOCKET ServerSocket;
	ServerSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (ServerSocket == INVALID_SOCKET) {
		WSACleanup();
		return 0;
	}

	//binds socket to address
	sockaddr_in SvrAddr;
	SvrAddr.sin_family = AF_INET;
	SvrAddr.sin_addr.s_addr = INADDR_ANY;
	SvrAddr.sin_port = htons(27000);
	if (bind(ServerSocket, (struct sockaddr*)&SvrAddr, sizeof(SvrAddr)) == SOCKET_ERROR)
	{
		closesocket(ServerSocket);
		WSACleanup();
		return 0;
	}

	//listen on a socket
	if (listen(ServerSocket, 1) == SOCKET_ERROR) {
		closesocket(ServerSocket);
		WSACleanup();
		return 0;
	}
	
	int terminator = 0;
	do {
		cout << "Waiting for client connection\n" << endl;

		//accepts a connection from a client
		SOCKET ConnectionSocket;
		ConnectionSocket = SOCKET_ERROR;
		if ((ConnectionSocket = accept(ServerSocket, NULL, NULL)) == SOCKET_ERROR) {
			closesocket(ServerSocket);
			WSACleanup();
			return 0;
		}

		cout << "Connection Established" << endl;

		//receives RxBuffer
		char RxBuffer[128] = {};
		int cmp = 1;

		while (cmp != 0) { 
			recv(ConnectionSocket, RxBuffer, sizeof(RxBuffer), 0);
			cmp = strcmp(RxBuffer, "quit");
			cout << "Msg Rx: " << RxBuffer << endl;

			send(ConnectionSocket, "Recieved Message", sizeof("Recieved Message"), 0);
			if (cmp == 0) {
				cout << "Client disconnected\n";
			}
		}
	} while (terminator == 0);


	
	closesocket(ConnectionSocket);	//closes incoming socket
	closesocket(ServerSocket);	//closes server socket	
	WSACleanup();			//frees Winsock resources

	return 1;
}
