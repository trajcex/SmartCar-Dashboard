#ifndef SHADER_UTILS_H
#define SHADER_UTILS_H

#include <GL/glew.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

unsigned int compileShader(GLenum type, const char* source);

unsigned int createShader(const char* vsSource, const char* fsSource);

#endif 
