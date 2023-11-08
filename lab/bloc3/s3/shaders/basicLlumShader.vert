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
out vec3 l2;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 TG;

uniform vec3 posFocus;
uniform vec3 posFocusCamera;

// Valors per als components que necessitem del focus de llum

void main()
{	
    matamb2 = matamb;
    matdiff2 = matdiff;
    matspec2 = matspec;
    matshin2 = matshin;
    
    //Cal passar la posició del vertex a SCO
    vertexSCO = view*TG*vec4(vertex,1);

    //Calculem  el vector L respecte el focus de llum escena
    vec4 pFocus = view*vec4(posFocus, 1);
    l = pFocus.xyz - vertexSCO.xyz;
    
    //Calculem  el vector L respecte el focus de llum camera
    pFocus = vec4(posFocusCamera, 1);
    l2 = pFocus.xyz - vertexSCO.xyz;

    //Cal passar el vector normal a SCO 
    mat3 NormalMatrix = inverse(transpose(mat3(view*TG)));
    normalSCO = NormalMatrix*normal;

    gl_Position = proj * view * TG * vec4 (vertex, 1.0);
}
