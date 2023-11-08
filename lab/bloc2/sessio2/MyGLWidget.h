// MyGLWidget.h
#include "BL2GLWidget.h"
#include "model.h"

class MyGLWidget : public BL2GLWidget {
  Q_OBJECT

  public:
    MyGLWidget(QWidget *parent=0) : BL2GLWidget(parent) {}
    ~MyGLWidget();

  protected:
    virtual void creaBuffers();
    virtual void paintGL();
    virtual void initializeGL();
    virtual void projectTransform();
    virtual void viewTransform();
    virtual void ini_camera();
    virtual void carregaShaders();
    virtual void keyPressEvent(QKeyEvent* event);
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
  
    bool ortho = false;
};
