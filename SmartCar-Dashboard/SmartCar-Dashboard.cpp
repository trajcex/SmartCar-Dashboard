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

#include "ShaderUtils/Shader.h"
#include "ShaderUtils/Texture.h"
#include "Callback/Callback.h"
#include "Car/TestBed.h"
#include "ShaderUtils/state.h"

//#define STB_IMAGE_IMPLEMENTATION
//#include "ShaderUtils/stb_image.h"

const double TARGET_FPS = 120.0;
const double FPS = 1.0 / TARGET_FPS;
const std::string SHADER_PATH = "ShaderUtils/Shader/";

int main()
{
 
    Car car = getCar();

    if (!glfwInit()) {
        std::cout << "GLFW Biblioteka se nije ucitala! :(\n";
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window;
    int wWidth = 600;
    int wHeight = 600;
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
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    if (glewInit() != GLEW_OK) {
            std::cout << "GLEW nije mogao da se ucita! :'(\n";
            return 3;
    }

    float vreteces[] = {
        
        -1.0,1.0 , 0.0,1.0,
        -1.0,-1.0 , 0.0,0.0,
        1.0,1.0 , 1.0,1.0,
        1.0,-1.0 , 1.0,0.0,

        -1.0, 0.0,  0.0, 1.0,
        -1.0, -1.0,    0.0, 0.0,
        1.0, 0.0,   1.0, 1.0,
        1.0, -1.0,    1.0, 0.0, 

        -0.8,1.0 , 0.0,1.0,
        -0.8,0.4 , 0.0,0.0,
        0.8,1.0 , 1.0,1.0,
        0.8,0.4 , 1.0,0.0,


        0.5,-0.15 , 0.0,1.0,
        0.5,-0.3 , 0.0,0.0,
        0.65,-0.15 , 1.0,1.0,
        0.65,-0.3 , 1.0,0.0,

    };
    unsigned int stride = (2 + 2) * sizeof(float);

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);


    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vreteces), vreteces, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    Shader basicShader((SHADER_PATH + "basic.vert").c_str(), (SHADER_PATH + "basic.frag").c_str());
    
    basicShader.use();

    Texture checkerTexture("res/speedometer.png");
    basicShader.setFloat("uTex", 0);

    Texture mapTexture("res/map.jpg");
    basicShader.setFloat("uTex", 0);

    Texture visorTexture("res/visor.png");
    basicShader.setFloat("uTex", 0);

    Texture leftArrowTexture("res/arrow.png");
    basicShader.setFloat("uTex", 0);

    basicShader.stop();

    glClearColor(0.5, 0.5, 0.5, 1.0);
    

    unsigned uColLoc = glGetUniformLocation(basicShader.ID, "uCol");
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    double previousTime = glfwGetTime();
    startSimulation(&car);

    while (!glfwWindowShouldClose(window)) {
        double currentTime = glfwGetTime();
        double elapsedTime = currentTime - previousTime;

        if (elapsedTime < FPS) {
            glfwWaitEventsTimeout(FPS - elapsedTime);
            continue;
        }

        previousTime = currentTime;

        /*float speed = car.getSpeed();
        std::cout << "Speed: " << speed << std::endl;*/

        basicShader.use();
        glClear(GL_COLOR_BUFFER_BIT);



        glBindVertexArray(VAO);
        unsigned modelLoc = glGetUniformLocation(basicShader.ID, "model");
        unsigned blinkLoc = glGetUniformLocation(basicShader.ID, "blink");

        glActiveTexture(GL_TEXTURE0);
        glm::mat4 modell = glm::mat4(1.0f);

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modell));


        mapTexture.bind();
        glm::vec4 color1(0.0f, 0.0f, 1.0f, 1.0f); 
        glUniform4fv(uColLoc, 1, glm::value_ptr(color1));
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        mapTexture.unbind();
        
        glActiveTexture(GL_TEXTURE0);
        
        checkerTexture.bind();
        glm::vec4 color(0.0f, 0.0f, 9.0f, 0.0f);
        glUniform4fv(uColLoc, 1, glm::value_ptr(color));
        glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);
        checkerTexture.unbind();

        if (drawVisor) {
            glActiveTexture(GL_TEXTURE0);
            visorTexture.bind();
            glUniform4fv(uColLoc, 1, glm::value_ptr(color));
            glDrawArrays(GL_TRIANGLE_STRIP, 8, 4);
            visorTexture.unbind();

        }
        glActiveTexture(GL_TEXTURE0);
        leftArrowTexture.bind();
        glm::vec4 colorLeft(1.0f, 0.0f, 0.0f, 0.0f);
        glUniform4fv(uColLoc, 1, glm::value_ptr(colorLeft));
        glDrawArrays(GL_TRIANGLE_STRIP, 12, 4);
        leftArrowTexture.unbind();

        glm::mat4 model = glm::mat4(1.0f);

        float x_translation = 0.0;
        float y_translation = -0.45;
        model = glm::translate(model, glm::vec3(x_translation, y_translation, 0.0f));
        float rotation_angle = 180.0f;
        model = glm::rotate(model, glm::radians(rotation_angle), glm::vec3(0.0f, 0.0f, 1.0f)); 
        
        if (blinkActive) {
            int elapsedFrames = frameCounter - blinkStartFrame;

            if (elapsedFrames < 20) {
                if (elapsedFrames % 2 == 0) {
                    glUniform1f(blinkLoc, 1);
                }
                else {
                    glUniform1f(blinkLoc, 0);
                }
            }
            else {
                blinkActive = false;
                blinkStartFrame = 0;
            }

        }


        glm::vec4 colorrig(1.0f, 0.0f, 0.0f, 1.0f);
        glActiveTexture(GL_TEXTURE0);
        leftArrowTexture.bind();
        glUniform4fv(uColLoc, 1, glm::value_ptr(colorrig));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLE_STRIP, 12, 4);
        leftArrowTexture.unbind();

        glBindVertexArray(0);
       
        
        basicShader.stop();

        frameCounter++;

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    endSimulation(&car);

    glDisable(GL_BLEND);

    basicShader.destroy();
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);

    glfwTerminate();
    return 0;
}
