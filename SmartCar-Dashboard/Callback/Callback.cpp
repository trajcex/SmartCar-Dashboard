#include "Callback.h"

#include <iostream>
#include <GLFW/glfw3.h> 

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    bool IsDown = action == GLFW_PRESS || action == GLFW_REPEAT;
    switch (key) {
    case GLFW_KEY_ESCAPE: glfwSetWindowShouldClose(window, GLFW_TRUE); break;
    case GLFW_KEY_ENTER: std::cout << "Pristinut ENTER \n"; break;
    }
}

void resizeFramebufferCallback(GLFWwindow* context, int width, int height)
{
	glViewport(0, 0, width, height);

}
void resizeWindowCallback(GLFWwindow* window, int width, int height)
{
    int newWidth = width;
    int newHeight = (int)(width / (16.0f / 9.0f));

    if (newHeight > height) {
        newHeight = height;
        newWidth = (int)(height * (16.0f / 9.0f));
    }

    glfwSetWindowSize(window, newWidth, newHeight);

}

