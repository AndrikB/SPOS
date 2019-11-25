#pragma once
#include "Stas/AbstractFixnumLockable.h"
namespace thread_sync {
	class DekkerLock :
		public AbstractFixnumLockable
	{
	public:
		using AbstractFixnumLockable::lock;
		using AbstractFixnumLockable::unlock;

		DekkerLock():AbstractFixnumLockable(2) {}
		void lock(int ID)override;
		void unlock(int ID) override;

	private:
		//volatile std::atomic_bool flag[2] = {};
		volatile bool flag[2] = {};
		volatile int turn = 1;
	};
}

