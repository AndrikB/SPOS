#include "DekkerLock.h"
#include <thread>
namespace thread_sync {
	void DekkerLock::lock(int ID)
	{
		int other = 1 - ID;
		flag[ID] = true;
		while (flag[other]) {
			if (turn != ID) {
				flag[ID] = false;
				while (turn != ID) { }
				flag[ID] = true;
			}
		}
	}
	void DekkerLock::unlock(int ID)
	{
		int other = 1 - ID;
		turn = other;
		flag[ID] = false;
		unregister_thread();
	}
}