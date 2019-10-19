#include <iostream>
#include <string>
#include <WS2tcpip.h>

#include <functional>

#include "demofuncs"

#include <future> 


#pragma comment(lib, "ws2_32.lib")

#pragma warning(disable : 4996)
	

using namespace std;



const int bufSize = 256;

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
	HWND hWnd = GetForegroundWindow();
	//ShowWindow(hWnd, SW_HIDE);
	createClient();

	char buf[bufSize];
	ZeroMemory(buf, bufSize);
	int bytesReceived = recv(sock, buf, bufSize, 0);
	int numberFunc = atoi(buf);
	//cout << "func number " << numberFunc << endl;
	

	bytesReceived = recv(sock, buf, bufSize, 0);
	if (bytesReceived > 0)
	{
		// Echo response to console
		//cout << "SERVER> " << string(buf, 0, bytesReceived) << endl;
	}
	int x = atoi(buf);

	int a=0;
	if (numberFunc == 0)
		a = spos::lab1::demo::f_func<spos::lab1::demo::INT>(x);
	else if (numberFunc == 1)
		a = spos::lab1::demo::g_func<spos::lab1::demo::INT>(x);
	//cout << a << endl;
	ZeroMemory(buf, bufSize);

	itoa(a, buf, 10);

	int sendResult = send(sock, buf, strlen(buf), 0);
	closeClient();	
	exit(0);
}