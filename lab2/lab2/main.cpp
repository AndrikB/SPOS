#include <iostream>
#include"DekkerLock.h"
#include <thread>
#include <atomic>


#define COUNT_ITEARTION 1e6
#define TYPE true

int counter = 0;


thread_sync::DekkerLock* d = new thread_sync::DekkerLock();

volatile std::atomic_bool flag[2] = { false,false };
//volatile bool flag[2] = {false, false};
volatile int turn = 0;

void s0() {
	for (int i = 0; i < COUNT_ITEARTION; i++)
	{
		flag[0] = true;
		while (flag[1]) {
			if (turn != 0) {
				flag[0] = false;
				while (turn != 0) {}
				flag[0] = true;
			}
		}
		counter++;
		turn = 1;
		flag[0] = false;
	}
}

void s1() {

	for (int i = 0; i < COUNT_ITEARTION; i++)
	{
		flag[1] = true;
		while (flag[0]) {
			if (turn != 1) {
				flag[1] = false;
				while (turn != 1) {}
				flag[1] = true;
			}
		}
		counter++;
		turn = 0;
		flag[1] = false;
	}
}



void f() {
	for (int i = 0; i < COUNT_ITEARTION; i++)
	{
		if (TYPE)
			d->lock();
		counter++;
		if (TYPE)
			d->unlock();
	}
}

int main() {
	std::thread first(f);
	std::thread second(f);
	
	first.join();
	second.join();
	std::cout << counter << '\t' << COUNT_ITEARTION * 2<<'\n';
	std::cout << (COUNT_ITEARTION * 2 - counter) ;


}