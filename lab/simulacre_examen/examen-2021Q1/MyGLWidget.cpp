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
  ExamGLWidget::initializeGL();
  emit persp();
  emit cub1();
}

void MyGLWidget::resizeGL(int w, int h) {
  ample = w;
  alt = h;
  ra = float(w)/float(h);
    
  if (ra < 1 && !camPlanta) {
    float alpha_ini = float(M_PI/3.0)/2.0f;
    fov = 2*atan(tan(alpha_ini/ra)); 
  }
  projectTransform();
}

void MyGLWidget::paintGL ()   // Mètode que has de modificar
{
  // En cas de voler canviar els paràmetres del viewport, descomenteu la crida següent i
  // useu els paràmetres que considereu (els que hi ha són els de per defecte)
  //  glViewport (0, 0, ample, alt);
  
  // Esborrem el frame-buffer i el depth-buffer
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Pintem el terra
  glBindVertexArray (VAO_Terra);
  modelTransformTerra ();
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

  if (not pinta_cubs) {
    // Pintem el Patricio
    glBindVertexArray (VAO_Patr);
    modelTransformPatricio ();
    glDrawArrays(GL_TRIANGLES, 0, patr.faces().size()*3);
  }
  
  else {
    // Pintem el cub 1
    glBindVertexArray(VAO_Cub);
    modelTransformCub (4.0, 0.0 + desfase);
    glDrawArrays(GL_TRIANGLES, 0, 36);
  
    // Pintem el cub 2
    glBindVertexArray(VAO_Cub);
    modelTransformCub (5.0, 2*M_PI/3 + desfase);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Pintem el cub 3
    glBindVertexArray(VAO_Cub);
    modelTransformCub (6.0, 4*M_PI/3 + desfase);
    glDrawArrays(GL_TRIANGLES, 0, 36);
  
    glBindVertexArray(0);
  }
}

void MyGLWidget::modelTransformCub (float escala, float angle) 
{
  TG = glm::mat4(1.f);
  TG = rotate(TG, angle, glm::vec3(0,1,0));
  TG = translate(TG, glm::vec3(5,0,0));
  TG = scale(TG, glm::vec3(escala,escala,escala));
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformPatricio ()    // Mètode que has de modificar
{
  TG = glm::mat4(1.f);
  TG = glm::rotate(TG, angle_patr + desfase, glm::vec3(0,1,0));
  TG = glm::translate(TG, glm::vec3(5,0,0));
  TG = rotate(TG, -glm::radians(90.0f), glm::vec3(0,1,0));
  TG = glm::scale(TG, glm::vec3 (2*escala, 2*escala, 2*escala));
  TG = glm::translate(TG, -centreBasePat);
  
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::viewTransform ()    // Mètode que has de modificar
{
  if (!camPlanta) {
    ExamGLWidget::viewTransform();
  }
  else
  {
    View = glm::lookAt(glm::vec3(0.f,5.f,0.f), glm::vec3(0.f,0.f,0.f), glm::vec3(1.f,0.f,0.f));
    glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
  }
}

void MyGLWidget::projectTransform ()
{
  if (!camPlanta)
    ExamGLWidget::projectTransform();
  else
  {
    glm::mat4 Proj = glm::ortho(-10.f, 10.f, -10.f, 10.f, 2.0f, 6.0f);
    glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
  }
}

void MyGLWidget::keyPressEvent(QKeyEvent* event) {
  makeCurrent();
  switch (event->key()) {
  case Qt::Key_V: {
    pinta_cubs = not pinta_cubs;
    break;
	}
  case Qt::Key_1: {
    angle_patr = angle_cub1;
    emit cub1();
    break;
	}
  case Qt::Key_2: {
    angle_patr = angle_cub2;
    emit cub2();
    break;
	}
  case Qt::Key_3: {
    angle_patr = angle_cub3;
    emit cub3();
    break;
	}
  case Qt::Key_F: {
    if (colFoc[2] != 1) colFoc = glm::vec3(1,1,1);
    else colFoc = glm::vec3(1,1,0);
    enviaColFocus();
    break;
	}
  case Qt::Key_C: {
    camPlanta = not camPlanta;
    if (camPlanta) emit orto();
    else emit persp();
    viewTransform();
    projectTransform();
    break;
	}
  case Qt::Key_Right: {
    desfase += 2*M_PI/3;
    break;
	}
  case Qt::Key_Left: {
    desfase -= 2*M_PI/3;
    break;
	}
  case Qt::Key_R: {
    reset();
    break;
	}
  default: ExamGLWidget::keyPressEvent(event); break;
  }
  update();
}

void MyGLWidget::tracta_ortogonal() {
  makeCurrent();
  camPlanta = true;
  viewTransform();
  projectTransform();
  update();
}

void MyGLWidget::tracta_perspectiva() {
  makeCurrent();
  camPlanta = false;
  viewTransform();
  projectTransform();
  update();
}

void MyGLWidget::tracta_cub1() {
  makeCurrent(); 
  angle_patr = angle_cub1;
  viewTransform();
  projectTransform();
  update();
}

void MyGLWidget::tracta_cub2() {
  makeCurrent(); 
  angle_patr = angle_cub2;
  viewTransform();
  projectTransform();
  update();
}

void MyGLWidget::tracta_cub3() {
  makeCurrent(); 
  angle_patr = angle_cub3;
  viewTransform();
  projectTransform();
  update();
}

void MyGLWidget::reset() {
  makeCurrent();
  angle_cub1 = angle_patr = 0.f;
  angle_cub2 = 2*M_PI/3;
  angle_cub3 = 4*M_PI/3;
  desfase = 0.f;
  camPlanta = false;
  pinta_cubs = true;
  
  
  colFoc = glm::vec3(1,1,1);
  enviaColFocus();
  emit persp();
  emit cub1();
  angleY = 0.65;
  angleX = -1.2;
  viewTransform();
  projectTransform();
  update();
}







