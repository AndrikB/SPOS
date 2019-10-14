#include "MessageBoxConnector.h"
#include <winsock2.h>
#include <iostream>
#include <ws2tcpip.h>
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

		system("start ..\\..\\MessageBox\\Debug\\MessageBox.exe");
		Socket = accept(sListen, (SOCKADDR*)&addr, &sizeofaddr);


		/*{
			char host[NI_MAXHOST];		// Client's remote name
			char service[NI_MAXSERV];	// Service (i.e. port) the client is connect on

			ZeroMemory(host, NI_MAXHOST); // same as memset(host, 0, NI_MAXHOST);
			ZeroMemory(service, NI_MAXSERV);

			if (getnameinfo((sockaddr*)&addr, sizeof(addr), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
			{
				std::cout << host << " connected on port " << service << std::endl;
			}
			else
			{
				inet_ntop(AF_INET, &addr.sin_addr, host, NI_MAXHOST);
				std::cout << host << " connected on port " <<
					ntohs(addr.sin_port) << std::endl;
			}
		}*/

	
		closesocket(sListen);
	}


}

MessageBoxConnector::~MessageBoxConnector()
{
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

	recv(Socket, c, sizeof(c), NULL);
	if (c[0] == 'T') return true;
	return false;
}
