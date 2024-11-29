#ifndef CALLBACKS_H
#define CALLBACKS_H

#include <GLFW/glfw3.h>

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void resizeFramebufferCallback(GLFWwindow* context, int width, int height);
void resizeWindowCallback(GLFWwindow* window, int width, int height);

#endif
