// MyGLWidget.h
#include "BL2GLWidget.h"
#include "model.h"
#include <QTimer>

class MyGLWidget : public BL2GLWidget {
  Q_OBJECT

  public:
    MyGLWidget(QWidget *parent=0);
    ~MyGLWidget();

  public slots:
    void animacio();

  protected:
    virtual void creaBuffers();
    virtual void paintGL();
    virtual void initializeGL();
    virtual void projectTransform();
    virtual void viewTransform();
    virtual void ini_camera();
    virtual void carregaShaders();
    virtual void keyPressEvent(QKeyEvent* event);
    virtual void mouseMoveEvent(QMouseEvent* e);
    virtual void mousePressEvent(QMouseEvent* e);
    virtual void modelPatricioTransform();
    virtual void modelTerraTransform();
    virtual void resizeGL(int w, int h);  
    virtual void calcula_capsa_model();

  private:
    int printOglError(const char file[], int line, const char func[]);


    GLuint projLoc, viewLoc, transLoc;
    
    GLuint VAO_hommer, VAO_terra;
    
    Model m;
    
    float ang = 0.0;
    float z_obs = 1.25, y_obs = 0.0;
    
    glm::vec3 pmin, pmax, VRP, OBS;
    glm::vec3 centre_capsa_model;
    float r, d, alpha_ini, alpha_nou, FOV, znear, zfar, raw, scl_model;

    float lortho, rortho, bortho, tortho;

    float psi = 0, thita = 0;
    float x_ant = 0, y_ant = 0;
    bool ortho = false;

    float rot_patricio = 0;

    QTimer timer;
};



