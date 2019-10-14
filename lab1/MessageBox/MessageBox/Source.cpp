#include <Windows.h>
#include <future>
#include <WS2tcpip.h>
#pragma comment (lib, "ws2_32.lib")

void createServer() {

}

void closeServer() {

}

void getValue() {

}

void sendValue(int msgboxID) {
	
}

void wait() {

	//todo wait
	sendValue(IDCANCEL);
	closeServer();
	exit(0);

}

int main() {
	HWND hWnd = GetConsoleWindow();
	//ShowWindow(hWnd, SW_MINIMIZE); 
	ShowWindow(hWnd, SW_HIDE);	

	std::future<void> f=std::async(wait);


	int msgboxID = MessageBoxA(NULL, "It will be closed after 60 second", "EXIT", MB_OKCANCEL);
	sendValue(msgboxID);
	closeServer();
	exit(0);
}