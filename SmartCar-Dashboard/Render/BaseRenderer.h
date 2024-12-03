#ifndef BASE_RENDERER_H
#define BASE_RENDERER_H

#include "../ShaderUtils/Shader.h"
#include "../ShaderUtils/Texture.h"

class BaseRenderer {
protected: 
    Shader& shader;
    Texture& texture;
public:
    BaseRenderer(Shader& s, Texture& t) : shader(s), texture(t) {}
    virtual ~BaseRenderer() = default;
    virtual void render() = 0;
};
#endif
