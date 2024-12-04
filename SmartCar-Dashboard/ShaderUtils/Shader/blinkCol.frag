#version 330 core

in vec2 chTex;

out vec4 outTex;

uniform vec4 uCol;



void main()
{ 
    outTex= uCol;
}