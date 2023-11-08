#include "MyLCDNumber.h"

MyLCDNumber::MyLCDNumber(QWidget *parent): QLCDNumber(parent) {
  connect(&timer, SIGNAL(timeout()), this, SLOT(actualitza_digit()));
  setDigitCount(12);
  display(temps.toString("hh:mm:ss:zzz"));
}

void MyLCDNumber::actualitza_digit() {
  temps = temps.addMSecs(1);
  display(temps.toString("hh:mm:ss:zzz"));
  if (hi_ha_record) {
    if (temps < record) setStyleSheet("background-color: rgb(55, 232, 11);");
    else if (temps == record) setStyleSheet("background-color: rgb(0, 0, 0);");
    else setStyleSheet("background-color: rgb(235, 30, 30);");
  }
}

void MyLCDNumber::tractaStartStop() {
  if (not start) {
    start = true;
    timer.start(1); 
    emit enviaColorStart("background-color: rgb(237, 51, 59);");
  }
  else {
    start = false;   
    timer.stop();
    emit enviaColorStart("background-color: rgb(98, 160, 234);");
  }
}

void MyLCDNumber::restart() { 
  start = false;
  timer.stop();
  if (not hi_ha_record) {
    hi_ha_record = true;
    record = temps;
    emit enviaRecord(temps.toString("hh:mm:ss.zzz"));
  }
  else {
    if (temps < record) {
      record = temps;
      emit enviaRecord(temps.toString("hh:mm:ss.zzz"));
    }
  }
  temps = QTime(0,0,0,0);
  display(temps.toString("hh:mm:ss:zzz"));
  setStyleSheet("background-color: rgb(55, 232, 11);");
  emit enviaColorStart("background-color: rgb(98, 160, 234);");
}

