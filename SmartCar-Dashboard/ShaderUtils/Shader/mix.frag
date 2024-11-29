#version 330 core

in vec2 chTex;

out vec4 outTex;

uniform sampler2D uTex;
uniform vec4 uCol;
uniform float mixFactor;

void main()
{ 
        outTex = mix(texture(uTex, chTex),uCol,mixFactor);
  
}