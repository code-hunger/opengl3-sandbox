#ifndef LOGGER_H_F36PNJYA
#define LOGGER_H_F36PNJYA

class Logger
{
	char unsigned level = 0;

public:
	void log(const char* msg, ...);
};

#endif /* end of include guard: LOGGER_H_F36PNJYA */
