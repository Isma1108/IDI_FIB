#version 330 core

in vec3 vertex;
in vec3 normal;

in vec3 matamb;
in vec3 matdiff;
in vec3 matspec;
in float matshin;

out vec3 matamb2;
out vec3 matdiff2;
out vec3 matspec2;
out float matshin2;

out vec4 vertexSCO;
out vec3 normalSCO;
out vec3 l;
out vec3 colorFocus;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 TG;

uniform vec3 posFocus;
uniform bool camera;

// Valors per als components que necessitem del focus de llum

void main()
{	
    matamb2 = matamb;
    matdiff2 = matdiff;
    matspec2 = matspec;
    matshin2 = matshin;
    
    colorFocus = vec3(0.8, 0.8, 0.8);
    vertexSCO = view*TG*vec4(vertex,1);

    vec4 pF = view*vec4(posFocus, 1);
    if (camera) pF = vec4(posFocus, 1);
    
    l = pF.xyz - vertexSCO.xyz;
    mat3 NormalMatrix = inverse(transpose(mat3(view*TG)));
    normalSCO = NormalMatrix*normal;
    gl_Position = proj * view * TG * vec4 (vertex, 1.0);
}
