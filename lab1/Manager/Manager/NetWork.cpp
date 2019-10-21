#include "NetWork.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <tchar.h>

#pragma comment (lib, "ws2_32.lib")
#pragma warning(disable : 4996)


NetWork server(AsyncWorking::countFunc);

NetWork::NetWork(int countF)
{
	this->countF = countF;
	ZeroMemory(buf, bufSize);
}

void NetWork::restart()
{
	processesInfo.clear();
	clientSockets.clear();
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

	if (bind(listening, (SOCKADDR*)&hint, sizeof(hint)) == SOCKET_ERROR) {
		std::cerr << "Can't bind a socket! Quitting" << std::endl;
		exit(1);
	}
	if (listen(listening, SOMAXCONN) == SOCKET_ERROR) {
		std::cerr << "Can't listen a socket! Quitting" << std::endl;
		exit(1);
	}

	// Wait for a connection
	sockaddr_in client;
	int clientSize = sizeof(client);

	for (int i = 0; i < countF; i++) {
		STARTUPINFO si;
		PROCESS_INFORMATION pi;
		ZeroMemory(&si, sizeof(si));
		si.cb = sizeof(si);
		ZeroMemory(&pi, sizeof(pi));

		CreateProcess(NULL, _tcsdup(TEXT("\"..\\..\\Function\\Debug\\Function\" - L - S")), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
		this->processesInfo.push_back(pi);

		SOCKET socket = accept(listening, (sockaddr*)&client, &clientSize);
		clientSockets.push_back(socket);

		itoa(i, buf, 10);
		send(socket, buf, strlen(buf) + 1, 0);//send number of funtcion		
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
	asyncW->values[index] = atoi(buf);
}

void NetWork::closeProcesses()
{
	for (int i = 0; i < countF; i++)//close all functions no calculated
		if (asyncW->fut[i].wait_for(std::chrono::milliseconds(NULL)) == std::future_status::timeout) {
			TerminateProcess(server.processesInfo[i].hProcess, i);
		}
}
