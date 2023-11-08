//#include <GL/glew.h>
#include "MyGLWidget.h"

#include <iostream>

MyGLWidget::MyGLWidget (QWidget* parent) : QOpenGLWidget(parent), program(NULL)
{
  setFocusPolicy(Qt::StrongFocus);  // per rebre events de teclat
}

MyGLWidget::~MyGLWidget ()
{
  if (program != NULL)
    delete program;
}

void MyGLWidget::initializeGL ()
{
  // Cal inicialitzar l'ús de les funcions d'OpenGL
  initializeOpenGLFunctions();
  
  glClearColor (0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)
  carregaShaders();
  creaBuffers();
}

void MyGLWidget::paintGL ()
{
  int mat_draw[5][5] = 
  { 1, 0, 0, 0, 1,
    1, 0, 0, 0, 1,
    1, 1, 1, 1, 1, 
    1, 0, 0, 0, 1,
    1, 0, 0, 0, 1
  };


// En cas de voler canviar els paràmetres del viewport, descomenteu la crida següent i
// useu els paràmetres que considereu (els que hi ha són els de per defecte)
  
  glClear (GL_COLOR_BUFFER_BIT);  // Esborrem el frame-buffer
  
  // Activem l'Array a pintar 
  glBindVertexArray(VAO1);

  for (int i = 0; i < 5; ++i) {
    for (int j = 0; j < 5; ++j) {
      if (mat_draw[i][j]) {
        glViewport(i*(ample/5), j*(alt/5), ample/5, alt/5);
        glDrawArrays(GL_TRIANGLES, 0, 9);
      }
    }
  }
 
  // Pintem l'escena
  //glDrawArrays(GL_TRIANGLES, 0, 9);

  // Desactivem el VAO
  glBindVertexArray(0);
}

void MyGLWidget::resizeGL (int w, int h)
{
// Aquest codi és necessari únicament per a MACs amb pantalla retina.
#ifdef __APPLE__
  GLint vp[4];
  glGetIntegerv (GL_VIEWPORT, vp);
  ample = vp[2];
  alt = vp[3];
#else
  ample = w;
  alt = h;
#endif
}

void MyGLWidget::creaBuffers ()
{
  glm::vec3 Vertices[9], Vertices2[3];  // Tres vèrtexs amb X, Y i Z
  Vertices[0] = glm::vec3(-1.0, -1.0, 0.0);
  Vertices[1] = glm::vec3(-1.0, 0.0, 0.0);
  Vertices[2] = glm::vec3(0.0, -1.0, 0.0);
  
  Vertices[3] = glm::vec3(-1.0, 0.0, 0.0);
  Vertices[4] = glm::vec3(0.0, -1.0, 0.0);
  Vertices[5] = glm::vec3(0.0, 0.0, 0.0);

  Vertices[6] = glm::vec3(-1.0, 0.0, 0.0);
Vertices[7] = glm::vec3(-0.5, 0.75, 0.0);
Vertices[8] = glm::vec3(0.0, 0.0, 0.0);
  Vertices2[0] = glm::vec3(0.5, -1.0, 0.0);
  Vertices2[1] = glm::vec3(0.5, -0.5, 0.0);
  Vertices2[2] = glm::vec3(1.0, -1.0, 0.0);
  
  
  // Creació del Vertex Array Object (VAO) que usarem per pintar
  glGenVertexArrays(1, &VAO1);
  glBindVertexArray(VAO1);

  // Creació del buffer amb les dades dels vèrtexs
  GLuint VBO1;
  glGenBuffers(1, &VBO1);
  glBindBuffer(GL_ARRAY_BUFFER, VBO1);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
  // Activem l'atribut que farem servir per vèrtex (només el 0 en aquest cas)	
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  /*
  glGenVertexArrays(1, &VAO2);
  glBindVertexArray(VAO2);
  
  GLuint VBO2;
  glGenBuffers(1, &VBO2);
  glBindBuffer(GL_ARRAY_BUFFER, VBO2);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices2), Vertices2, GL_STATIC_DRAW);
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);
  */

  // Desactivem el VAO
  glBindVertexArray(0);
}

void MyGLWidget::carregaShaders()
{
  // Creem els shaders per al fragment shader i el vertex shader
  QOpenGLShader fs (QOpenGLShader::Fragment, this);
  QOpenGLShader vs (QOpenGLShader::Vertex, this);
  // Carreguem el codi dels fitxers i els compilem
  fs.compileSourceFile("shaders/basicShader.frag");
  vs.compileSourceFile("shaders/basicShader.vert");
  // Creem el program
  program = new QOpenGLShaderProgram(this);
  // Li afegim els shaders corresponents
  program->addShader(&fs);
  program->addShader(&vs);
  // Linkem el program
  program->link();
  // Indiquem que aquest és el program que volem usar
  program->bind();

  // Obtenim identificador per a l'atribut “vertex” del vertex shader
  vertexLoc = glGetAttribLocation (program->programId(), "vertex");
}
