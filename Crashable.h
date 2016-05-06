#ifndef CRASHABLE_H
#define CRASHABLE_H

class Crashable
{
public:
	virtual void dumpInfoLog(FILE *stream = stderr) = 0;
	virtual ~Crashable() {}
};

#endif /* end of include guard: CRASHABLE_H */
