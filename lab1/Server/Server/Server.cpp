#include <iostream>
#include "NetWork.h"
#include <string>
#include <cstdlib>

#pragma comment (lib, "ws2_32.lib")
#pragma warning(disable : 4996)

void main() {

	server.createServer();

	int x;
	std::cin >> x;
	server.sendToAll(x);

	for (int i = 0; i < asyncW->countFunc; i++)
	{
		asyncW->fut[i] = std::async(&NetWork::getAnswer, server, i);
	}
	asyncW->check_was_calculated();

	server.closeServer();
	system("pause");
}