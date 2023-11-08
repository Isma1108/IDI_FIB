#include "MyGLWidget.h"
#include <iostream>
#include <stdio.h>
#include <cmath>

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
            error = "GL_INVALID_VALUE";
            break;
        case 0x502: 
            error = "GL_INVALID_OPERATION";
            break;
        case 0x503:
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

void MyGLWidget::initializeGL() {
    LL2GLWidget::initializeGL();
    iniCamera2();
}

void MyGLWidget::paintGL() {
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  for (int i = 0; i < 2; ++i) {
    if (i == 0 or camera2on) {
      if (i == 0) {
        glm::vec3 filtre(1.0);
        glUniform3fv(filtreLoc, 1, &filtre[0]);
        viewTransform(); 
        projectTransform();
      }
      else {
        glClear (GL_DEPTH_BUFFER_BIT);
        glm::vec3 filtre(0.0, 1.0, 0.0);
        glUniform3fv(filtreLoc, 1, &filtre[0]);
        glViewport (ample*3/4, alt*3/4, ample/4, alt/4);
        viewTransform2();
        projectTransform2();
      }
      // Rick
      glBindVertexArray (VAO_models[RICK]);
      if (invisible) {
        glm::vec3 inv(0,0.7,0.0);
        glUniform3fv(invisibleLoc, 1, &inv[0]);
      }
      
      RickTransform();
      glDrawArrays(GL_TRIANGLES, 0, models[RICK].faces().size()*3);

      glm::vec3 inv(0);
      glUniform3fv(invisibleLoc, 1, &inv[0]);

      // Morty
      glBindVertexArray (VAO_models[MORTY]);
      MortyTransform();
      glDrawArrays(GL_TRIANGLES, 0, models[MORTY].faces().size()*3);
  
      // VideoCamera
      glBindVertexArray (VAO_models[VIDEOCAMERA]);
      VideoCameraTransform();
      glDrawArrays(GL_TRIANGLES, 0, models[VIDEOCAMERA].faces().size()*3);  
  
      // Terra
      glBindVertexArray (VAO_Terra);
      identTransform();
      glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
      
      // Paret
      ParetTransform();
      glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }
  }
  glBindVertexArray (0);
}


