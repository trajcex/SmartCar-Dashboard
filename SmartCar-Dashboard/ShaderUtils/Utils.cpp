#include "Utils.h"


void drawArcGradiant(float startAngleDeg, float endAngleDeg, int numSegments, float radius, float* vertices) {
    float startAngle = startAngleDeg * M_PI / 180.0f;
    float endAngle = endAngleDeg * M_PI / 180.0f;

    if (startAngle >= endAngle) {
        std::cerr << "Start angle mora biti manji od end angle!" << std::endl;
        return;
    }

    for (int i = 0; i <= numSegments; i++) {
        float t = static_cast<float>(i) / numSegments;
        float angle = endAngle - (endAngle - startAngle) * i / numSegments;

        vertices[0 + (i * 6)] = radius * cos(angle);
        vertices[1 + (i * 6)] = radius * sin(angle);
        if (t <= 0.5f) {
            float localT = t / 0.5f;
            vertices[2 + (i * 6)] = localT;
            vertices[3 + (i * 6)] = 1.0f;
            vertices[4 + (i * 6)] = 0.0f;
        }
        else {
            float localT = (t - 0.5f) / 0.5f;
            vertices[2 + (i * 6)] = 1.0f;
            vertices[3 + (i * 6)] = 1.0f - localT;
            vertices[4 + (i * 6)] = 0.0f;
        }

        vertices[5 + (i * 6)] = 1.0f;
    }
}

void drawArc(float startAngleDeg, float endAngleDeg, int numSegments, float radius, float* vertices, float color[4]) {
    float startAngle = startAngleDeg * M_PI / 180.0f;
    float endAngle = endAngleDeg * M_PI / 180.0f;

    if (startAngle >= endAngle) {
        std::cerr << "Start angle mora biti manji od end angle!" << std::endl;
        return;
    }

    for (int i = 0; i <= numSegments; i++) {
        float t = static_cast<float>(i) / numSegments;
        float angle = endAngle - (endAngle - startAngle) * i / numSegments;

        vertices[0 + (i * 6)] = radius * cos(angle);
        vertices[1 + (i * 6)] = radius * sin(angle);

        vertices[2 + (i * 6)] = color[0];
        vertices[3 + (i * 6)] = color[1];
        vertices[4 + (i * 6)] = color[2];
        vertices[5 + (i * 6)] = color[3];
    }
}


glm::vec4 getColor(int gear) {

    glm::vec4 white(1.0f, 1.0f, 1.0f, 1.0f);
    glm::vec4 green(0.208f, 0.854f, 0.024f, 1.0f);
    glm::vec4 red(0.9059, 0.2980, 0.2353, 1.0);
    glm::vec4 orange(0.9529, 0.6118, 0.0706, 1.0);
    glm::vec4 purple(0.6078, 0.3490, 0.7137, 1.0);
    glm::vec4 blue(0.2039, 0.5961, 0.8588, 1.0);
    switch (gear) {
        case 1: return red;
        case 2: return orange;
        case 3: return purple;
        case 4: return blue;
        case 5: return green;
        default: return white;
    }
}


void generateNumber(float* number) {
    const int rows = 10;
    const int cols = 16;
    const float x1 = 0.62f, x2 = 0.75f;
    const float y1 = -0.54f, y2 = -0.64f;

    int index = 0;
    for (int i = 0; i < rows; ++i) {
        float z = 0.1f * i;
        number[index++] = x1; number[index++] = y1; number[index++] = z;   number[index++] = 1.0f;
        number[index++] = x1; number[index++] = y2; number[index++] = z;   number[index++] = 0.0f;
        number[index++] = x2; number[index++] = y1; number[index++] = z + 0.1f;   number[index++] = 1.0f;
        number[index++] = x2; number[index++] = y2; number[index++] = z + 0.1f;   number[index++] = 0.0f;
    }

}