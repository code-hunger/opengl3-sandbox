#ifndef APP_H
#define APP_H

#include <vector>

class App
{
public:
    App ();
    virtual ~App ();
    void run();

private:
	std::vector<ShaderProgram> shaderPrograms;
	GLint transfLoc = 0;

    void init();
};

#endif /* end of include guard: APP_H */
