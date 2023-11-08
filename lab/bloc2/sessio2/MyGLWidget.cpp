// MyGLWidget.cpp
#include "MyGLWidget.h"
#include "model.h"
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
    m.load("./models/Patricio.obj");
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
    Vertices[0] = glm::vec3(-2.5,0.0,2.5);
    Vertices[1] = glm::vec3(2.5,0.0,2.5);
    Vertices[2] = glm::vec3(-2.5,0.0,-2.5);
    Vertices[3] = glm::vec3(-2.5,0.0,-2.5);
    Vertices[4] = glm::vec3(2.5,0.0,-2.5);
    Vertices[5] = glm::vec3(2.5,0.0,2.5);

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
    Colors[2] = glm::vec3(0.0,1.0,0.0);
    Colors[3] = glm::vec3(0.0,1.0,0.0);
    Colors[4] = glm::vec3(1.0,0.0,0.0);
    Colors[5] = glm::vec3(0.0,1.0,0.0);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO_terra[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Colors), Colors, GL_STATIC_DRAW);
    glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(colorLoc);
    
    glBindVertexArray(0);
}
void MyGLWidget::initializeGL() {
    BL2GLWidget::initializeGL();
    glEnable(GL_DEPTH_TEST);
    calcula_capsa_model();
    ini_camera();
}

void MyGLWidget::projectTransform() {
    glm::mat4 Proj(1.0);
    if (not ortho) Proj = glm::perspective(FOV, raw, znear, zfar);
    else Proj = glm::ortho(lortho, rortho, bortho, tortho, znear, zfar);
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, &Proj[0][0]);
}


void MyGLWidget::viewTransform() {  
    glm::mat4 View = glm::lookAt(OBS, VRP, glm::vec3(0,1,0));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &View[0][0]);
}


void MyGLWidget::paintGL() {
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    viewTransform();
    modelPatricioTransform();
    glBindVertexArray(VAO_hommer);
    glDrawArrays(GL_TRIANGLES, 0, m.faces().size()*3);

    modelTerraTransform();
    glBindVertexArray(VAO_terra);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindVertexArray(0);
}

void MyGLWidget::ini_camera() {
    //Coords min i max de la capsa contenidora de l'escena hard-coded
    pmin = glm::vec3(-2.5,0.0,-2.5);
    pmax = glm::vec3(2.5,4.0,2.5);

    //Les altres variables es dedueixen a partir de la capsa contenidora de l'escena
    VRP = (pmin+pmax)/2.0f; //View Reference Point, coincideix amb el centre de la capsa de l'escena
    r = glm::distance(VRP, pmin); //radi, meitat de la diagonal de la capsa
    lortho = bortho = -r;
    rortho = tortho = r;
    d = 2*r;
    raw = 1.0f; //relació d'aspecte del window, inicialment és de 1.0
    OBS = VRP + d*glm::vec3(0,0,1) + glm::vec3(0,1,0); //Coordenades del punt de l'observador
    alpha_ini = float(asin(r/d)); 
    FOV = 2*alpha_ini; //Field of view
    znear = d-r;        
    zfar = d+r;

    projectTransform();
    viewTransform();
}

void MyGLWidget::carregaShaders() {
    BL2GLWidget::carregaShaders();
    projLoc = glGetUniformLocation(program->programId(), "proj");
    viewLoc = glGetUniformLocation(program->programId(), "view");
    transLoc = glGetUniformLocation(program->programId(), "TG");
}

void MyGLWidget::modelPatricioTransform() {
    glm::mat4 TG(1.0f);
    TG = glm::rotate(TG, ang, glm::vec3(0.0,1.0,0.0));
    TG = glm::scale(TG, glm::vec3(scl_model));
    TG = glm::translate(TG, -centre_capsa_model);
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
        case Qt::Key_O:
            ortho = not ortho;
            projectTransform();
            break;
        case Qt::Key_Z:
            FOV -= 0.1;
            projectTransform();
            break;
        case Qt::Key_X:
            FOV += 0.1;
            projectTransform();
            break;
    }
    update();
}

void MyGLWidget::resizeGL(int w, int h) {
    raw = float(w)/float(h);
    //En aquest punt, la relació d'aspecte del viewport i del window son iguals
    
    //Però si rav < 1 i estem en òptica perspectiva es retalla l'escena !! :(
    //Cal recalcular el nou FOV a partir de l'angle inicial amb LA FORMULA xd
    if (not ortho && raw < 1) {
        alpha_nou = atan(tan(alpha_ini)/raw); 
        FOV = 2*alpha_nou;
    }
    //amb òptica ortogonal es retalla l'imatge si rav != raw
    //cal recalcular left, right, bottom i top
    if (ortho) {
        if (raw > 1) {
            lortho = -r*raw;
            rortho = r*raw;
        }
        else if (raw < 1) {
            bortho = -r/raw;
            tortho = r/raw;
        }
    }
    projectTransform();
}

void MyGLWidget::calcula_capsa_model() {
    float xmin, ymin, zmin, xmax, ymax, zmax;

    xmin = xmax = m.vertices()[0];
    ymin = ymax = m.vertices()[1];
    zmin = zmax = m.vertices()[2];
    
    for (unsigned int i = 3; i < m.vertices().size(); i += 3) {
        float x = m.vertices()[i], y = m.vertices()[i+1], z = m.vertices()[i+2];
        if (x < xmin) xmin = x;
        if (x > xmax) xmax = x;
        if (y < ymin) ymin = y;
        if (y > ymax) ymax = y;
        if (z < zmin) zmin = z;
        if (z > zmax) zmax = z;
    }

    centre_capsa_model = glm::vec3((xmin + xmax)/2, ymin, (zmax + zmin)/2);
    //scl_model = 4.0/(xmax-xmin);
    scl_model = 4.0/(xmax-xmin);
}


MyGLWidget::~MyGLWidget() {
}


