// MyGLWidget.cpp
#include "MyGLWidget.h"
#include "model.h"
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


void MyGLWidget::creaBuffers() {
    m.load("./models/HomerProves.obj");
    glGenVertexArrays(1, &VAO_hommer);
    glBindVertexArray(VAO_hommer);
    GLuint VBO_hommer[2];
    glGenBuffers(2, VBO_hommer);
   
    glBindBuffer(GL_ARRAY_BUFFER, VBO_hommer[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*m.faces().size()*3*3, m.VBO_vertices(), GL_STATIC_DRAW);
    glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vertexLoc);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO_hommer[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*m.faces().size()*3*3, m.VBO_matdiff(), GL_STATIC_DRAW);
    glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(colorLoc);

    glm::vec3 Vertices[6]; 
    Vertices[0] = glm::vec3(-2.0,-1.0,2.0);
    Vertices[1] = glm::vec3(2.0,-1.0,2.0);
    Vertices[2] = glm::vec3(-2.0,-1.0,-2.0);
    Vertices[3] = glm::vec3(-2.0,-1.0,-2.0);
    Vertices[4] = glm::vec3(2.0,-1.0,-2.0);
    Vertices[5] = glm::vec3(2.0,-1.0,2.0);

    glGenVertexArrays(1, &VAO_terra);
    glBindVertexArray(VAO_terra);

    GLuint VBO_terra[2];    
    glGenBuffers(2, VBO_terra);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_terra[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vertexLoc);

    glm::vec3 Colors[6]; 
    Colors[0] = glm::vec3(1.0,0.0,0.0);
    Colors[1] = glm::vec3(0.0,1.0,0.0);
    Colors[2] = glm::vec3(0.0,0.0,1.0);
    Colors[3] = glm::vec3(0.0,0.0,1.0);
    Colors[4] = glm::vec3(0.0,1.0,0.0);
    Colors[5] = glm::vec3(1.0,0.0,0.0);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO_terra[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Colors), Colors, GL_STATIC_DRAW);
    glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(colorLoc);
    
    glBindVertexArray(0);
}
void MyGLWidget::initializeGL() {
    BL2GLWidget::initializeGL();
    glEnable(GL_DEPTH_TEST);
    ini_camera();
}

void MyGLWidget::projectTransform() {
    glm::mat4 Proj = glm::perspective((float)M_PI/2.0, 1.0, 0.4, 3.0);
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, &Proj[0][0]);
}

void MyGLWidget::viewTransform() {  
    glm::mat4 View = glm::lookAt(glm::vec3(0,y_obs,z_obs), glm::vec3(0,0,0), glm::vec3(0,1,0));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &View[0][0]);
}


void MyGLWidget::paintGL() {
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    viewTransform();
    modelHommerTransform();
    glBindVertexArray(VAO_hommer);
    glDrawArrays(GL_TRIANGLES, 0, m.faces().size()*3);

    modelTerraTransform();
    glBindVertexArray(VAO_terra);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindVertexArray(0);
}

void MyGLWidget::ini_camera() {
    //actualitzaria les variables ...
    projectTransform();
    viewTransform();
}

void MyGLWidget::carregaShaders() {
    BL2GLWidget::carregaShaders();
    projLoc = glGetUniformLocation(program->programId(), "proj");
    viewLoc = glGetUniformLocation(program->programId(), "view");
    transLoc = glGetUniformLocation(program->programId(), "TG");
}

void MyGLWidget::modelHommerTransform() {
    glm::mat4 TG(1.0f);
    TG = glm::rotate(TG, ang, glm::vec3(0.0,1.0,0.0));
    glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTerraTransform() {
    glm::mat4 TG(1.0f);
    glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::keyPressEvent(QKeyEvent* event) {
    makeCurrent();
    switch (event->key()) {
        case Qt::Key_R: 
            ang += M_PI/4;
            break;
        case Qt::Key_E:
            ang -= M_PI/4;
            break;
        case Qt::Key_U:
            y_obs += 0.15;
            break;
        case Qt::Key_D:
            y_obs -= 0.15;
            break;
        case Qt::Key_P:
            z_obs += 0.25;
            break;
        case Qt::Key_O:
            z_obs -= 0.25;
            break;
    }
    update();
}

MyGLWidget::~MyGLWidget() {
}


