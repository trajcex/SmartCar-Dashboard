#version 330 core

in vec2 chTex;

out vec4 outTex;

uniform sampler2D uTex;

void main()
{
	outTex = texture(uTex, chTex);
	if (outTex.a<0.1){
		discard;
	}
}