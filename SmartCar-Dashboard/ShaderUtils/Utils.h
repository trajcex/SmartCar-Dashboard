
#ifndef UTILS_H
#define UTILS_H

#define _USE_MATH_DEFINES 
#include <cmath>
#include <iostream>
#include <glm/glm.hpp>

void drawArcGradiant(float startAngleDeg, float endAngleDeg, int numSegments, float radius, float* vertices);
void drawArc(float startAngleDeg, float endAngleDeg, int numSegments, float radius, float* vertices, float color[4]);
glm::vec4 getColor(int gear);
void generateNumber(float* number);

#endif 