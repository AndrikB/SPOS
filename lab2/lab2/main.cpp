#include <iostream>
#include"DekkerLock.h"
#include <thread>
#include <atomic>


#define COUNT_ITEARTION 1e6


thread_sync::DekkerLock* d = new thread_sync::DekkerLock();
int counter = 0;

void syncIncrement() {
	for (int i = 0; i < COUNT_ITEARTION; i++)
	{
		d->lock();
		counter++;
		d->unlock();
	}
}

void unsyncIncrement() {
	for (int i = 0; i < COUNT_ITEARTION; i++)
	{
		counter++;
	}
}


int main() {
	std::thread first(syncIncrement);
	std::thread second(syncIncrement);
	
	first.join();
	second.join();
	std::cout << counter << '\t' << COUNT_ITEARTION * 2<<'\n';
	std::cout << (COUNT_ITEARTION * 2 - counter) ;


}