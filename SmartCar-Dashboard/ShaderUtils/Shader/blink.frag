#version 330 core

in vec2 chTex;

out vec4 outTex;

uniform sampler2D uTex;
uniform vec4 uCol;
uniform int isBlink;


void main()
{ 
    vec4 tex = texture(uTex, chTex);
    if (isBlink == 1){
        if (tex.a < 0.1)discard;
        tex = mix(tex,uCol,1);
    }
    outTex= tex;
}