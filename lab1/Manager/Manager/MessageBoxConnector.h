#pragma once
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")


class MessageBoxConnector
{
public:
	MessageBoxConnector();

	SOCKET Socket;
	PROCESS_INFORMATION pi;

	~MessageBoxConnector();
	void Send();
	bool get();
};

