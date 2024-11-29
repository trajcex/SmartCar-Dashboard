#version 330 core

layout(location = 0) in vec2 inPoc;
layout(location = 1) in vec2 inTex;

out vec2 chTex;

uniform mat4 model;

void main(){
	gl_Position =  model * vec4(inPoc.x,inPoc.y, 0.0, 1.0);
	chTex = inTex;
}