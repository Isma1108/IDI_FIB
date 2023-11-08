// MyGLWidget.cpp
#include "MyGLWidget.h"
#include <iostream>
#include <stdio.h>
#include "glm/gtc/matrix_inverse.hpp"
#include "glm/glm.hpp"

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


void MyGLWidget::calcula_posFocus() {
  posFocusD0 = glm::vec3(View*TGDisco*glm::vec4(1.2,0,0,1));
  glUniform3fv(posFocusLocD0, 1, &posFocusD0[0]);
  
  posFocusD1 = glm::vec3(View*TGDisco*glm::vec4(-1.2,0,0, 1));
  glUniform3fv(posFocusLocD1, 1, &posFocusD1[0]);
  
  posFocusD2 = glm::vec3(View*TGDisco*glm::vec4(0,0,1.2,1));
  glUniform3fv(posFocusLocD2, 1, &posFocusD2[0]);
  
  posFocusD3 = glm::vec3(View*TGDisco*glm::vec4(0,0,-1.2,1));
  glUniform3fv(posFocusLocD3, 1, &posFocusD3[0]);
}

void MyGLWidget::actualitza_colFocus() {
    if (encendreB) {
      if (indexB == 0) colFocusD0 = glm::vec3(0.4, 0, 0);
      else if (indexB == 1) colFocusD1 = glm::vec3(0, 0.4, 0);
      else if (indexB == 2) colFocusD2 = glm::vec3(0, 0, 0.4);
      else colFocusD3 = glm::vec3(0.4, 0.4, 0);

    }
    else {
      if (indexB == 0) colFocusD0 = glm::vec3(0, 0, 0);
      else if (indexB == 1) colFocusD1 = glm::vec3(0, 0, 0);
      else if (indexB == 2) colFocusD2 = glm::vec3(0, 0, 0);
      else colFocusD3 = glm::vec3(0, 0, 0);
    }

    ++indexB; 
    if (indexB == 4) {indexB = 0; encendreB = not encendreB;}

    glUniform3fv(colFocusLocD0, 1, &colFocusD0[0]); 
    glUniform3fv(colFocusLocD1, 1, &colFocusD1[0]); 
    glUniform3fv(colFocusLocD2, 1, &colFocusD2[0]); 
    glUniform3fv(colFocusLocD3, 1, &colFocusD3[0]); 
}

void MyGLWidget::initializeGL() {
  // Cal inicialitzar l'ús de les funcions d'OpenGL
  initializeOpenGLFunctions();  

  glClearColor(0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)
  glEnable(GL_DEPTH_TEST);
  //glEnable(GL_CULL_FACE);
  
  carregaShaders();
  
  creaBuffersMorty();
  creaBuffersFantasma();
  creaBuffersDiscoBall();
  creaBuffersTerraIParets();

  iniEscena();
  iniCamera();

  angleDisco = 0.0f;
  angleMortyFant = 0.0f;
  focusEscON = true;
  indexB = 0;
  encendreB = false;
  animacioON = false;
  cul = false;
  wire = false;

  colFocusEsc = glm::vec3(0.4, 0.4, 0.4);
  glUniform3fv(colFocusLocEsc, 1, &colFocusEsc[0]);

  colFocusD0 = glm::vec3(0.4, 0, 0);
  glUniform3fv(colFocusLocD0, 1, &colFocusD0[0]); 
  
  colFocusD1 = glm::vec3(0, 0.4, 0);
  glUniform3fv(colFocusLocD1, 1, &colFocusD1[0]); 
  
  colFocusD2 = glm::vec3(0, 0, 0.4);
  glUniform3fv(colFocusLocD2, 1, &colFocusD2[0]); 
  
  colFocusD3 = glm::vec3(0.4, 0.4, 0);
  glUniform3fv(colFocusLocD3, 1, &colFocusD3[0]); 
}

void MyGLWidget::paintGL() {
  // En cas de voler canviar els paràmetres del viewport, descomenteu la crida següent i
  // useu els paràmetres que considereu (els que hi ha són els de per defecte)
  // glViewport (0, 0, ample, alt);
  
  // Esborrem el frame-buffer i el depth-buffer
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // TERRA
  glBindVertexArray (VAO_Terra);
  modelTransformTerra ();
  glDrawArrays(GL_TRIANGLES, 0, 30);
  
  // MORTY
  glBindVertexArray (VAO_Morty);
  modelTransformMorty ();
  glDrawArrays(GL_TRIANGLES, 0, morty.faces().size()*3);
  
  // FANTASMA esquerra
  glBindVertexArray (VAO_Fantasma);
  modelTransformFantasma (1.0f);
  glDrawArrays(GL_TRIANGLES, 0, fantasma.faces().size()*3);
  
  // FANTASMA dreta
  glBindVertexArray (VAO_Fantasma);
  modelTransformFantasma (9.0f);
  glDrawArrays(GL_TRIANGLES, 0, fantasma.faces().size()*3);

  // DISCO BALL
  glBindVertexArray (VAO_DiscoBall);
  modelTransformDiscoBall ();
  glDrawArrays(GL_TRIANGLES, 0, discoBall.faces().size()*3);
  
  glBindVertexArray(0);
}

