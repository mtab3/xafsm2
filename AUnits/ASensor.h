
#ifndef ASENSOR_H
#define ASENSOR_H

#include "AUnit0.h"

class ASensor : public AUnit0
{
  Q_OBJECT

  double setTime;       // Actually set time;
  double setDarkTime;   // Actually set time;
  double Dark;                 // back ground value normalized for 1 sec

 public:
  ASensor( void );

  virtual bool InitSensor( void );
  virtual double SetTime( double dtime );   // in sec
  double GetSetTime( void ) { return setTime; };   // actual set time
  void setDark( double dark ) { Dark = dark; emit newDark( Dark ); };
  double getDark( void ) { return Dark; };

 signals:
  void newDark( double dark );
};

#endif
