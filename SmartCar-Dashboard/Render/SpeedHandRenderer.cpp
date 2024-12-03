#include "SpeedHandRenderer.h"

SpeedHandRenderer::SpeedHandRenderer( Shader& s, Texture& t)
    : BaseRenderer(s, t), center(glm::vec3(0.0)), speed(0.0f) {}

float SpeedHandRenderer::getSpeed() const {
    return speed;
}

void SpeedHandRenderer::setSpeed(float newSpeed) {
    speed = newSpeed;
}

void SpeedHandRenderer::render() {
        
        shader.bind();
        float rotation_angle=0;
        int drawStartIndex;
        int drawCount = 4; 

        if (speed < 30)
        {
            shader.setVec4("uCol", glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
            shader.setFloat("mixFactor", 1.0);
        }
        else {
            shader.setFloat("mixFactor", 0.0);
        }

        if (speed <= 20) {
            center.y = -0.51;
            rotation_angle = 30 - (speed * 1.5);
            drawStartIndex = 16;
        }
        else if (speed > 20 && speed <= 40) {
            center.y = -0.51;
            rotation_angle = 30 - (speed * 1.3);
            drawStartIndex = 16;
        }
        else {
            center.y = -0.53;
            rotation_angle = (70 - speed) * 1.9;
            drawStartIndex = 20;
        }

        glm::mat4 modelSpeed = glm::mat4(1.0f);
        modelSpeed = glm::translate(modelSpeed, center);
        modelSpeed = glm::rotate(modelSpeed, glm::radians(rotation_angle), glm::vec3(0.0f, 0.0f, 1.0f));
        modelSpeed = glm::translate(modelSpeed, -center);

        glActiveTexture(GL_TEXTURE0);
        shader.setMat4("model", modelSpeed);
        texture.bind();
        glDrawArrays(GL_TRIANGLE_STRIP, drawStartIndex, drawCount);
        texture.unbind();
        shader.unbind();
}