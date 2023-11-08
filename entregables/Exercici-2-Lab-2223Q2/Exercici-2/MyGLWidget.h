#include "LL2GLWidget.h"

#include <QTimer>
#include "model.h"

class MyGLWidget : public LL2GLWidget {
  Q_OBJECT

  public:
    MyGLWidget(QWidget *parent=0);
    ~MyGLWidget();

  public slots:
    void animacio();

  protected:
    virtual void initializeGL();
    virtual void paintGL();
    virtual void iniCamera2();
    virtual void RickTransform();
    virtual void MortyTransform();
    virtual void VideoCameraTransform();
    virtual void iniEscena();
    virtual void iniCamera();
    virtual void viewTransform();
    virtual void mouseMoveEvent(QMouseEvent *e);
    virtual void keyPressEvent(QKeyEvent* event);
    virtual void projectTransform2();
    virtual void viewTransform2();
    virtual void carregaShaders();
    virtual void resizeGL(int w, int h);
    virtual void actualitza_vrp(); 
    virtual void restart();

  private:

    int printOglError(const char file[], int line, const char func[]);

    //Rotació rick
    float rotRick = M_PI;

    //Rotacio videocamera
    float rotYcamera = M_PI/4, rotXcamera = M_PI/4;

    //distancia camera2 (videocamera)
    float d;

    //Punts minim i maxim de la caixa contenidora de l'escena;
    glm::vec3 pmin, pmax;

    //Paràmetres de l'òptica perspectiva de la càmera 1
    float alpha_ini, alpha_nou;

    //Paràmeres de l'òptica perspectiva i la vista de la càmera 2
    float fov2, ra2, znear2, zfar2; 
    glm::vec3 obs2, vrp2;

    //Angles d'Euler
    float psi, theta;

    //Activacio de la camera2 (videocamera)
    bool camera2on = false;

    //invisibilitat del Rick
    bool invisible = false;

    //Sentit del gir automatic de la videocamera
    bool q_auto = true;

    //Activacio del moviment automatic de la videocamera
    bool autocam = false;

    //Filtre verd i invisibilitat
    GLuint filtreLoc, invisibleLoc;

    //Timer
    QTimer timer;
};
