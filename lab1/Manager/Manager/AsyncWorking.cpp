#pragma once
#include "NetWork.h"
#include <iostream>
#include <conio.h>

#pragma warning(disable : 4996)


#define ESC_KEY 27

AsyncWorking* asyncW = new AsyncWorking();

void AsyncWorking::check_end(int index) {
	if (values[index] == 0) {
		std::cout << "zero value becouse " << index << " func return 0\n";
		is_checking = false;
		return;
	}

	for (bool wasCalc : wasCalculated) {
		if (!wasCalc) return;
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
			if (fut[i]._Is_ready()) {
				check_end(i);
				if (!is_checking) {
					if (is_in_popWindow) {//todo change -----------------------
						delete m;		
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
			is_in_popWindow = true;			
			
			pops_new_window();


		}
	}
}

void AsyncWorking::pops_new_window()
{
	m = new MessageBoxConnector();
	if (m->get()) {
		delete m;
		std::cout << "exit\n";//todo write why -----------------------------------------------------------
		is_checking = false;
	}
	else {
		if (m)
			delete m;
		is_in_popWindow = false;
	}


}
