#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES 
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>

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
#include "Render/SpeedHandRenderer.h"
#include "Render/BackgroundRenderer.h"



const std::string SHADER_PATH = "ShaderUtils/Shader/";

const double TARGET_FPS = 60.0;
const double FPS = 1.0 / TARGET_FPS;

const double BLINK_DURATION_SECONDS = 3; 
const double BLINK_INTERVAL_SECONDS = 0.6667; 

const int MAX_BLINK_FRAMES = static_cast<int>(BLINK_DURATION_SECONDS * TARGET_FPS);
const int BLINK_INTERVAL_FRAMES = static_cast<int>(BLINK_INTERVAL_SECONDS * TARGET_FPS);
const int HALF_BLINK_INTERVAL = BLINK_INTERVAL_FRAMES / 2;

const int NUM_SEGMENTS = 50;
const float RADIUS = 0.4;

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
        number[index++] = x2; number[index++] = y1; number[index++] = z+0.1f;   number[index++] = 1.0f;
        number[index++] = x2; number[index++] = y2; number[index++] = z+0.1f;   number[index++] = 0.0f;
    }

}
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
    {
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

        -0.46, -0.5 , 0.0,1.0,
        -0.22, -0.5 , 0.0,0.0,
        -0.46, -0.52 , 1.0,1.0,
        -0.22, -0.52 , 1.0,0.0,

        -0.01, -0.1 , 0.0,1.0,
        -0.01, -0.34 , 0.0,0.0,
        0.01, -0.1 , 1.0,1.0,
        0.01, -0.34 , 1.0,0.0,

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

    float vertices[(NUM_SEGMENTS + 1) * 6];
    float color[4] = { 0.0, 0.0, 0.0, 1.0 };
    drawArc(241.0f, 300.0f, NUM_SEGMENTS, RADIUS, vertices, color);
    unsigned int strid1 = (2 + 4) * sizeof(float);

    unsigned int VAO1;
    glGenVertexArrays(1, &VAO1);
    glBindVertexArray(VAO1);

    unsigned int VBO1;
    glGenBuffers(1, &VBO1);
    glBindBuffer(GL_ARRAY_BUFFER, VBO1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, strid1, (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, strid1, (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    float vertices1[101 * 6];
    float color1[4] = { 1.0, 0.0, 0.0, 1.0 };
    drawArcGradiant(43.0f, 315.0f, 100, 0.19, vertices1);

    unsigned int VAO2;
    glGenVertexArrays(1, &VAO2);
    glBindVertexArray(VAO2);

    unsigned int VBO2;
    glGenBuffers(1, &VBO2);
    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, strid1, (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, strid1, (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


    float numbers[10 * 16];
    generateNumber(numbers);
    unsigned int VAO3;
    glGenVertexArrays(1, &VAO3);
    glBindVertexArray(VAO3);

    unsigned int VBO3;
    glGenBuffers(1, &VBO3);
    glBindBuffer(GL_ARRAY_BUFFER, VBO3);
    glBufferData(GL_ARRAY_BUFFER, sizeof(numbers), numbers, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, stride, (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    Shader basicShader((SHADER_PATH + "basic.vert").c_str(), (SHADER_PATH + "basic.frag").c_str());
    Shader mixShader((SHADER_PATH + "basic.vert").c_str(), (SHADER_PATH + "mix.frag").c_str());
    Shader transformShader((SHADER_PATH + "transform.vert").c_str(), (SHADER_PATH + "blink.frag").c_str());
    Shader transformMixShader((SHADER_PATH + "transform.vert").c_str(), (SHADER_PATH + "mix.frag").c_str());

    Shader basicPROBA((SHADER_PATH + "basicCol.vert").c_str(), (SHADER_PATH + "basicCol.frag").c_str());

    mixShader.bind();
    Texture mapTexture("res/map.jpg");
    BackgroundRenderer backgroundRenderer(mixShader, mapTexture);
    mixShader.setFloat("uTex", 0);
    mixShader.unbind();


    basicShader.bind();
    Texture speedHandTexture("res/speed_hand.png");
    SpeedHandRenderer speedHandRenderer(transformMixShader, speedHandTexture);
    basicShader.setFloat("uTex", 0);


    Texture checkerTexture("res/speedometer.png");
    basicShader.setFloat("uTex", 0);

    Texture numberTexture("res/number.png");
    basicShader.setFloat("uTex", 0);

    Texture visorTexture("res/visor.png");
    basicShader.setFloat("uTex", 0);
    basicShader.unbind();

    transformShader.bind();
    Texture leftArrowTexture("res/arrow.png");
    basicShader.setFloat("uTex", 0);
    transformShader.unbind();

    glClearColor(0.5, 0.5, 0.5, 1.0);




    double previousTime = glfwGetTime();
    startSimulation(&car);
    float MAXX = car.getMaxFuelAmount();
    float maxFuel = car.getMaxFuelAmount();
    std::cout << "MAX FUEL" << MAXX << std::endl;

    while (!glfwWindowShouldClose(window)) {

        double currentTime = glfwGetTime();
        double elapsedTime = currentTime - previousTime;

        if (elapsedTime < FPS) {
            glfwWaitEventsTimeout(FPS - elapsedTime);
            continue;
        }
        previousTime = currentTime;

        if (glfwGetKey(window, GLFW_KEY_G) != GLFW_PRESS)
        {
            accelerator -= 1;
            if (accelerator < 0) accelerator = 0;
        }

        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(VAO);



        //BACKGROUND
        backgroundRenderer.render();

        //SPEEDOMETER
        basicShader.bind();
        glActiveTexture(GL_TEXTURE0);
        checkerTexture.bind();
        glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);
        checkerTexture.unbind();

        float speed = car.getSpeed();
        //std::cout << "Speed: " << speed << std::endl;
        speedHandRenderer.setSpeed(speed);
        speedHandRenderer.render();

        //VISOR
        if (drawVisor) {
            basicShader.bind();
            //std::cout << "VISOR" << std::endl;
            glActiveTexture(GL_TEXTURE0);
            visorTexture.bind();
            glDrawArrays(GL_TRIANGLE_STRIP, 8, 4);
            visorTexture.unbind();
            basicShader.unbind();
        }
        basicShader.unbind();

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glActiveTexture(GL_TEXTURE0);

        transformShader.bind();
        leftArrowTexture.bind();

        glm::mat4 model = glm::mat4(1.0f);
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
        float rotation_angle = -180.0f;
        model1 = glm::rotate(model, glm::radians(rotation_angle), glm::vec3(0.0f, 1.0f, 0.0f));

        transformShader.setMat4("model", model1);
        transformShader.setFloat("mixFactor", 0.1);
        glDrawArrays(GL_TRIANGLE_STRIP, 12, 4);
        transformShader.setBool("isBlink", false);
        transformShader.unbind();
        glDisable(GL_BLEND);

        glBindVertexArray(0);

        glBindVertexArray(VAO1);

        basicPROBA.bind();
        glLineWidth(11.0f);
        float maxFuel = car.getFuelAmount();
        //std::cout << "FUEL" << maxFuel << std::endl;
        /*if (frameCounter % 2 == 0) {
            if (maxFuel == 0.0) {
                maxFuel = 50.0;
            }
            else {
                maxFuel -= 1;

            }}
        */
        int fuel = round(50.0 - maxFuel);

        basicPROBA.setFloat("uRx", 1.0);
        basicPROBA.setFloat("uRy", 1.0);
        basicPROBA.setFloat("uX", 0.0);
        basicPROBA.setFloat("uY", -0.41);
        glDrawArrays(GL_LINE_STRIP, 0, fuel);
        basicPROBA.unbind();
        glBindVertexArray(0);


        glBindVertexArray(VAO2);
        basicPROBA.bind();
        glLineWidth(6.0f);
        basicPROBA.setFloat("uRx", 1.0);
        basicPROBA.setFloat("uRy", 0.8);
        basicPROBA.setFloat("uX", -0.68);
        basicPROBA.setFloat("uY", -0.59);
        glDrawArrays(GL_LINE_STRIP, 0, accelerator);
        basicPROBA.unbind();
        glBindVertexArray(0);


        glBindVertexArray(VAO3);
        mixShader.bind();
        numberTexture.bind();
        mixShader.setFloat("mixFactor", 1);
        int gear = car.getGear();
        mixShader.setVec4("uCol", getColor(gear));

        glDrawArrays(GL_TRIANGLE_STRIP, gear * 4, 4);
        numberTexture.unbind();
        mixShader.unbind();
        glBindVertexArray(0);

        frameCounter++;

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    endSimulation(&car);

    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
}
    glfwTerminate();
    return 0;
}
