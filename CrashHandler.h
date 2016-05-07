#ifndef CRASHHANDLER_H
#define CRASHHANDLER_H

#include "Crashable.h"

class CrashHandler
{
private:
	void (*cleanup)() = nullptr;

public:
	CrashHandler(void (*cleanup)()) : cleanup(cleanup){};
	CrashHandler(){};
	void handle(Crashable &object, bool success) const
	{
		if (success) return;

		object.dumpInfoLog();

		handle((void *)0, "");
	}
	void handle(void *success, const char *mess) const
	{
		if (success) return;

		if (mess) fprintf(stderr, "Message: %s", mess);

		if (cleanup) {
			cleanup();
		} else {
			fprintf(stderr, "No cleanup function provided!\n");
		}

		throw - 1;
	}
	void handle(const char *mess, bool success)
	{
		handle((void *)success, mess);
	}
	void setCleanup(void (*cleanup)()) { this->cleanup = cleanup; }
	virtual ~CrashHandler() {}
};

#endif /* end of include guard: CRASHHANDLER_H */
