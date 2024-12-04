#version 330 core

layout(location = 0) in vec2 inPoc;
layout(location = 1) in vec2 inTex;

out vec2 chTex;
uniform float uX;
void main(){
	gl_Position =  vec4(inPoc.x+uX,inPoc.y, 0.0, 1.0);
	chTex = inTex;
}