#include "BackgroundRenderer.h"

BackgroundRenderer::BackgroundRenderer(Shader& s, Texture& t) : BaseRenderer(s, t) {}

void BackgroundRenderer::render(){
    shader.bind();
    
    shader.setFloat("mixFactor", 0.15);
    shader.setVec4("uCol", glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
    
    glActiveTexture(GL_TEXTURE0);
    texture.bind();
    
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    
    texture.unbind();
    shader.unbind();
}


