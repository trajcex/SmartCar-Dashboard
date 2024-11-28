#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <GL/glew.h>   
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ShaderUtils/ShaderUtils.h"
#include "Callback/Callback.h"

const double TARGET_FPS = 120.0;
const double FPS = 1.0 / TARGET_FPS;
const std::string SHADER_PATH = "ShaderUtils/Shader/";

int main()
{
    if (!glfwInit()) {
        std::cout << "GLFW Biblioteka se nije ucitala! :(\n";
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window;
    int wWidth = 970;
    int wHeight = 545;
    const char wTitle[] = "Smart Car Dashbord";
    window = glfwCreateWindow(wWidth, wHeight, wTitle, NULL, NULL);

    if (window == NULL()) {
        std::cout << "Prozor nije napravljen! :(\n";
        glfwTerminate();
        return 2; 
    }

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, keyCallback);
    glfwSetFramebufferSizeCallback(window, resizeFramebufferCallback);
    glfwSetWindowSizeLimits(window, wWidth, wHeight, GLFW_DONT_CARE, GLFW_DONT_CARE);
    glfwSetWindowSizeCallback(window, resizeWindowCallback);

    if (glewInit() != GLEW_OK) {
            std::cout << "GLEW nije mogao da se ucita! :'(\n";
            return 3;
    }

    float vreteces[] = {

        0.25, 0.1, 1.0, 0.0, 0.0,
        0.75, 0.1, 0.0, 0.0, 1.0,
        0.5, 0.5,  1.0, 1.0, 0.0,

    };

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);


    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vreteces), vreteces, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    unsigned int basicShader = createShader((SHADER_PATH+"basic.vert").c_str(), (SHADER_PATH+"basic.frag").c_str());
    
    float aspectRatio = (float)wWidth / (float)wHeight;
    unsigned int uAspectLoc = glGetUniformLocation(basicShader, "uAspectRatio");
    glUniform1f(uAspectLoc, aspectRatio);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


    glClearColor(0.5, 0.5, 0.5, 1.0);
    
    double previousTime = glfwGetTime();
    float triangleX = 0.0;
    float triangleY = 0.0;

    while (!glfwWindowShouldClose(window)) {
        double currentTime = glfwGetTime();
        double elapsedTime = currentTime - previousTime;

        if (elapsedTime < FPS) {
            glfwWaitEventsTimeout(FPS - elapsedTime);
            continue;
        }

        previousTime = currentTime;

        glfwGetFramebufferSize(window, &wWidth, &wHeight);
        glUseProgram(basicShader);

        aspectRatio = (float)wHeight / (float)wWidth;
        glm::mat4 projection = glm::mat4(1.0f);
        projection[0][0] = aspectRatio;  
        projection[1][1] = 1.0f;  
        projection[2][2] = 1.0f; 
        projection[3][3] = 1.0f;

        GLuint projectionLoc = glGetUniformLocation(basicShader, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

        glClear(GL_COLOR_BUFFER_BIT);
        /*riangleX += 0.01;
        triangleY += 0.01;
        if (triangleX > 1.0) {
            triangleX = -2.0;
            triangleY = -2.0;
        }*/


        glm::mat4  model = glm::mat4(1.0);
        model = glm::translate(model, glm::vec3(triangleX, triangleY, 0.0));

        GLuint modelLoc = glGetUniformLocation(basicShader, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));


        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);
        glUseProgram(0);


        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteProgram(basicShader);
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);

    glfwTerminate();
    return 0;
}
