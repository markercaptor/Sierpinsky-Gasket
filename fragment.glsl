#version 400 core
uniform vec3 colors;
out vec4 color;

void main()
{
	color=vec4(colors,1);
}