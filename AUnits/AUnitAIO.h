
#ifndef AUNITAIO_H
#define AUNITAIO_H

#include "ASensor.h"

class AUnitAIOi : public ASensor
{
  Q_OBJECT

 public:
  AUnitAIOi( void );
};

class AUnitAIOo : public AMotor
{
  Q_OBJECT

  double MaxV, MinV;
  
 public:
  AUnitAIOo( void );

  void setMaxV( QString maxv ) { MaxV = maxv.toDouble(); };
  void setMinV( QString minv ) { MinV = minv.toDouble(); };
};

#endif
