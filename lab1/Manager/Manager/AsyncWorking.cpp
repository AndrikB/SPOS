#pragma once
#include "NetWork.h"
#include <iostream>
#include <conio.h>

#pragma warning(disable : 4996)

#define UPD_TIME 1
#define EXIT_TIME 10
#define ESC_KEY 27

AsyncWorking* asyncW = new AsyncWorking();

void AsyncWorking::restart()
{
	is_checking = false;
	is_in_popWindow = false;
	for (int i = 0; i < countFunc; i++) {
		wasCalculated[i] = false;
		values[i] = -1;
	}
}

void AsyncWorking::check_end(int index) {
	if (values[index] == 0) {
		std::cout << "zero value because " << (index==0?"f":"g") << " func return 0\n";
		is_checking = false;
		server.closeProcesses();
		return;

	}

	for (int i = 0; i < countFunc; i++)
		if (!wasCalculated[i]) {
			return;
		}

	int answer = 1;
	for (int i = 0; i < countFunc; i++) {
		answer *= values[i];
	}
	std::cout << answer<<" value\n";
	is_checking = false;
}	

void AsyncWorking::check_was_calculated() {
	is_checking = true;
	while (is_checking) {

		for (int i = 0; i < countFunc; i++) {
			if (fut[i].wait_for(std::chrono::milliseconds(1))==std::future_status::ready) {
				check_end(i);

				if (!is_checking) {//is calculated
					if (is_in_popWindow) {//close message box
						m->isCalculated = true;
						m->closeServ();
					}
					break;
				}

			}
		}//create window

		if (is_in_popWindow) {
			if (time(nullptr) - timeLastMBStarted >= timeLeft) {//timeout
				is_checking = false;
				is_in_popWindow = false;
				m->isCalculated = false;
				m->closeServ();
				m = nullptr;
				timeLastMBStarted = time(nullptr);
				
			}
			if (time(nullptr) - timeLastMBStarted >= UPD_TIME) {//restart
				timeLastMBStarted += UPD_TIME;
				timeLeft -= UPD_TIME;
				m->restart(timeLeft);
			}

		}

	}

}



void AsyncWorking::chec_Esc()
{
	while (is_checking) {
		int c =  getch();//todo daemonize
		if (!is_checking)
			return;
		if (c == ESC_KEY) {
			pops_new_window();
		}
	}
}

void AsyncWorking::pops_new_window()
{
	time(&timeLastMBStarted);
	timeLeft = EXIT_TIME;
	m = new MessageBoxConnector(timeLeft);
	is_in_popWindow = true;
	if (m->get()) {//click exit
		is_in_popWindow = false;
		delete m;
		std::cout << "exit because "
			<< (!wasCalculated[0] ? "f " : "")
			<< (!wasCalculated[1] ? "g " : "")
			<< "is not calculated\n";
		is_checking = false;
	}
	else {//click no exit
		delete m;
		is_in_popWindow = false;
	}


}
