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
#include "Render/SpeedHandRenderer.h"
#include "Render/BackgroundRenderer.h"

#include "Vertex/Vertex.h"
#include "ShaderUtils/Utils.h"

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
    glfwSetWindowSizeLimits(window, wWidth, wHeight, wWidth, wHeight);
    glfwSetWindowSizeCallback(window, resizeWindowCallback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    if (glewInit() != GLEW_OK) {
        std::cout << "GLEW nije mogao da se ucita! :'(\n";
        return 3;
    }
    {
        float vreteces[] = {

            //BACKGROUD
            -1.0,1.0 , 0.0,1.0,
            -1.0,-1.0 , 0.0,0.0,
            1.0,1.0 , 1.0,1.0,
            1.0,-1.0 , 1.0,0.0,

            //SPEEDOMETER
            -1.0, 0.0,  0.0, 1.0,
            -1.0, -1.0,    0.0, 0.0,
            1.0, 0.0,   1.0, 1.0,
            1.0, -1.0,    1.0, 0.0,

            //VISOR
            -0.8,1.0 , 0.0,1.0,
            -0.8,0.4 , 0.0,0.0,
            0.8,1.0 , 1.0,1.0,
            0.8,0.4 , 1.0,0.0,

            //LEFT ARROW
            0.5,-0.15 , 0.0,1.0,
            0.5,-0.3 , 0.0,0.0,
            0.65,-0.15 , 1.0,1.0,
            0.65,-0.3 , 1.0,0.0,

            //SPEEDOMETER ARROW
            //45
            -0.46, -0.5 , 0.0,1.0,
            -0.22, -0.5 , 0.0,0.0,
            -0.46, -0.52 , 1.0,1.0,
            -0.22, -0.52 , 1.0,0.0,
            //90
            -0.01, -0.1 , 0.0,1.0,
            -0.01, -0.34 , 0.0,0.0,
            0.01, -0.1 , 1.0,1.0,
            0.01, -0.34 , 1.0,0.0,

            //POWER ON
            0.55, -0.87 , 0.0,1.0,
            0.55, -0.97 , 0.0,0.0,
            0.65, -0.87 , 0.5,1.0,
            0.65, -0.97 , 0.5,0.0,

            //POWER OFF
            0.55, -0.87 , 0.5,1.0,
            0.55, -0.97 , 0.5,0.0,
            0.65, -0.87 , 1.0,1.0,
            0.65, -0.97 , 1.0,0.0,

            /*-0.5, -0.87 , 0.0,1.0,
            -0.5, -0.97 , 0.0,0.0,
            0.5, -0.87 , 1.0,1.0,
            0.5, -0.97 , 1.0,0.0,*/

            //-1.5, -0.87 , 0.0,1.0,
            //-1.5, -0.97 , 0.0,0.0,
            //-0.5, -0.87 , 1.0,1.0,
            //-0.5, -0.97 , 1.0,0.0,


            0.5, -0.87 , 0.0,1.0,
            0.5, -0.97 , 0.0,0.0,
            1.2, -0.87 , 1.0,1.0,
            1.2, -0.97 , 1.0,0.0,
        };
        
        VertexBufferLayout textureLayout;
        textureLayout.push({ 0,2,GL_FLOAT, GL_FALSE ,0 });
        textureLayout.push({ 1,2,GL_FLOAT, GL_FALSE ,2 });
        
        VertexBufferLayout colorLayout;
        colorLayout.push({ 0,2,GL_FLOAT, GL_FALSE ,0 });
        colorLayout.push({ 1,4,GL_FLOAT, GL_FALSE ,2 });

        VertexArray vao;
        VertexBuffer vbo(vreteces, sizeof(vreteces));
        vao.bind();
        vao.addBuffer(vbo, textureLayout);
        vbo.unbind();
        vao.unbind();

        float fuleMask[(NUM_SEGMENTS + 1) * 6];
        float black[4] = { 0.0, 0.0, 0.0, 1.0 };
        drawArc(241.0f, 300.0f, NUM_SEGMENTS, RADIUS, fuleMask, black);
        VertexBuffer fuelVBO(fuleMask, sizeof(fuleMask));
        VertexArray fuelVAO;
        fuelVAO.bind();
        fuelVAO.addBuffer(fuelVBO, colorLayout);
        fuelVBO.unbind();
        fuelVAO.unbind();

        float gasFlowVertices[101 * 6];
        drawArcGradiant(43.0f, 315.0f, 100, 0.19, gasFlowVertices);
        VertexBuffer gasFlowVBO(gasFlowVertices, sizeof(gasFlowVertices));
        VertexArray gasFlowVAO;
        gasFlowVAO.bind();
        gasFlowVAO.addBuffer(gasFlowVBO, colorLayout);
        gasFlowVBO.unbind();
        gasFlowVAO.unbind();
    
        float numbers[10 * 16];
        generateNumber(numbers);
        VertexArray numberTextureVAO;
        VertexBuffer numberTextureVBO(numbers, sizeof(numbers));
        numberTextureVAO.bind();
        numberTextureVAO.addBuffer(numberTextureVBO, textureLayout);
        numberTextureVBO.unbind();
        numberTextureVAO.unbind();


        float circle[(NUM_SEGMENTS + 2) * 6];
        float b[4] = { 1.0, 0.0, 0.0, 0.2 };
        generateCircleVertices(circle, NUM_SEGMENTS, 0.11, b);
        VertexArray circleVAO;
        VertexBuffer circleVBO(circle,sizeof(circle));
        circleVAO.bind();
        circleVAO.addBuffer(circleVBO,colorLayout);
        circleVBO.unbind();
        circleVAO.unbind();

        Shader basicShader((SHADER_PATH + "basic.vert").c_str(), (SHADER_PATH + "basic.frag").c_str());
        Shader mixShader((SHADER_PATH + "basic.vert").c_str(), (SHADER_PATH + "mix.frag").c_str());
        Shader transformShader((SHADER_PATH + "transform.vert").c_str(), (SHADER_PATH + "blink.frag").c_str());
        Shader transformMixShader((SHADER_PATH + "transform.vert").c_str(), (SHADER_PATH + "mix.frag").c_str());
        Shader basicColorShader((SHADER_PATH + "basicCol.vert").c_str(), (SHADER_PATH + "basicCol.frag").c_str());
        Shader blinkShader((SHADER_PATH + "basicCol.vert").c_str(), (SHADER_PATH + "blinkCol.frag").c_str());
        Shader radioShader((SHADER_PATH + "radio.vert").c_str(), (SHADER_PATH + "radio.frag").c_str());

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
        transformShader.setFloat("uTex", 0);
        transformShader.unbind();

        basicShader.bind();
        Texture powerButtonTexture("res/power.png");
        basicShader.setFloat("uTex", 0);
        basicShader.unbind();

        radioShader.bind();
        Texture radioTexture("res/radio.png");
        radioShader.setFloat("uTex", 0);
        radioShader.unbind();


        glClearColor(0.5, 0.5, 0.5, 1.0);

        double previousTime = glfwGetTime();
        startSimulation(&car);
        float MAXX = car.getMaxFuelAmount();
        float maxFuel = car.getMaxFuelAmount();

        float radioPos = 0.0f;
        float radioPosSecondary = 0.0f;
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

            vao.bind();
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

            vao.unbind();
        
            fuelVAO.bind();

            basicColorShader.bind();
            glLineWidth(11.0f);
            //float maxFuel = car.getFuelAmount();
            //std::cout << "FUEL" << maxFuel << std::endl;
            if (frameCounter % 2 == 0) {
                if (maxFuel == 0.0) {
                    maxFuel = 50.0;
                }
                else {
                    maxFuel -= 1;

                }}
        
            int fuel = round(50.0 - maxFuel);

            basicColorShader.setFloat("uRx", 1.0);
            basicColorShader.setFloat("uRy", 1.0);
            basicColorShader.setFloat("uX", 0.0);
            basicColorShader.setFloat("uY", -0.41);
            glDrawArrays(GL_LINE_STRIP, 0, fuel);
            basicColorShader.unbind();
            fuelVAO.unbind();

            gasFlowVAO.bind();
            basicColorShader.bind();
            basicColorShader.setFloat("uRx", 1.0);
            basicColorShader.setFloat("uRy", 0.8);
            basicColorShader.setFloat("uX", -0.68);
            basicColorShader.setFloat("uY", -0.59);
            glLineWidth(6.0f);
            glDrawArrays(GL_LINE_STRIP, 0, accelerator);
            basicColorShader.unbind();
            gasFlowVAO.unbind();

            int gear = car.getGear();
            glm::vec4 blinkColor = getColor(gear);
            blinkColor.a = 0.0;
            if (frameCounter % BLINK_INTERVAL_FRAMES < HALF_BLINK_INTERVAL) blinkColor.a = 0.2;
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            circleVAO.bind();
            blinkShader.bind();
            blinkShader.setFloat("uRx", 1.0);
            blinkShader.setFloat("uRy", 0.8);
            blinkShader.setFloat("uX", 0.675);
            blinkShader.setFloat("uY", -0.595);
            blinkShader.setVec4("uCol", blinkColor);
            glDrawArrays(GL_TRIANGLE_FAN, 0, NUM_SEGMENTS + 2);
            blinkShader.unbind();
            circleVAO.unbind();
            glDisable(GL_BLEND);


            glActiveTexture(GL_TEXTURE0);
            numberTextureVAO.bind();
            mixShader.bind();
            numberTexture.bind();
            mixShader.setFloat("mixFactor", 1);
            mixShader.setVec4("uCol", getColor(gear));
            glDrawArrays(GL_TRIANGLE_STRIP, gear * 4, 4);
            numberTexture.unbind();
            mixShader.unbind();
            numberTextureVAO.unbind();

            vao.bind();
            basicShader.bind();
            glActiveTexture(GL_TEXTURE0);
            powerButtonTexture.bind();
            if (radioOn) {
                glDrawArrays(GL_TRIANGLE_STRIP, 24, 4);
            }else{
                glDrawArrays(GL_TRIANGLE_STRIP, 28, 4);
            }
            powerButtonTexture.unbind();
            basicShader.unbind();
            vao.unbind();

            if (radioOn) {
                vao.bind();
                glActiveTexture(GL_TEXTURE0);
                radioShader.bind();
            
                radioPos -= 0.008;
                if (radioPos < -2) {
                    radioPos  = radioPosSecondary;
                    radioPosSecondary = 0.0;
                }
                radioShader.setFloat("uX", radioPos);
                radioTexture.bind();
                glDrawArrays(GL_TRIANGLE_STRIP, 32, 4);
                
                if (radioPos < -1.0) radioPosSecondary -= 0.008;
                radioShader.setFloat("uX", radioPosSecondary);
                radioTexture.bind();
                glDrawArrays(GL_TRIANGLE_STRIP, 32, 4);


                
                radioTexture.unbind();
                radioShader.unbind();
                vao.unbind();
            }else{
                radioPos = 0.0;
                radioPosSecondary = 0.0;
            }



            frameCounter++;

            glfwSwapBuffers(window);
            glfwPollEvents();
        }

        endSimulation(&car);
    }
    glfwTerminate();
    return 0;
}
