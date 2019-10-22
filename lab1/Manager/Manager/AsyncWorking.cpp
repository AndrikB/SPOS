#pragma once
#include "NetWork.h"
#include <iostream>
#include <conio.h>

#pragma warning(disable : 4996)


#define ESC_KEY 27

AsyncWorking* asyncW = new AsyncWorking();

void AsyncWorking::restart()
{
	is_checking = false;
	is_in_popWindow = false;
	
}

void AsyncWorking::check_end(int index) {
	if (values[index] == 0) {
		std::cout << "zero value because " << (index==0?"f":"g") << " func return 0\n";
		is_checking = false;
		server.closeProcesses();
		return;

	}

	for (int i = 0; i < countFunc; i++)
		if (fut[i].wait_for(std::chrono::milliseconds(NULL)) == std::future_status::timeout) {
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
			if (fut[i].wait_for(std::chrono::milliseconds(NULL))==std::future_status::ready) {
				check_end(i);

				if (!is_checking) {
					if (is_in_popWindow) {
						delete m;	
						m = nullptr;
					}
					break;
				}

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
	is_in_popWindow = true;
	m = new MessageBoxConnector();
	if (m->get()) {//click exit
		is_in_popWindow = false;
		delete m;
		std::cout << "exit because "
			<< ((fut[0].wait_for(std::chrono::milliseconds(NULL)) == std::future_status::timeout) ? "f " : "")
			<< ((fut[1].wait_for(std::chrono::milliseconds(NULL)) == std::future_status::timeout) ? "g " : "")
			<< "is not calculated\n";
		is_checking = false;
	}
	else {//click no exit
		if (is_checking)//it was no simulation click
			delete m;
		is_in_popWindow = false;
	}


}
