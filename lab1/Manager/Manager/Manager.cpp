#include <iostream>
#include "NetWork.h"
#include <string>
#include <cstdlib>
#include <conio.h>

#pragma comment (lib, "ws2_32.lib")
#pragma warning(disable : 4996)

void main() 
{
	for (int i = 0; i < 10; i++) {
		server.restart();
		asyncW->restart();
		std::cout << "x= ";
		int x;
		std::cin >> x;

		server.createServer();

		server.sendToAll(x);

		for (int i = 0; i < asyncW->countFunc; i++)
		{
			asyncW->fut[i] = std::async(&NetWork::getAnswer, server, i);
		}

		std::future<void> f = std::async(&AsyncWorking::chec_Esc, asyncW);

		asyncW->check_was_calculated();

		server.closeServer();
		

		std::cout << "Press any key to continue . . .";
		if (f.wait_for(std::chrono::microseconds(NULL)) == std::future_status::ready)
			getch();
		std::cout << '\n';
	}
	
}