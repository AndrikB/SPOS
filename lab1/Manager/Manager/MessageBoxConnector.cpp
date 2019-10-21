#include "MessageBoxConnector.h"
#include <winsock2.h>
#include <iostream>
#include <ws2tcpip.h>
#include <tchar.h>

#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable: 4996)


MessageBoxConnector::MessageBoxConnector()
{
	{
		//downloading the library
		//WSAStartup
		WSAData wsaData;
		WORD DLLVersion = MAKEWORD(2, 1);
		if (WSAStartup(DLLVersion, &wsaData) != 0) {
			std::cout << "Error" << std::endl;
			exit(1);
		}

		//fill in socket address information
		SOCKADDR_IN addr;
		int sizeofaddr = sizeof(addr);
		addr.sin_addr.s_addr = inet_addr("127.0.0.1");
		addr.sin_port = htons(1111);
		addr.sin_family = AF_INET;

		SOCKET sListen = socket(AF_INET, SOCK_STREAM, NULL);
		if (sListen == INVALID_SOCKET)
		{
			std::cerr << "Can't create a socket! Quitting" << std::endl;
			exit(1);
		}
		if (bind(sListen, (SOCKADDR*)&addr, sizeof(addr))== SOCKET_ERROR) {
			std::cerr << "Can't bind a socket! Quitting" << std::endl;
			exit(1);
		}
		if (listen(sListen, SOMAXCONN) == SOCKET_ERROR) {
			std::cerr << "Can't listen a socket! Quitting" << std::endl;
			exit(1);
		}

		
		STARTUPINFO si;
		ZeroMemory(&si, sizeof(si));
		si.cb = sizeof(si);
		ZeroMemory(&pi, sizeof(pi));

		CreateProcess(NULL, _tcsdup(TEXT("\"..\\..\\MessageBox\\Debug\\MessageBox\" - L - S)")), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
		Socket = accept(sListen, (SOCKADDR*)&addr, &sizeofaddr);

	
		closesocket(sListen);
	}


}

MessageBoxConnector::~MessageBoxConnector()
{
	TerminateProcess(pi.hProcess, 0);
	//CloseHandle(pi.hProcess);
	//CloseHandle(pi.hThread);
	closesocket(Socket);
	WSACleanup();
}

void MessageBoxConnector::Send()
{
	char* c = new char[8];
	ZeroMemory(c, 8);

	c[0] = 's';

	send(Socket, c, sizeof(c), NULL);

}

bool MessageBoxConnector::get()
{
	char* c = new char[8];
	ZeroMemory(c, 8);

	int bytesReceived =recv(Socket, c, sizeof(c), NULL);
	if (bytesReceived == SOCKET_ERROR)
		return false;
	if (c[0] == 'T') return true;
	return false;
}
