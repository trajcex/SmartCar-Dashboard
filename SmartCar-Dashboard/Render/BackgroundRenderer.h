#pragma once
#ifndef BACKGROUND_RENDERER_H
#define BACKGROUND_RENDERER_H

#include "BaseRenderer.h"


class BackgroundRenderer : public BaseRenderer {
public:
    BackgroundRenderer(Shader& s, Texture& t);

    void render() override;
};

#endif 