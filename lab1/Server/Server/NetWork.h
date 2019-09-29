#pragma once

#include <vector>
#include <WS2tcpip.h>
#include "AsyncWorking.h"
#pragma comment (lib, "ws2_32.lib")


class NetWork
{
public:
	NetWork(int countF);

	bool createServer();

	bool sendToAll(int);

	bool closeServer();

	void getAnswer(int index);

private:
	const static int bufSize = 256;
	char buf[bufSize];
	int countF;
	std::vector<SOCKET> clientSockets;
};

