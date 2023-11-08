// MyGLWidget.h
#include "BL3GLWidget.h"

class MyGLWidget : public BL3GLWidget {
  Q_OBJECT

  public:
    MyGLWidget(QWidget *parent=0) : BL3GLWidget(parent) {}
    ~MyGLWidget();

  protected:
    virtual void mouseMoveEvent(QMouseEvent *e);
    virtual void keyPressEvent(QKeyEvent* event);
    virtual void iniMaterialTerra();
    virtual void carregaShaders();
    virtual void initializeGL();
    virtual void modelTransformPatricio();
  

  private:
    int printOglError(const char file[], int line, const char func[]);
    
    //Locs de posFocus (escena), posFocus (camara), colorFocus (escena) y colorFocus (camara)
    GLuint posFocusLoc, posFocusCameraLoc, colorLoc, colorCameraLoc;

    glm::vec3 posFocus, posFocusCamera, colorFocus, colorFocusCamera, movimentPatricio;
    bool culling;
};
