#include <iostream>
#include <WS2tcpip.h>
#include <string>
#include <cstdlib>
#include <vector>
#include <future> 

#pragma comment (lib, "ws2_32.lib")
#pragma warning(disable : 4996)
using namespace std;

const int countFunc = 2;
const int bufSize = 256;
char buf[bufSize];
vector< SOCKET> clientSockets;

void createServer() {
	// Initialze winsock
	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);

	int wsOk = WSAStartup(ver, &wsData);
	if (wsOk != 0)
	{
		cerr << "Can't Initialize winsock! Quitting" << endl;
		return;
	}

	//

	// Create a socket
	SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
	if (listening == INVALID_SOCKET)
	{
		cerr << "Can't create a socket! Quitting" << endl;
		return;
	}

	// Bind the ip address and port to a socket
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(54000);
	hint.sin_addr.S_un.S_addr = INADDR_ANY; // Could also use inet_pton .... 

	bind(listening, (sockaddr*)&hint, sizeof(hint));

	// Tell Winsock the socket is for listening 
	listen(listening, SOMAXCONN);

	// Wait for a connection
	sockaddr_in client;
	int clientSize = sizeof(client);
	for (int i = 0; i < countFunc; i++) {
		system("start ..\\..\\Client\\Debug\\Client.exe");
		SOCKET socket = accept(listening, (sockaddr*)&client, &clientSize);
		clientSockets.push_back(socket);
		itoa(i, buf, 10);
		send(socket, buf, strlen(buf) + 1, 0);//send number of funtcion
		cout << "connect" << endl;
	}

	

	// Close listening socket
	closesocket(listening);

}

void closeServer() {
	// Close the socket
	while(!clientSockets.empty())
		closesocket(clientSockets[0]);

	// Cleanup winsock
	WSACleanup();
}

void send(int x) {
	itoa(x, buf, 10);
	for (SOCKET socket: clientSockets)
	{
		send(socket, buf, strlen(buf) + 1, 0);
	}
}

pair<int, int> get_answer(int index) {

}

void main() {
	createServer();

	int x;
	cin >> x;
	send(x);

	future<pair<int, int>> fut = std::async(get_answer, 313222313);
	
	int bytesReceived = recv(clientSockets[0], buf, bufSize, 0);
	if (bytesReceived == SOCKET_ERROR)
	{
		cerr << "Error in recv(). Quitting" << endl;
	}
	int a = atoi(buf);
	cout << a;

	while (false)
	{
		ZeroMemory(buf, bufSize);

		// Wait for client to send data
		int bytesReceived = recv(clientSockets[0], buf, bufSize, 0);
		if (bytesReceived == SOCKET_ERROR)
		{
			cerr << "Error in recv(). Quitting" << endl;
			break; 
		}

		if (bytesReceived == 0)
		{
			cout << "Client disconnected " << endl;
			break;
		}

		cout << string(buf, 0, bytesReceived) << endl;

		// Echo message back to client

		//send(clientSocket, buf, bytesReceived + 1, 0);
		ZeroMemory(buf, bufSize);
		string userInput;
		cout << "> ";
		getline(cin, userInput);

		send(clientSockets[0], userInput.c_str(), userInput.size() + 1, 0);
	}

	closeServer();


	system("pause");
}