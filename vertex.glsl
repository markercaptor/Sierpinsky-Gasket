#version 400 core
 in vec3 vertexPosition_modelspace;
 in vec3 vertexColor;
uniform mat4 objectToWorld;

void main()
{
	gl_Position = objectToWorld*vec4(vertexPosition_modelspace,1.0);
	

	
}

