
#ifndef AMOTOR_H
#define AMOTOR_H

#include "AUnit0.h"

class AMotor : public AUnit0
{
  Q_OBJECT

  double UPP;           // Unit per Puls : only for PM
  
 public:
  AMotor( void );

  double getUPP( void ) { return UPP; };
  void setUPP( QString upp ) { UPP = upp.toDouble(); };

  virtual void SetValue( double v );
};

#endif
