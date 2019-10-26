#pragma once
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")


class MessageBoxConnector
{
public:
	MessageBoxConnector(int time);

	SOCKET Socket;
	PROCESS_INFORMATION pi;

	~MessageBoxConnector();
	bool get();
	void restart(int time);
	void closeServ();

	bool isCalculated;

private:
	bool isRestarting = false;

	void Send(int time);
	void createServ();
};

