#include <QLCDNumber>
#include <QTimer>
#include <QTime>

class MyLCDNumber : public QLCDNumber {
  Q_OBJECT
  public:
    MyLCDNumber(QWidget *parent);

    public slots:
      void tractaStartStop();
      void actualitza_digit();
      void restart();

    signals:
      void enviaRecord(QString temps);
      void enviaColorStart(QString color);
  
  private:
    bool start = false;
    QTimer timer;
    QTime record;
    QTime temps = QTime(0,0,0,0);
    bool hi_ha_record = false;
};
