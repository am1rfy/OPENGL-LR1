#ifndef GLUTBACKEND_H
#define	GLUTBACKEND_H

#include "callbacks.h"

void GLUTBackendInit(int argc, char** argv);

bool GLUTBackendCreateWindow(unsigned int Width, unsigned int Height, const char* pTitle);

void GLUTBackendRun(ICallbacks* pCallbacks);

#endif