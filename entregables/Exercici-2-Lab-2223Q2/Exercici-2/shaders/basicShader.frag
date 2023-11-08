#version 330 core

in vec3 fcolor;

out vec4 FragColor;

uniform vec3 filtre;
uniform vec3 invisible;

void main() {
	if (invisible[1] != 0) FragColor = vec4(invisible, 1); 
	else FragColor = vec4(fcolor*filtre, 1);
}

