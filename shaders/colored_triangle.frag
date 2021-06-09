#version 450

//Color coming from the vertex shader
layout (location = 0) in vec3 inColor;

//Displayed color
layout (location = 0) out vec4 outFragColor;

void main()
{
    outFragColor = vec4(inColor, 1.0f);
}