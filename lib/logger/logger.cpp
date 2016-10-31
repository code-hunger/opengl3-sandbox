#include "logger.h"

#include <cstdarg>
#include <cstdio>

#include <iostream>

Logger::Logger(char unsigned level, bool should_indent)
    : level(level), should_indent(should_indent)
{
}

const Logger& Logger::operator()(Color color)
{
	refresh();
	printf("\033[%d;%dm", color != Color::Default, color);
	return *this;
}

const Logger& Logger::operator()(const char* msg, ...) const
{
	va_list args;
	va_start(args, msg);
	vprintf(msg, args);
	va_end(args);
	return *this;
}

const Logger& Logger::operator()(const char* msg, ...)
{
	refresh();
	va_list args;
	va_start(args, msg);
	vprintf(msg, args);
	va_end(args);
	return *this;
}

void Logger::refresh() const
{
	puts("\033[0m");
	for (int i = 0; i < level; ++i) {
		putchar('\t');
	}
}

Logger& Logger::operator--()
{
	if (level) --level;
	return *this;
}

Logger& Logger::operator++()
{
	if (level < 10) ++level;
	return *this;
}
