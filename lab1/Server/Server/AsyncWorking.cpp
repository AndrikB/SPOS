#pragma once
#include "NetWork.h"
#include <iostream>
AsyncWorking* asyncW = new AsyncWorking();

void AsyncWorking::check_end(int index) {
	if (values[index] == 0) {
		std::cout << "zero value becouse " << index << " func return 0\n";
		is_checking = false;

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
				if (!is_checking)
					break;
			}

		}
	}

	for (int i = 0; i < countFunc; i++)
		if (!wasCalculated[i])
			server.sendToClient(i, (char*)"0");
}