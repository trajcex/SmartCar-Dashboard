#version 330 core

in vec2 chTex;

out vec4 outTex;

uniform sampler2D uTex;
uniform vec4 uCol;
uniform float mixFactor;

void main()
{ 
        vec4 tex = texture(uTex, chTex);
        if(tex.a <0.1) discard;
        outTex = mix(tex,uCol,mixFactor);
        
  
}