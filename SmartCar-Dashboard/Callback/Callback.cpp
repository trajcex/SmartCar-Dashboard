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
    case GLFW_KEY_LEFT: if (IsDown) letfBlinkActive = true; leftBlinkStartFrame = frameCounter; break;
    case GLFW_KEY_RIGHT: if (IsDown) rightBlinkActive = true; rightBlinkStartFrame = frameCounter; break;
    case GLFW_KEY_G: accelerator += 1; if (accelerator > 100) accelerator = 100; break;
    case GLFW_KEY_UP: if (IsDown) temperature += 1; if (temperature > 30) temperature = 30;  break;
    case GLFW_KEY_DOWN: if (IsDown) temperature -= 1; if (temperature <0) temperature = 0;  break;
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

        float xNormalized = (xpos / width) * 2.0f - 1.0f;
        float yNormalized = 1.0f - (ypos / height) * 2.0f; 

        float xPowerMin = 0.55f;
        float xPowerMax = 0.65f;
        float yPowerMin = -0.97f;
        float yPowerMax = -0.87f;

        if (xNormalized >= xPowerMin && xNormalized <= xPowerMax &&
            yNormalized >= yPowerMin && yNormalized <= yPowerMax) {
            radioOn = !radioOn;
            //std::cout << "Clicked inside target area!" << std::endl;
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
    //int newSize = std::min(width, height);


    //glfwSetWindowSize(window, newSize, newSize);

}