void MyGLWidget::RickTransform() 
{
    glm::mat4 TG(1.0f);  
    TG = glm::translate(TG, glm::vec3(-2, 0, -2));
    TG = glm::rotate(TG, rotRick, glm::vec3(0,1,0));
    TG = glm::scale(TG, glm::vec3(escalaModels[RICK]));
    TG = glm::translate(TG, -centreBaseModels[RICK]);
    glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::VideoCameraTransform ()
{
    glm::mat4 TG(1.0f);
    TG = glm::translate(TG, glm::vec3(-4, 3, -4));
    TG = glm::rotate(TG, rotYcamera, glm::vec3(0,1,0));
    TG = glm::rotate(TG, rotXcamera, glm::vec3(1,0,0));
    TG = glm::scale(TG, glm::vec3(escalaModels[VIDEOCAMERA]));
    TG = glm::translate(TG, -centreBaseModels[VIDEOCAMERA]);
    glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::MortyTransform() {
    glm::mat4 TG(1.0f);
    TG = glm::translate(TG, posMorty);
    TG = glm::rotate(TG, angleMorty, glm::vec3(0,1,0));
    TG = glm::scale(TG, glm::vec3(escalaModels[MORTY]));
    TG = glm::translate(TG, -centreBaseModels[MORTY]);
    glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::iniEscena()
{
    //Coordenades dels punts mínim i màxim de l'escena hard-coded.
    pmin = glm::vec3(-4.0,0.0,-4.0);
    pmax = glm::vec3(4.0,3.0,4.0);
}

void MyGLWidget::iniCamera(){
    vrp = (pmin + pmax)/2.0f;
    radiEscena = glm::distance(vrp, pmax);
    d = 2*radiEscena;
    obs = vrp + d*glm::vec3(0,0,1);
    up = glm::vec3(0, 1, 0);
    alpha_ini = float(asin(radiEscena/d));
    fov = 2*alpha_ini;
    znear = d - radiEscena;
    zfar = d + radiEscena;
    ra = 1.0f;
    psi = 0;
    theta = M_PI/4;
}

void MyGLWidget::iniCamera2() {
    obs2 = glm::vec3(-4,3,-4);
    vrp2 = glm::vec3(-2,0.2,-2);
    fov2 = glm::radians(float(60));
    znear2 = 0.25;
    zfar2 = 10;
    rotYcamera = rotXcamera = M_PI/4;
    actualitza_vrp();
}

void MyGLWidget::viewTransform ()
{
    glm::mat4 View(1.0);
    View = glm::translate(View, glm::vec3(0.0,0.0,-d));
    View = glm::rotate(View, theta, glm::vec3(1.0,0.0,0.0));
    View = glm::rotate(View, -psi, glm::vec3(0.0,1.0,0.0));
    View = glm::translate(View, -vrp);
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &View[0][0]);
}

void MyGLWidget::projectTransform2() {
  glm::mat4 Proj(1.0f);
  Proj = glm::perspective (fov2, ra, znear2, zfar2);
  glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
}

void MyGLWidget::viewTransform2() {
  // Matriu de posició i orientació de l'observador
  glm::mat4 View(1.0f);
  View = glm::lookAt (obs2, vrp2, up);
  glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
}

void MyGLWidget::mouseMoveEvent(QMouseEvent *e)
{
    makeCurrent();
    
    psi -= (e->x() - xClick)*factorAngleX;
    theta += (e->y() - yClick)*factorAngleY;
    xClick = e->x();
    yClick = e->y();

    update ();
}

void MyGLWidget::actualitza_vrp()
{
  float aux = glm::distance(glm::vec2(obs2.x, obs2.z), glm::vec2(vrp2.x, vrp2.z));
  vrp2 = obs2 + glm::vec3(aux*glm::sin(rotYcamera), -obs2.y, aux*glm::cos(rotYcamera)) + glm::vec3(0,0.2,0);
}

void MyGLWidget::restart() 
{
  psi = 0;
  theta = M_PI/4;
  rotYcamera = M_PI/4;
  actualitza_vrp();
  posMorty = glm::vec3(1,0,0);
  angleMorty = 0;
  invisible = false;
  camera2on = false;
}

void MyGLWidget::keyPressEvent(QKeyEvent* event) {
    makeCurrent();
        switch (event->key()) {
            case Qt::Key_Up: { 
              posMorty = posMorty + glm::vec3(sin(angleMorty), 0, cos(angleMorty))*glm::vec3(0.1);
              break;
            }
            case Qt::Key_Down: {
              posMorty = posMorty - glm::vec3(sin(angleMorty), 0, cos(angleMorty))*glm::vec3(0.1);
              break;
            }
            case Qt::Key_Left: {
                angleMorty += glm::radians(float(45));
                break;
            }
            case Qt::Key_Right: {
                angleMorty -= glm::radians(float(45));
                break;
            }
            case Qt::Key_Q: {
                if (rotYcamera < M_PI/2) {
                  rotYcamera += glm::radians(float(1));
                  actualitza_vrp();
                }
                break;
            }
            case Qt::Key_E: {
                if (rotYcamera > 0) {
                  rotYcamera -= glm::radians(float(1));
                  actualitza_vrp();
                }
                break;
            }
            case Qt::Key_C: {
              camera2on = not camera2on; 
              break;
            }
            case Qt::Key_G: {
              invisible = not invisible;
              break;
            }
            case Qt::Key_R: {
              restart();
              break;
            }
            case Qt::Key_T: {
              autocam = not autocam;
              if (autocam) timer.start(1000);
              else timer.stop(); 
              break;
            }
            default: event->ignore(); break;
        }

    update();
}

void MyGLWidget::carregaShaders() {
  LL2GLWidget::carregaShaders();
  filtreLoc = glGetUniformLocation (program->programId(), "filtre");
  invisibleLoc = glGetUniformLocation (program->programId(), "invisible");
}

void MyGLWidget::resizeGL(int w, int h) {
  LL2GLWidget::resizeGL(w, h);
  ra = float(w)/float(h);
  if (ra < 1) {
    alpha_nou = atan(tan(alpha_ini)/ra);
    fov = 2*alpha_nou;
  }
  projectTransform();
}

void MyGLWidget::animacio() {
  makeCurrent();   

  if (rotYcamera + glm::radians(float(15)) > M_PI/2) q_auto = false;
  else if (rotYcamera - glm::radians(float(15)) < 0) q_auto = true;

  if (q_auto) rotYcamera += glm::radians(float(15));
  else rotYcamera -= glm::radians(float(15));
  
  actualitza_vrp(); 
  update();
}

MyGLWidget::MyGLWidget(QWidget *parent) : LL2GLWidget(parent) {
    connect(&timer, SIGNAL(timeout()), this, SLOT(animacio()));
}

MyGLWidget::~MyGLWidget() {}
