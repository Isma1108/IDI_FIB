#include "ExamGLWidget.h"

class MyGLWidget:public ExamGLWidget
{
  Q_OBJECT

  public:
    MyGLWidget(QWidget *parent=0) : ExamGLWidget(parent) {}
    ~MyGLWidget();

  public slots:
    void tracta_ortogonal();
    void tracta_perspectiva();
    void tracta_cub1();
    void tracta_cub2();
    void tracta_cub3();

  signals:
    void orto();
    void persp();
    void cub1();
    void cub2();
    void cub3();

  protected:
    virtual void paintGL ();
    virtual void keyPressEvent(QKeyEvent* event);
    virtual void modelTransformCub (float escala, float angle);
    virtual void modelTransformPatricio ();
    virtual void projectTransform ();
    virtual void viewTransform ();
    virtual void initializeGL();
    virtual void resizeGL(int w, int h);

    void reset();

  private:
    int printOglError(const char file[], int line, const char func[]);

    bool pinta_cubs = true;
    float angle_cub1 = 0.f, angle_cub2 = 2*M_PI/3;
    float angle_cub3 = 4*M_PI/3;
    float angle_patr = 0.f;
    float desfase = 0.f;
};
