// MyGLWidget.cpp
#include "MyGLWidget.h"
#include <iostream>
#include <stdio.h>

#define printOpenGLError() printOglError(__FILE__, __LINE__)
#define CHECK() printOglError(__FILE__, __LINE__,__FUNCTION__)
#define DEBUG() std::cout << __FILE__ << " " << __LINE__ << " " << __FUNCTION__ << std::endl;

int MyGLWidget::printOglError(const char file[], int line, const char func[]) 
{
    GLenum glErr;
    int    retCode = 0;

    glErr = glGetError();
    const char * error = 0;
    switch (glErr)
    {
        case 0x0500:
            error = "GL_INVALID_ENUM";
            break;
        case 0x501:
            error = "GL_INVALID_VALUE";
            break;
        case 0x502: 
            error = "GL_INVALID_OPERATION";
            break;
        case 0x503:
            error = "GL_STACK_OVERFLOW";
            break;
        case 0x504:
            error = "GL_STACK_UNDERFLOW";
            break;
        case 0x505:
            error = "GL_OUT_OF_MEMORY";
            break;
        default:
            error = "unknown error!";
    }
    if (glErr != GL_NO_ERROR)
    {
        printf("glError in file %s @ line %d: %s function: %s\n",
                             file, line, error, func);
        retCode = 1;
    }
    return retCode;
}

MyGLWidget::~MyGLWidget() {
}

void MyGLWidget::initializeGL() {
  // Cal inicialitzar l'ús de les funcions d'OpenGL
  initializeOpenGLFunctions();  

  glClearColor(0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)
  glEnable(GL_DEPTH_TEST);
  carregaShaders();
  creaBuffersPatricio();
  creaBuffersTerraIParet();
  
  //Enviamos todos los uniforms con valores por defecto
  //e inicializamos variables.

  posFocus = glm::vec3(0,0.5,0);
  glUniform3fv(posFocusLoc, 1, &posFocus[0]);

  posFocusCamera = glm::vec3(1,1,1);
  glUniform3fv(posFocusCameraLoc, 1, &posFocusCamera[0]);

  colorFocus = glm::vec3(1,1,1);
  glUniform3fv(colorLoc, 1, &colorFocus[0]);

  colorFocusCamera = glm::vec3(0,0,0);
  glUniform3fv(colorCameraLoc, 1, &colorFocusCamera[0]);

  movimentPatricio = glm::vec3(0,0,0);
  culling = false;

  iniEscena();
  iniCamera();

} 

void MyGLWidget::modelTransformPatricio() {
  //Para escalar el patricio como se pide solo es necesario multiplicar x 0.15.
  //ya que sabemos que por la escala da 2, y 2*0.15 = 0.3 que es lo que se pide.

  TG = glm::mat4(1.f);
  TG = translate(TG, movimentPatricio);
  TG = glm::scale(TG, glm::vec3(escala*0.15, escala*0.15, escala*0.15));
  TG = glm::translate(TG, -centrePatr);
  
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}


void MyGLWidget::mouseMoveEvent(QMouseEvent *e)
{
  makeCurrent();
  // Aqui cal que es calculi i s'apliqui la rotacio o el zoom com s'escaigui...
  if (DoingInteractive == ROTATE)
  {
    // Fem la rotació
    angleY += (e->x() - xClick) * M_PI / ample;
    angleX += (e->y() - yClick) * M_PI / ample;
    viewTransform ();
  }

  xClick = e->x();
  yClick = e->y();

  update ();
}

void MyGLWidget::keyPressEvent(QKeyEvent* event) {
  makeCurrent();
  switch (event->key()) {

    //Activacion/desactivacion back-face culling
    case Qt::Key_B :
      culling = not culling;
      if (culling) glEnable(GL_CULL_FACE);
      else glDisable(GL_CULL_FACE);
      break;
    

    //Encender/apagar foco escena (apagar = color rgb(0,0,0) del foco, que es negro)
    case Qt::Key_1 :
      if (colorFocus.x == 0) colorFocus = glm::vec3(0.8,0.8,0.8);
      else colorFocus = glm::vec3(0,0,0);
      break;


    //Encender/apagar foco camara (apagar = color rgb(0,0,0) del foco, que es negro)
    case Qt::Key_2 :
      if (colorFocusCamera.x == 0) colorFocusCamera = glm::vec3(0.8,0.8,0.8);
      else colorFocusCamera = glm::vec3(0,0,0);
      break;

    
    //Teclas para mover el patricio y el foco a la vez.
    case Qt::Key_Left :
      movimentPatricio.x -= 0.05;
      posFocus.x -= 0.05;
      break;
    
    case Qt::Key_Right :
      movimentPatricio.x += 0.05;
      posFocus.x += 0.05;
      break;
    
    case Qt::Key_Up :
      movimentPatricio.z += 0.05;
      posFocus.z += 0.05;
      break;
    
    case Qt::Key_Down :
      movimentPatricio.z -= 0.05;
      posFocus.z -= 0.05;
      break;
  
  //Lo siguiente esta comentado porque es de la sesion 2, donde teniamos que usar tecla f para 
  //cambiar de foco de escena a foco de camara y viceversa, ademas de usar las teclas k y l 
  //para mover el foco (en la sesion 3 estan los dos focos y se activan/desactivan con las teclas 1 y 2)

   /* 
    case Qt::Key_K :
      posFocus.x -= 0.3;
      glUniform3fv(posFocusLoc, 1, &posFocus[0]);
      break;
    
    case Qt::Key_L :
      posFocus.x += 0.3;
      glUniform3fv(posFocusLoc, 1, &posFocus[0]);
      break;

    case Qt::Key_F :
      camera = not camera;
      if (camera) posFocus = glm::vec3(0,0,0);
      else posFocus = glm::vec3(1,1,1);
      break;
    */
    
    default: BL3GLWidget::keyPressEvent(event); break;
  }

  //Hace falta enviar los uniforms que se hayan podido cambiar!! si no no veremos cambio alguno
  glUniform3fv(posFocusLoc, 1, &posFocus[0]);
  glUniform3fv(colorLoc, 1, &colorFocus[0]);
  glUniform3fv(colorCameraLoc, 1, &colorFocusCamera[0]);

  //El update vuelve a llamar a paintGL() por lo que se veran posibles movimientos del patricio ;)
  update();
}

void MyGLWidget::iniMaterialTerra() {
  // Donem valors al material del terra
  amb = glm::vec3(0,0,0.2);
  diff = glm::vec3(0,0,0.9);
  spec = glm::vec3(0.8,0.8,0.8);
  shin = 100;
}

void MyGLWidget::carregaShaders() {
  BL3GLWidget::carregaShaders();
  
  //demanem identificadors per als uniforms del vertex shader
  posFocusLoc = glGetUniformLocation (program->programId(), "posFocus");
  posFocusCameraLoc = glGetUniformLocation (program->programId(), "posFocus2");
  colorLoc = glGetUniformLocation (program->programId(), "colorFocus");
  colorCameraLoc = glGetUniformLocation (program->programId(), "colorFocusCamera");
}



