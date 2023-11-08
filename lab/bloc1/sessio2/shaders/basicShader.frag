#version 330 core

in vec3 FColor;
out vec4 FragColor;

void main() {
    //FragColor = vec4(0, 0, 0, 1);
    FragColor = vec4(FColor,1.0);
   
    if (int(gl_FragCoord.y)/11 % 2 == 0 || int(gl_FragCoord.x)/11 % 2 == 0) discard;
    
    /*else if (gl_FragCoord.x < 395. && gl_FragCoord.y > 400.) FragColor = vec4(1.,0.,0.,1);
    else if (gl_FragCoord.x >= 395. && gl_FragCoord.y > 400.) FragColor = vec4(0.,0.,1.,1);
    else if (gl_FragCoord.x > 395. && gl_FragCoord.y <= 400.) FragColor = vec4(0.,1.,0.,1);
    else FragColor = vec4(1.,1.,0.,1);
    */
}

