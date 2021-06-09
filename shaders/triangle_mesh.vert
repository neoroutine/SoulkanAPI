#version 450

//Vertex input
layout (location = 0) in vec3 vecPosition;
layout (location = 1) in vec3 vecNormal;
layout (location = 2) in vec3 vecColor;

//Color going to the fragment shader
layout (location = 0) out vec3 outColor;

void main()
{
    gl_Position = vec4(vecPosition, 1.0f);
    outColor    = vecColor;
}