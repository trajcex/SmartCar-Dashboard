#include "Callback.h"

#include <iostream>
#include <GLFW/glfw3.h> 
#include "../ShaderUtils/state.h"

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    bool IsDown = action == GLFW_PRESS;
    switch (key) {
    case GLFW_KEY_ESCAPE: glfwSetWindowShouldClose(window, GLFW_TRUE); break;
    case GLFW_KEY_ENTER: std::cout << "ENTER \n"; break;
    case GLFW_KEY_V: if (IsDown) drawVisor = true; break;
    case GLFW_KEY_LEFT: if (IsDown) blinkActive = true; blinkStartFrame = frameCounter; break;
    case GLFW_KEY_RIGHT: if (IsDown) std::cout << "RIGHT \n"; break;
    }
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos){}
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        int width, height;
        glfwGetWindowSize(window, &width, &height);

        float xMin = width * 0.1f;  // (width * 0.2) / 2
        float xMax = width * 0.9f;  // (width * 1.8) / 2
        float yMax = height * 0.3f; // (height * 0.6) / 2

        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        if (xpos >= xMin && xpos <= xMax && ypos <= yMax) {
            drawVisor = false;
        }
    }
}

void resizeFramebufferCallback(GLFWwindow* context, int width, int height)
{
	glViewport(0, 0, width, height);

}

void resizeWindowCallback(GLFWwindow* window, int width, int height)
{
    /*int newWidth = width;
    int newHeight = (int)(width / (16.0f / 9.0f));

    if (newHeight > height) {
        newHeight = height;
        newWidth = (int)(height * (16.0f / 9.0f));
    }*/
    int newSize = std::max(width, height);


    glfwSetWindowSize(window, newSize, newSize);

}