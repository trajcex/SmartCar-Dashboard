#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glew.h>   
#include <string>
#include <iostream>

class Texture {
public:
    unsigned int ID;

    Texture(const std::string& filepath, GLenum wrapS = GL_REPEAT, GLenum wrapT = GL_REPEAT,
        GLenum minFilter = GL_NEAREST, GLenum magFilter = GL_NEAREST);
    ~Texture();

    void bind() const;
    void unbind() const;

    static unsigned int loadImageToTexture(const char* filepath);

private:
    void setTextureParameters(GLenum wrapS, GLenum wrapT, GLenum minFilter, GLenum magFilter);
};

#endif
