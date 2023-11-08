#version 330 core

out vec4 FragColor;

in vec3 matamb2;
in vec3 matdiff2;
in vec3 matspec2;
in float matshin2;

in vec4 vertexSCO;
in vec3 normalSCO;
in vec3 l;
in vec3 colorFocus;

vec3 llumAmbient = vec3(0.2, 0.2, 0.2);

vec3 Ambient() {
    return llumAmbient * matamb2;
}

vec3 Difus (vec3 NormSCO, vec3 L, vec3 colFocus) 
{
    // Tant sols retorna el terme difús
    // S'assumeix que els vectors que es reben com a paràmetres estan normalitzats
    vec3 colRes = vec3(0);
    // Càlcul component difusa, si n'hi ha
    if (dot (L, NormSCO) > 0)
      colRes = colFocus * matdiff2 * dot (L, NormSCO);
    return (colRes);
}

vec3 Especular (vec3 NormSCO, vec3 L, vec4 vertSCO, vec3 colFocus) 
{
    // Tant sols retorna el terme especular!
    // Els vectors rebuts com a paràmetres (NormSCO i L) estan normalitzats
    vec3 colRes = vec3 (0);
    // Si la llum ve de darrera o el material és mate no fem res
    if ((dot(NormSCO,L) < 0) || (matshin2 == 0))
      return colRes;  // no hi ha component especular

    // Calculem R i V
    vec3 R = reflect(-L, NormSCO); // equival a: 2.0*dot(NormSCO,L)*NormSCO - L;
    vec3 V = normalize(-vertSCO.xyz); // perquè la càmera està a (0,0,0) en SCO

    if (dot(R, V) < 0)
      return colRes;  // no hi ha component especular
    
    // Calculem i retornem la component especular
    float shine = pow(max(0.0, dot(R, V)), matshin2);
    return (matspec2 * colFocus * shine); 
}

void main()
{	
    vec3 n = normalize(normalSCO); 
    vec3 l2 = normalize(l);
    vec3 fcolor = Ambient() + Difus(n, l2, colorFocus) + Especular(n, l2, vertexSCO, colorFocus);
    FragColor = vec4(fcolor, 1);
}
