#include <iostream>
#include <string>
#include <WS2tcpip.h>

#include <functional>

#include "demofuncs"

#pragma comment(lib, "ws2_32.lib")

#pragma warning(disable : 4996)
	

using namespace std;

SOCKET sock;

void createClient() {
	string ipAddress = "127.0.0.1";			// IP Address of the server
	int port = 54000;						// Listening port # on the server

	// Initialize WinSock
	WSAData data;
	WORD ver = MAKEWORD(2, 2);
	int wsResult = WSAStartup(ver, &data);
	if (wsResult != 0)
	{
		cerr << "Can't start Winsock, Err #" << wsResult << endl;
		return;
	}

	// Create socket
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
	{
		cerr << "Can't create socket, Err #" << WSAGetLastError() << endl;
		WSACleanup();
		return;
	}

	// Fill in a hint structure
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

	// Connect to server
	int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
	if (connResult == SOCKET_ERROR)
	{
		cerr << "Can't connect to server, Err #" << WSAGetLastError() << endl;
		closesocket(sock);
		WSACleanup();
		return;
	}

	// Do-while loop to send and receive data

}

void closeClient() {
	// Gracefully close down everything
	closesocket(sock);
	WSACleanup();
}



void main() {
	createClient();

	char buf[4096];
	ZeroMemory(buf, 4096);
	string userInput;

	int bytesReceived = recv(sock, buf, 4096, 0);
	if (bytesReceived > 0)
	{
		// Echo response to console
		cout << "SERVER> " << string(buf, 0, bytesReceived) << endl;
	}
	int x = atoi(buf);

	
	const function<int(int)>& f = spos::f_func<spos::INT>;

	int a = f(x);

	ZeroMemory(buf, 4096);

	itoa(a, buf, 10);

	int sendResult = send(sock, buf, strlen(buf), 0);

	system("pause");
	exit(0);
	do
	{
		// Prompt the user for some text
		cout << "> ";
		getline(cin, userInput);

		if (userInput.size() > 0)		// Make sure the user has typed in something
		{
			// Send the text
			int sendResult = send(sock, userInput.c_str(), userInput.size() + 1, 0);
			if (sendResult != SOCKET_ERROR)
			{
				// Wait for response
				ZeroMemory(buf, 4096);
				int bytesReceived = recv(sock, buf, 4096, 0);
				if (bytesReceived > 0)
				{
					// Echo response to console
					cout << "SERVER> " << string(buf, 0, bytesReceived) << endl;
				}
			}
		}

	} while (userInput.size() > 0);

	closeClient();
}