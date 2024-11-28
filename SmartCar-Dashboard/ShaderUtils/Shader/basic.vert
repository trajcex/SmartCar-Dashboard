#version 330 core

layout(location = 0) in vec3 inPoc;
layout(location = 1) in vec3 inCol;

out vec3 chCol;

uniform mat4 model;

uniform mat4 projection;  

void main(){
	gl_Position =   model * vec4(inPoc.x,inPoc.y, 0.0, 1.0);
	chCol = inCol;
}