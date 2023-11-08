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

void MyGLWidget::resizeGL(int width, int height) {
  ample = width;
  alt = height;

  ra = float(ample)/float(alt);

  if (!camPlanta && ra < 1) {
    float alpha_nou = atan(tan(alpha_ini)/ra);
    fov = 2*alpha_nou;
  }

  projectTransform();

}

void MyGLWidget::iniEscena ()
{
  creaBuffersPatricio();
  creaBuffersTerra();
  creaBuffersCub();
  creaBuffersEsfera();

  // Paràmetres de l'escena - arbitraris
  pmin = glm::vec3(0,0,0);
  pmax = glm::vec3(30,4,20);
  centreEsc = (pmin+pmax)/2.0f;
  radiEsc = glm::distance(centreEsc, pmin);

  enviaPosFocus();
}

void MyGLWidget::enviaPosFocus()
{
  if (not focus_esc) posFoc = glm::vec3(0,0,0);
  else posFoc = glm::vec3(View*glm::vec4(15,10,10,1));
  glUniform3fv (posfocusLoc, 1, &posFoc[0]);
}

void MyGLWidget::iniCamera ()
{
  angleY = 0.5;
  camPlanta = false;
  ra = 1.0f;
  d = 2*radiEsc;
  alpha_ini = float(asin(radiEsc/d));
  fov = 2*alpha_ini;
  zn = d - radiEsc;
  zf = d + radiEsc;

  projectTransform ();
  viewTransform ();
}

void MyGLWidget::paintGL ()
{
  // Esborrem el frame-buffer i el depth-buffer
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Pintem el terra
  glBindVertexArray (VAO_Terra);
  modelTransformTerra ();
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

  // Pintem el Patricio
  glBindVertexArray (VAO_Patr);
  modelTransformPatricio ();
  glDrawArrays(GL_TRIANGLES, 0, patr.faces().size()*3);

  // Pintem l'arbre
  pintaArbre (glm::vec3(10,0,10));
  pintaArbre (glm::vec3(20,0,5));
  pintaArbre (glm::vec3(25,0,15));

  glBindVertexArray(0);
}

void MyGLWidget::modelTransformPatricio ()
{
  TG = glm::mat4(1.f);
  TG = translate(TG, glm::vec3(10,0,10));
  TG = glm::rotate(TG, -angle_patr, glm::vec3(0,1,0));
  TG = glm::translate(TG, glm::vec3(5,0,0));
  TG = glm::scale(TG, glm::vec3 (2*escala, 2*escala, 2*escala));
  TG = glm::translate(TG, -centreBasePat);
  
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::viewTransform ()
{
  if (!camPlanta) {
    View = glm::translate(glm::mat4(1.f), glm::vec3(0, 0, -d));
    View = glm::rotate(View, angleX, glm::vec3(1,0,0));
    View = glm::rotate(View, -angleY, glm::vec3(0, 1, 0));
    View = glm::translate(View, -centreEsc);

    glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
  }
  else
  {
    View = glm::lookAt(glm::vec3(15,5,10), glm::vec3(15,0,10), glm::vec3(0,0,-1));
    glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
  }
}

void MyGLWidget::projectTransform ()
{
  if (!camPlanta)
    ExamGLWidget::projectTransform();
  else
  {
    glm::mat4 Proj;
    Proj = glm::ortho(-15.0f,15.0f,-10.0f,10.0f,1.0f,6.0f);
    glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
  }
}

void MyGLWidget::keyPressEvent(QKeyEvent* event) 
{
  makeCurrent();
  switch (event->key()) {
  case Qt::Key_Up: {
    angle_patr += M_PI/16;
    break;
	}
  case Qt::Key_F: {
    focus_esc = not focus_esc;
    enviaPosFocus();
    break;
	}
  case Qt::Key_C: {
    camPlanta = not camPlanta;
    viewTransform();
    projectTransform();
    break;
	}
  case Qt::Key_R: {
      // ...
    break;
	}
  default: ExamGLWidget::keyPressEvent(event); break;
  }
  update();
}


void MyGLWidget::mouseMoveEvent(QMouseEvent *e)
{
  makeCurrent();
  if ((DoingInteractive == ROTATE) && !camPlanta)
  {
    // Fem la rotació
    angleY += (e->x() - xClick) * M_PI / ample;
    angleX += (e->y() - yClick) * M_PI /ample;
    viewTransform ();
  }

  xClick = e->x();
  yClick = e->y();

  update ();
}

void MyGLWidget::actualitza_gir(int num) {
  makeCurrent();
  if (num - num_ant > 0) {
    angle_patr += (num - num_ant)*M_PI/16; 
  }
  num_ant = num;
  update();
}