void MyGLWidget::modelTransformDiscoBall() {
  TGDisco = glm::translate(glm::mat4(1.f), glm::vec3(5,5,5));
  TGDisco = glm::scale(TGDisco, glm::vec3(escalaDiscoBall, escalaDiscoBall, escalaDiscoBall));
  TGDisco = glm::rotate(TGDisco, angleDisco, glm::vec3(0,1,0));
  TGDisco = glm::translate(TGDisco, -centreBaseDiscoBall);

  NormalMatrix = glm::inverseTranspose(glm::mat3(View*TGDisco));
  glUniformMatrix3fv(NormalMatrixLoc, 1, GL_FALSE, &NormalMatrix[0][0]);
  calcula_posFocus();
  
  posFocusEsc = glm::vec3(View*glm::vec4(5,10,5,1));
  glUniform3fv(posFocusLocEsc, 1, &posFocusEsc[0]);

  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TGDisco[0][0]);
}

void MyGLWidget::modelTransformMorty() {
  TG = glm::translate(glm::mat4(1.f), glm::vec3(5,0,5));
  TG = glm::scale(TG, glm::vec3(escalaMorty, escalaMorty, escalaMorty));
  TG = glm::rotate(TG, angleMortyFant, glm::vec3(0,1,0));
  TG = glm::translate(TG, -centreBaseMorty);
  
  NormalMatrix = glm::inverseTranspose(glm::mat3(View*TG));
  glUniformMatrix3fv(NormalMatrixLoc, 1, GL_FALSE, &NormalMatrix[0][0]);
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformFantasma(float posX) {
  TG = glm::translate(glm::mat4(1.0f), glm::vec3(posX,0.5,5));
  TG = glm::scale(TG, glm::vec3(escalaFantasma, escalaFantasma, escalaFantasma));
  TG = glm::rotate(TG, angleMortyFant, glm::vec3(0,1,0));
  TG = glm::translate(TG, -centreBaseFantasma);
  
  NormalMatrix = glm::inverseTranspose(glm::mat3(View*TG));
  glUniformMatrix3fv(NormalMatrixLoc, 1, GL_FALSE, &NormalMatrix[0][0]);

  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformTerra() {
  glm::mat4 TG = glm::mat4(1.f);  // Matriu de transformació
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);

  NormalMatrix = glm::inverseTranspose(glm::mat3(View*TG));
  glUniformMatrix3fv(NormalMatrixLoc, 1, GL_FALSE, &NormalMatrix[0][0]);

}

void MyGLWidget::mouseMoveEvent(QMouseEvent *e)
{
  makeCurrent();
  // Aqui cal que es calculi i s'apliqui la rotacio o el zoom com s'escaigui...
  if (DoingInteractive == ROTATE)
  {
    // Fem la rotació
    angleY += (e->x() - xClick) * M_PI / ample;
    viewTransform ();
  }

  xClick = e->x();
  yClick = e->y();

  update ();
}

void MyGLWidget::keyPressEvent(QKeyEvent* event) {
  makeCurrent();
  switch (event->key()) {
  case Qt::Key_A: {
    angleMortyFant += glm::radians(45.0f);
    break;
	}
  case Qt::Key_D: {
    angleMortyFant -= glm::radians(45.0f);
    break;
	}
  case Qt::Key_E: {
	  focusEscON = not focusEscON;
    if (focusEscON) colFocusEsc = glm::vec3(0.4, 0.4, 0.4);
    else colFocusEsc = glm::vec3(0,0,0);
    glUniform3fv(colFocusLocEsc, 1, &colFocusEsc[0]);
    break;
  }	
  case Qt::Key_B: {
    actualitza_colFocus();
    break;
  }
  case Qt::Key_R: {
    angleDisco += glm::radians(5.0f);
    break;
	}
  case Qt::Key_S: {
    animacioON = not animacioON;
    if (animacioON) timer.start(100);
    else timer.stop();
    break;
	}	
  default: LL4GLWidget::keyPressEvent(event); break;
  }
  update();
}

void MyGLWidget::carregaShaders() {
  LL4GLWidget::carregaShaders();

  NormalMatrixLoc = glGetUniformLocation(program->programId(), "NormalMatrix");

  posFocusLocEsc = glGetUniformLocation(program->programId(), "posFocusEsc"); 
  colFocusLocEsc = glGetUniformLocation(program->programId(), "colFocusEsc"); 
  
  posFocusLocD0 = glGetUniformLocation(program->programId(), "posFocusD0"); 
  colFocusLocD0 = glGetUniformLocation(program->programId(), "colFocusD0"); 
  
  posFocusLocD1 = glGetUniformLocation(program->programId(), "posFocusD1"); 
  colFocusLocD1 = glGetUniformLocation(program->programId(), "colFocusD1"); 
  
  posFocusLocD2 = glGetUniformLocation(program->programId(), "posFocusD2"); 
  colFocusLocD2 = glGetUniformLocation(program->programId(), "colFocusD2"); 

  posFocusLocD3 = glGetUniformLocation(program->programId(), "posFocusD3"); 
  colFocusLocD3 = glGetUniformLocation(program->programId(), "colFocusD3"); 
}

void MyGLWidget::animacio() {
  makeCurrent();   
  actualitza_colFocus();
  angleDisco += glm::radians(5.0f);
  update();
}

MyGLWidget::MyGLWidget(QWidget *parent) : LL4GLWidget(parent) {
    connect(&timer, SIGNAL(timeout()), this, SLOT(animacio()));
}

void MyGLWidget::culling() {
  makeCurrent();
  cul = not cul;
  if (cul) glEnable(GL_CULL_FACE);
  else glDisable(GL_CULL_FACE);
  update();
}

void MyGLWidget::mode_disco() {
  makeCurrent();
  animacioON = not animacioON;
  if (animacioON) timer.start(100);
  else timer.stop();
  update();
}

void MyGLWidget::wireframe() {
  makeCurrent();
  wire = not wire;
  if (wire) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  update();
}









