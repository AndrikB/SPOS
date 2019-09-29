#pragma once
#include <future> 




class AsyncWorking {
public:
	const static int countFunc = 2;

	int values[countFunc] = {};
	bool wasCalculated[countFunc] = {};
	std::future<void> fut[countFunc];
	std::future<void>f;
	bool is_checking=false;

	void check_end(int index);
	void check_was_calculated();
};

extern AsyncWorking* asyncW;