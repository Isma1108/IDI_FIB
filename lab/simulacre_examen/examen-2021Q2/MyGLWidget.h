#include "ExamGLWidget.h"

class MyGLWidget:public ExamGLWidget
{
  Q_OBJECT

  public:
    MyGLWidget(QWidget *parent=0) : ExamGLWidget(parent) {}
    ~MyGLWidget();

  public slots:
    void actualitza_gir(int num);

  protected:
    virtual void paintGL ();
    virtual void keyPressEvent(QKeyEvent* event);
    virtual void mouseMoveEvent (QMouseEvent *event);
    virtual void resizeGL(int width, int height);

    virtual void modelTransformPatricio ();
    virtual void enviaPosFocus ();
    virtual void iniEscena ();
    virtual void iniCamera ();
    virtual void projectTransform ();
    virtual void viewTransform ();

  private:
    int printOglError(const char file[], int line, const char func[]);
    glm::vec3 pmin, pmax;
    float alpha_ini, d;
    float angleX = 0.5f;
    bool focus_esc = false;
    float angle_patr = 0.0f;
    int num_ant = 0;
};
