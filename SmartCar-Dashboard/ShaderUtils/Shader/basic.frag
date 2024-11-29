#version 330 core

in vec2 chCol;

out vec4 outCol;

uniform sampler2D uTex;
uniform vec4 uCol;
uniform float blink;

void main()
{
	vec4 texColor = texture(uTex, chCol);
    if (uCol.a>0.1) {
        texColor = mix(texColor,uCol,0.15);
    }

    if(texColor.a < 0.1)
        discard;
    outCol = texColor;
}