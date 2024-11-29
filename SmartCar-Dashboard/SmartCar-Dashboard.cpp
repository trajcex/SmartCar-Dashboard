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

const std::string SHADER_PATH = "ShaderUtils/Shader/";

const double TARGET_FPS = 60.0;
const double FPS = 1.0 / TARGET_FPS;

const double BLINK_DURATION_SECONDS = 3; 
const double BLINK_INTERVAL_SECONDS = 0.6667; 

const int MAX_BLINK_FRAMES = static_cast<int>(BLINK_DURATION_SECONDS * TARGET_FPS);
const int BLINK_INTERVAL_FRAMES = static_cast<int>(BLINK_INTERVAL_SECONDS * TARGET_FPS);
const int HALF_BLINK_INTERVAL = BLINK_INTERVAL_FRAMES / 2;


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


        /*0.5,-0.15 , 0.0,1.0,
        0.5,-0.3 , 0.0,0.0,
        0.65,-0.15 , 1.0,1.0,
        0.65,-0.3 , 1.0,0.0,*/

        -1.0,1.0, 0.0,1.0,
        -1.0,-1.0, 0.0,0.0,
        1.0,1.0, 1.0,1.0,
        1.0,-1.0, 1.0,0.0,

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
    
    Shader mixShader((SHADER_PATH + "basic.vert").c_str(), (SHADER_PATH + "mix.frag").c_str());
    
    Shader transformShader((SHADER_PATH + "transform.vert").c_str(), (SHADER_PATH + "blink.frag").c_str());
    mixShader.use();
    Texture mapTexture("res/map.jpg");
    basicShader.setFloat("uTex", 0);
    mixShader.stop();


    basicShader.use();
    Texture checkerTexture("res/speedometer.png");
    basicShader.setFloat("uTex", 0);


    Texture visorTexture("res/visor.png");
    basicShader.setFloat("uTex", 0);
    basicShader.stop();

    transformShader.use();
    Texture leftArrowTexture("res/arrow.png");
    basicShader.setFloat("uTex", 0);
    transformShader.stop();

    glClearColor(0.5, 0.5, 0.5, 1.0);
    



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

        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(VAO);



        //BACKGROUND
        mixShader.use();
        mixShader.setFloat("mixFactor", 0.15);
        glm::vec4 color1(0.0f, 0.0f, 1.0f, 1.0f); 
        mixShader.setVec4("uCol", color1);
        glActiveTexture(GL_TEXTURE0);
        mapTexture.bind();
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        mapTexture.unbind();
        mixShader.stop();


        //SPEEDOMETER
        basicShader.use();
        glActiveTexture(GL_TEXTURE0);
        checkerTexture.bind();
        glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);
        checkerTexture.unbind();

        //VISOR
        if (drawVisor) {
            glActiveTexture(GL_TEXTURE0);
            visorTexture.bind();
            glDrawArrays(GL_TRIANGLE_STRIP, 8, 4);
            visorTexture.unbind();
        }
        basicShader.stop();

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glActiveTexture(GL_TEXTURE0);
        
        transformShader.use();
        leftArrowTexture.bind();
        
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.65f, -0.25f, 0.0f));
        model = glm::scale(model, glm::vec3(0.1f, 0.1f, 1.0f));

        if (rightBlinkActive) {
            int elapsedFrames = frameCounter - rightBlinkStartFrame;

            if (elapsedFrames < MAX_BLINK_FRAMES) {
                if (elapsedFrames % BLINK_INTERVAL_FRAMES < HALF_BLINK_INTERVAL) {
                    transformShader.setBool("isBlink", false);
                }
                else {
                    transformShader.setBool("isBlink", true);
                }
            }
            else {
                transformShader.setBool("isBlink", false);
                rightBlinkActive = false;
                rightBlinkStartFrame = 0;
            }
        }
        glm::vec4 color(1.0f, 0.5f, 0.0f, 1.0f);
        transformShader.setVec4("uCol", color);
        transformShader.setMat4("model", model);
        transformShader.setFloat("mixFactor", 0.1);
        glDrawArrays(GL_TRIANGLE_STRIP, 12, 4);
        transformShader.setBool("isBlink", false);


        if (letfBlinkActive) {
            int elapsedFrames = frameCounter - leftBlinkStartFrame;

            if (elapsedFrames < MAX_BLINK_FRAMES) {
                if (elapsedFrames % BLINK_INTERVAL_FRAMES < HALF_BLINK_INTERVAL) {
                    transformShader.setBool("isBlink", false);
                }
                else {
                    transformShader.setBool("isBlink", true);
                }
            }
            else {
                transformShader.setBool("isBlink", false);
                letfBlinkActive = false;
                leftBlinkStartFrame = 0;
            }
        }
        glm::mat4 model1 = glm::mat4(1.0f);
        model1 = glm::translate(model1, glm::vec3(-0.65f, -0.25f, 0.0f));
        model1 = glm::scale(model1, glm::vec3(0.1f, 0.1f, 1.0f));
        float rotation_angle = 180.0f;
        model1 = glm::rotate(model1, glm::radians(rotation_angle), glm::vec3(0.0f, 0.0f, 1.0f));

        transformShader.setMat4("model", model1);
        transformShader.setFloat("mixFactor", 0.1);
        glDrawArrays(GL_TRIANGLE_STRIP, 12, 4);
        transformShader.setBool("isBlink", false);

        leftArrowTexture.unbind();
        transformShader.stop();
        glDisable(GL_BLEND);
        

        glBindVertexArray(0);
       
        frameCounter++;

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    endSimulation(&car);

    //glDisable(GL_BLEND);

    basicShader.destroy();
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);

    glfwTerminate();
    return 0;
}
