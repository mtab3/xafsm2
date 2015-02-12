
#ifndef AUNITAIO_H
#define AUNITAIO_H

#include "ASensor.h"
#include "AMotor.h"

class AUnitAIOi : public ASensor
{
  Q_OBJECT

 public:
  AUnitAIOi( void );
  void init0( void );
};

class AUnitAIOo : public AMotor
{
  Q_OBJECT

  double MaxV, MinV;
  
 public:
  AUnitAIOo( void );
  void init0( void );

  void setMaxV( QString maxv ) { MaxV = maxv.toDouble(); };
  void setMinV( QString minv ) { MinV = minv.toDouble(); };

  void CloseShutter( bool close );
  void GoMaxAbs( double start, double end, int steps );
  void GoMaxAbsQ( double start, double end, int steps, double time );
  void GoMaxRel( double width, int steps );
  void GoMaxRelQ( double width, int steps, double time );
};

#endif
