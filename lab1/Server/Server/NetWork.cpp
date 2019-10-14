//#include "Server.cpp"

#include "NetWork.h"
#include <iostream>
#include <string>
#include <cstdlib>

#pragma comment (lib, "ws2_32.lib")
#pragma warning(disable : 4996)


NetWork server(AsyncWorking::countFunc);

NetWork::NetWork(int countF)
{
	ZeroMemory(buf, bufSize);
	this->countF = countF;
}

bool NetWork::createServer()
{
	if (!clientSockets.empty()) return false;
	/// Initialze winsock
	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);

	int wsOk = WSAStartup(ver, &wsData);
	if (wsOk != 0)
	{
		std::cerr << "Can't Initialize winsock! Quitting" << std::endl;
		return false;
	}

	// Create a socket
	SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
	if (listening == INVALID_SOCKET)
	{
		std::cerr << "Can't create a socket! Quitting" << std::endl;
		return false;
	}

	// Bind the ip address and port to a socket
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(54000);
	hint.sin_addr.S_un.S_addr = INADDR_ANY; // Could also use inet_pton .... 

	bind(listening, (sockaddr*)&hint, sizeof(hint));//todo check all is okay

	// Tell Winsock the socket is for listening 
	listen(listening, SOMAXCONN);

	// Wait for a connection
	sockaddr_in client;
	int clientSize = sizeof(client);

	for (int i = 0; i < countF; i++) {
		system("start ..\\..\\Client\\Debug\\Client.exe");
		SOCKET socket = accept(listening, (sockaddr*)&client, &clientSize);
		clientSockets.push_back(socket);
		itoa(i, buf, 10);
		send(socket, buf, strlen(buf) + 1, 0);//send number of funtcion
		std::cout << "connect" << std::endl;


		{
			char host[NI_MAXHOST];		// Client's remote name
			char service[NI_MAXSERV];	// Service (i.e. port) the client is connect on

			ZeroMemory(host, NI_MAXHOST); // same as memset(host, 0, NI_MAXHOST);
			ZeroMemory(service, NI_MAXSERV);

			if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
			{
				std::cout << host << " connected on port " << service << std::endl;
			}
			else
			{
				inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
				std::cout << host << " connected on port " <<
					ntohs(client.sin_port) << std::endl;
			}
		}
	}



	// Close listening socket
	closesocket(listening);
	return true;
}

bool NetWork::sendToAll(int x)
{
	if (clientSockets.size() == 0)return false;
	itoa(x, buf, 10);
	for (SOCKET socket : clientSockets)
	{
		send(socket, buf, strlen(buf) + 1, 0);
	}
	return true;
}

bool NetWork::sendToClient(int i, char m[])
{
	if (asyncW->wasCalculated[i])
		return false;
	send(clientSockets[i], m, strlen(m) + 1, 0);

	return true;
}

bool NetWork::closeServer()
{
	// Close the socket
	while (!clientSockets.empty()) {

		closesocket(clientSockets[0]);
		clientSockets.erase(clientSockets.begin());
	}

	// Cleanup winsock
	WSACleanup();
	return true;
}

void NetWork::getAnswer(int index)
{
	char buf[bufSize];
	int bytesReceived = recv(clientSockets[index], buf, bufSize, 0);
	if (bytesReceived == SOCKET_ERROR)
	{
		std::cerr << "Error in recv(). Quitting" << std::endl;
	}
	std::cout << index << "___: " << atoi(buf) << std::endl;
	asyncW->values[index] = atoi(buf);
	asyncW->wasCalculated[index] = true;
}
