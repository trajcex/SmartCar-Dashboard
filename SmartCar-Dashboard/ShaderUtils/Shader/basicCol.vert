#version 330 core 

layout(location = 0) in vec2 inPos;
layout(location = 1) in vec4 inCol;

out vec4 chCol;
uniform float uX;
uniform float uY;

uniform float uRx;
uniform float uRy;

void main(){
	gl_Position = vec4(inPos.x*uRx+uX,inPos.y*uRy+uY, 0.0, 1.0);
	chCol = inCol;

}
