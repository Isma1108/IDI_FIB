#version 330 core

in vec3 vertex;
in vec3 color;
out vec3 FColor;

void main()  {
    gl_Position = vec4 (0.5*vertex, 1.0);
    FColor = color;
}
