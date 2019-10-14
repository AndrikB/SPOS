#pragma once
#include <future> 




class AsyncWorking {
public:
	const static int countFunc = 2;

	int values[countFunc] = {};
	bool wasCalculated[countFunc] = {};
	std::future<void> fut[countFunc];
	bool is_checking=false;

	std::chrono::time_point<std::chrono::system_clock> start;

	void check_end(int index);
	void check_was_calculated();
	void chec_Esc();
	void pops_new_window();
};

extern AsyncWorking* asyncW;