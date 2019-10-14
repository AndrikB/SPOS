#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <iostream>
#include <functional>
#pragma warning(disable: 4996)

#include <Windows.h>

#include <future>



SOCKET connection;
void createServer() {
	//downloading the library
	//WSAStartup
	WSAData wsaData;
	WORD DLLVersion = MAKEWORD(2, 1);
	if (WSAStartup(DLLVersion, &wsaData) != 0) {
		std::cout << "Error" << std::endl;
		exit(1);
	}

	//fill in socket address information
	SOCKADDR_IN addr;
	int sizeofaddr = sizeof(addr);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(1111);
	addr.sin_family = AF_INET;

	connection = socket(AF_INET, SOCK_STREAM, NULL);
	if (connect(connection, (SOCKADDR*)&addr, sizeof(addr)) != 0) {
		std::cout << "Error: failed connect to server.\n";
		exit(0);
	}

}

void closeServer() {
	// Gracefully close down everything
	closesocket(connection);
	WSACleanup();
}

void getValue() {
	char* c = new char[8];
	ZeroMemory(c, 8);
	recv(connection, c, sizeof(c), NULL);
	exit(0);
}

void sendValue(int msgboxID) {

	char* c = new char[8];
	ZeroMemory(c, 8);

	c[0] = (msgboxID == IDOK? 'T' : 'F');

	send(connection, c, sizeof(c), NULL);

}

void wait() {
	std::this_thread::sleep_for(std::chrono::seconds(60));
	sendValue(IDOK);
	closeServer();
	exit(0);

}

int main() {
	HWND hWnd = GetConsoleWindow();
	//ShowWindow(hWnd, SW_MINIMIZE); 
	ShowWindow(hWnd, SW_HIDE);	
	createServer();

	std::future<void> f=std::async(wait);


	int msgboxID = MessageBoxA(NULL, "It will be closed after 60 second", "EXIT", MB_OKCANCEL);
	sendValue(msgboxID);
	closeServer();
	exit(0);
}