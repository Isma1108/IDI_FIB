#version 330 core

in vec3  fmatamb;
in vec3  fmatdiff;
in vec3  fmatspec;
in float fmatshin;
in vec3  fvertex;
in vec3  fnormal;

const vec3 llumAmbient = vec3(0.1, 0.1, 0.1);

uniform mat4 TG;
uniform mat4 Proj;
uniform mat4 View;

uniform vec3 posFocusEsc;
uniform vec3 colFocusEsc;

uniform vec3 posFocusD0;
uniform vec3 colFocusD0;

uniform vec3 posFocusD1;
uniform vec3 colFocusD1;

uniform vec3 posFocusD2;
uniform vec3 colFocusD2;

uniform vec3 posFocusD3;
uniform vec3 colFocusD3;

out vec4 FragColor;


vec3 Ambient() {
  return llumAmbient*fmatamb;
}

vec3 Difus (vec3 NormSCO, vec3 L, vec3 colFocus)
{
  // Fixeu-vos que SOLS es retorna el terme de Lambert!
  // S'assumeix que els vectors que es reben com a paràmetres estan normalitzats
  vec3 colRes = vec3(0);
  if (dot (L, NormSCO) > 0)
    colRes = colFocus * fmatdiff * dot (L, NormSCO);
  return (colRes);
}

vec3 Especular (vec3 NormSCO, vec3 L, vec3 vertSCO, vec3 colFocus)
{
  // Fixeu-vos que SOLS es retorna el terme especular!
  // Assumim que els vectors estan normalitzats
  vec3 colRes = vec3 (0);
  // Si la llum ve de darrera o el material és mate no fem res
  if ((dot(NormSCO,L) < 0) || (fmatshin == 0))
    return colRes;  // no hi ha component especular

  vec3 R = reflect(-L, NormSCO); // equival a: 2.0*dot(NormSCO,L)*NormSCO - L;
  vec3 V = normalize(-vertSCO); // perquè la càmera està a (0,0,0) en SCO

  if (dot(R, V) < 0)
    return colRes;  // no hi ha component especular

  float shine = pow(max(0.0, dot(R, V)), fmatshin);
  return (colRes + fmatspec * colFocus * shine);
}

void main()
{
    vec3 normal_norm = normalize(fnormal);
    
    vec3 lesc = normalize(posFocusEsc - fvertex);
    vec3 ld0 = normalize(posFocusD0 - fvertex);
    vec3 ld1 = normalize(posFocusD1 - fvertex);
    vec3 ld2 = normalize(posFocusD2 - fvertex);
    vec3 ld3 = normalize(posFocusD3 - fvertex);

    vec3 color = Ambient() + Difus(normal_norm, lesc, colFocusEsc) + Especular(normal_norm, lesc, fvertex, colFocusEsc);
    color += Difus(normal_norm, ld0, colFocusD0) + Especular(normal_norm, ld0, fvertex, colFocusD0);
    color += Difus(normal_norm, ld1, colFocusD1) + Especular(normal_norm, ld1, fvertex, colFocusD1);
    color += Difus(normal_norm, ld2, colFocusD2) + Especular(normal_norm, ld2, fvertex, colFocusD2);
    color += Difus(normal_norm, ld3, colFocusD3) + Especular(normal_norm, ld3, fvertex, colFocusD3);

    FragColor = vec4(color, 1);
}
