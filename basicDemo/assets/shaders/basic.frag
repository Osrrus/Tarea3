#version 330 core

// Fragment Color

in vec3 offsetPos;

out vec4 color;

void main()
{
    color = vec4(offsetPos,1.0f);
}