#version 330 core
// Atributte 0 of the vertex
layout (location = 0) in vec3 vertexPosition;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

out vec3  offsetPos;

void main()
{
    offsetPos = vertexPosition + 0.5f;
    gl_Position = Projection * View * Model * vec4(vertexPosition, 1.0f);
}